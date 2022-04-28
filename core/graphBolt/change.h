#ifndef KICKSTARTER_ENGINE_H
#define KICKSTARTER_ENGINE_H

#include "../common/bitsetscheduler.h"
#include "../common/utils.h"
#include "ingestor.h"
#include <vector>

#define MAX_LEVEL 65535
#define MAX_PARENT 4294967295

#ifdef EDGEDATA
#else
struct EmptyEdgeData{};
typedef EmptyEdgeData EdgeData;

EdgeData emptyEdgeData;
#endif

  void deltaCompute(edgeArray &edge_additions, edgeArray &edge_deletions) {
    timer iteration_timer, phase_timer, full_timer;
    double misc_time, copy_time, phase_time, iteration_time;
    full_timer.start();

    // Handle newly added vertices
    n_old = n;
    if (edge_additions.maxVertex >= n) {
      processVertexAddition(edge_additions.maxVertex);
    }

    // Reset values before incremental computation
    active_vertices_bitset.reset();
    parallel_for(uintV v = 0; v < n; v++) {
      frontier[v] = 0;
      // all_affected_vertices is used only for switching purposes
      all_affected_vertices[v] = 0;
      changed[v] = 0;
      // Make a copy of the old dependency data
      dependency_data_old[v] = dependency_data[v];
    }

    // ======================================================================
    // PHASE 1 - Update global_info
    // ======================================================================
    // Pretty much nothing is going to happen here. But, maintaining consistency with GraphBolt
    global_info_old.copy(global_info);
    global_info.processUpdates(edge_additions, edge_deletions);

    // ======================================================================
    // PHASE 2 = Identify vertex values affected by edge deletions
    // ======================================================================
    bool frontier_not_empty = false;
    parallel_for(long i = 0; i < edge_deletions.size; i++) {
      uintV source = edge_deletions.E[i].source;
      uintV destination = edge_deletions.E[i].destination;
      if (dependency_data[destination].parent == source) {
        dependency_data[destination].reset();
        initializeVertexValue<VertexValueType, GlobalInfoType>(
            destination, dependency_data[destination].value, global_info);
        active_vertices_bitset.schedule(destination);
        all_affected_vertices[destination] = true;
      }
    }

    // ======================================================================
    // PHASE 3 - Trimming phase
    // ======================================================================
    bool should_switch_now = false;
    bool use_delta = true;
    while (active_vertices_bitset.anyScheduledTasks()) {

      // For all the vertices 'v' affected, update value of 'v' from its
      // inNghs, such that level(v) > level(inNgh) in the old dependency tree
      active_vertices_bitset.newIteration();
      parallel_for(uintV v = 0; v < n; v++) {
        if (active_vertices_bitset.isScheduled(v)) {
          intE inDegree = my_graph.V[v].getInDegree();
          DependencyData<VertexValueType> v_value_old = dependency_data[v];
          parallel_for(intE i = 0; i < inDegree; i++) {
            uintV u = my_graph.V[v].getInNeighbor(i);
            // Process inEdges with smallerLevel than currentVertex.
            if (dependency_data_old[v].level > dependency_data_old[u].level) {
#ifdef EDGEDATA
              EdgeData *edge_data = my_graph.V[v].getInEdgeData(i);
#else
              EdgeData *edge_data = &emptyEdgeData;
#endif
              bool ret =
                  reduce(u, v, *edge_data, dependency_data[u], v_value_old, global_info);
            }
          }
          // Evaluate the shouldReduce condition.. See if the new value is
          // greater than the old value
          if ((shouldPropagate(dependency_data_old[v].value,
                               dependency_data[v].value, global_info)) ||
              (shouldPropagate(v_value_old.value, dependency_data[v].value,
                               global_info))) {
            changed[v] = 1;
          }
        }
      }

      parallel_for(uintV v = 0; v < n; v++) {
        if (changed[v]) {
          changed[v] = 0;
          // Push down in dependency tree
          intE outDegree = my_graph.V[v].getOutDegree();
          DependencyData<VertexValueType> v_value = dependency_data[v];
          parallel_for(intE i = 0; i < outDegree; i++) {
            uintV w = my_graph.V[v].getOutNeighbor(i);
            // Push the changes down only to its outNghs in the dependency
            // tree
            if (dependency_data[w].parent == v) {
              DependencyData<VertexValueType> newV, oldV;
              oldV = dependency_data[w];

              // Reset dependency_data[w]
              dependency_data[w].reset();
              initializeVertexValue<VertexValueType, GlobalInfoType>(
                  w, dependency_data[w].value, global_info);
              newV = dependency_data[w];

              // Update w's value based on u's value if needed
#ifdef EDGEDATA
              EdgeData *edge_data = my_graph.V[v].getOutEdgeData(i);
#else
              EdgeData *edge_data = &emptyEdgeData;
#endif
              bool ret = reduce(v, w, *edge_data, v_value, newV, global_info);

              if ((oldV.value != newV.value) || (oldV.level != newV.level)) {
                dependency_data[w] = newV;
                all_affected_vertices[w] = true;

                if ((shouldPropagate(dependency_data_old[w].value, newV.value,
                                     global_info)) ||
                    (shouldPropagate(oldV.value, newV.value, global_info))) {
                  active_vertices_bitset.schedule(w);
                }
                if (shouldPropagate(oldV.value, newV.value, global_info)) {
                  frontier[w] = 1;
                }
              }
            }
          }
        }
      }
      bool *temp = changed;
      changed = frontier;
      frontier = temp;
    }

    // Pull once for all the affected vertices
    parallel_for(uintV v = 0; v < n; v++) {
      if (all_affected_vertices[v] == 1) {
        intE inDegree = my_graph.V[v].getInDegree();
        parallel_for(intE i = 0; i < inDegree; i++) {
          uintV u = my_graph.V[v].getInNeighbor(i);
#ifdef EDGEDATA
          EdgeData *edge_data = my_graph.V[v].getInEdgeData(i);
#else
          EdgeData *edge_data = &emptyEdgeData;
#endif
          bool ret =
              reduce(u, v, *edge_data, dependency_data[u], dependency_data[v], global_info);
        }
      }
    }

    // ======================================================================
    // PHASE 4 - Process additions
    // ======================================================================
    parallel_for(long i = 0; i < edge_additions.size; i++) {
      uintV source = edge_additions.E[i].source;
      uintV destination = edge_additions.E[i].destination;
#ifdef EDGEDATA
      EdgeData *edge_data = edge_additions.E[i].edgeData;
#else
      EdgeData *edge_data = &emptyEdgeData;
#endif
      bool ret = reduce(source, destination, *edge_data, dependency_data[source],
                        dependency_data[destination], global_info);
      if (ret) {
        all_affected_vertices[destination] = true;
      }
    }

    // ======================================================================
    // PHASE 5 - Traditional processing
    // ======================================================================
    // For all affected vertices, start traditional processing
    active_vertices_bitset.reset();
    parallel_for(uintV v = 0; v < n; v++) {
      if (all_affected_vertices[v] == 1) {
        active_vertices_bitset.schedule(v);
      }
    }
    traditionalIncrementalComputation();

    cout << "Finished batch : " << full_timer.stop() << "\n";
    printOutput();
  }