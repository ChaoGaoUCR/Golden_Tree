// Copyright (c) 2020 Mugilan Mariappan, Joanna Che and Keval Vora.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights (to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef GoldenTree_ENGINE_H
#define GoldenTree_ENGINE_H

#include "../common/bitsetscheduler.h"
#include "../common/utils.h"
#include "../graph/IO.h"
#include "ingestor.h"
#include "../common/parallel.h"
#include <vector>

#define MAX_LEVEL 65535
#define MAX_PARENT 4294967295

#ifdef EDGEDATA
#else
struct EmptyEdgeData{};
typedef EmptyEdgeData EdgeData;

EdgeData emptyEdgeData;
#endif

// ======================================================================
// VertexValue INITIALIZATION
// ======================================================================
// Set the initial value of the vertex
template <class VertexValueType, class GlobalInfoType>
inline void initializeVertexValue(const uintV &v,
                                  VertexValueType &v_vertex_value,
                                  const GlobalInfoType &global_info);

// // // ======================================================================
// ACTIVATE VERTEX FOR FIRST ITERATION
// ======================================================================
// Return whether a vertex should be active when the processing starts. For
// BFS/SSSP, only source vertex returns true. For CC, all vertices return true.
template <class GlobalInfoType>
inline bool frontierVertex(const uintV &v, const GlobalInfoType &global_info);

// ======================================================================
// EDGE FUNCTION
// ======================================================================
// For an edge (u, v), compute v's value based on u's value.
// Return false if the value from u should not be use to update the value of v.
// Return true otherwise.
template <class VertexValueType, class EdgeDataType, class GlobalInfoType>
inline bool edgeFunction(const uintV &u, const uintV &v,
                         const EdgeDataType &edge_data,
                        //  const VertexValueType &u_value,
                         VertexValueType &v_value, GlobalInfoType &global_info);

// ======================================================================
// SHOULDPROPAGATE
// ======================================================================
// shouldPropagate condition for deciding if the value change in
// updated graph violates monotonicity
template <class VertexValueType, class GlobalInfoType>
inline bool shouldPropagate(const VertexValueType &old_value,
                            const VertexValueType &new_value,
                            GlobalInfoType &global_info);

// ======================================================================
// HELPER FUNCTIONS
// ======================================================================
template <class GlobalInfoType>
void printAdditionalData(ofstream &output_file, const uintV &v,
                         GlobalInfoType &info);

// ======================================================================
// GoldenTree ENGINE
// ======================================================================
template <class vertex, class VertexValueType, class GlobalInfoType>
class GoldenTreeEngine {

public:
  graph<vertex> &my_graph;
  // graph<vertex> tmpGraph;
  commandLine config;

  // Current Graph graph
  // number of vertices in current graph
  long n;
  GlobalInfoType &global_info;

  // Previous graph
  // number of vertices in old graph
  long n_old;
  GlobalInfoType global_info_old;

  template <class T> struct DependencyData {
    uintV parent;
    T value;
    uint16_t level;
    DependencyData() : level(MAX_LEVEL), value(), parent(MAX_PARENT) {}

    DependencyData(uint16_t _level, T _value, uint32_t _parent)
        : level(_level), value(_value), parent(_parent) {}

    DependencyData(const DependencyData &object)
        : level(object.level), value(object.value), parent(object.parent) {}

    void reset() {
      parent = MAX_PARENT;
      level = MAX_LEVEL;
    }

    inline bool operator==(const DependencyData &rhs) {
      if ((value == rhs.value) && (parent == rhs.parent) &&
          (level == rhs.level))
        return true;
      else
        return false;
    }

    inline bool operator!=(const DependencyData &rhs) {
      if ((value != rhs.value) || (parent != rhs.parent) ||
          (level != rhs.level))
        return true;
      else
        return false;
    }

    template <class P>
    friend ostream &operator<<(ostream &os, const DependencyData<P> &dt);
  };

  template <class P>
  friend ostream &operator<<(ostream &os, const DependencyData<P> &dt) {
    // os << dt.value << " " << dt.level;
    os << dt.value;
    return os;
  }

  DependencyData<VertexValueType> *dependency_data;
  DependencyData<VertexValueType> *dependency_data_old;
  

  // TODO : Replace with more efficient vertexSubset using bitmaps
  bool *frontier;
  bool *all_affected_vertices;
  bool *changed;

  // graph<vertex> *multi;
  graph<vertex>** snapshot;
  edgeArray** tree_leaf;

  BitsetScheduler active_vertices_bitset;

  // Stream Ingestor
  Ingestor<vertex> ingestor;
  int current_batch;

  GoldenTreeEngine(graph<vertex> &_my_graph, GlobalInfoType &_global_info,
                    commandLine _config)
      : my_graph(_my_graph), global_info(_global_info), global_info_old(),
        config(_config), ingestor(_my_graph, _config), current_batch(0),
        active_vertices_bitset(my_graph.n) {
    // &tmpGraph = new graph<vertex>;         
    n = my_graph.n;
    n_old = 0;
    // multi = multi_graph<vertex>(ingestor.numberOfSnapshots, my_graph.n);

    

  }

  void init() {
    createDependencyData();
    createTemporaryStructures();
    createVertexSubsets();
    initVertexSubsets();
    initTemporaryStructures();
    initDependencyData();
    initial_all_snapshot();
    initial_edgearray();

    // edgeArray        

  }

  ~GoldenTreeEngine() {
    freeDependencyData();
    freeTemporaryStructures();
    freeVertexSubsets();
    global_info.cleanup();
    free_snapshot();
    free_edge_array();


  }

  // ======================================================================
  // DEPENDENCY DATA STORAGE
  // ======================================================================
  void createDependencyData() {
    dependency_data = newA(DependencyData<VertexValueType>, n);
  }
  void resizeDependencyData() {
    dependency_data =
        renewA(DependencyData<VertexValueType>, dependency_data, n);
    initDependencyData(n_old, n);
  }
  void freeDependencyData() { deleteA(dependency_data); }
  void initDependencyData() { initDependencyData(0, n); }
  void initDependencyData(long start_index, long end_index) {
    parallel_for(long v = start_index; v < end_index; v++) {
      dependency_data[v].reset();
      initializeVertexValue<VertexValueType, GlobalInfoType>(
          v, dependency_data[v].value, global_info);
    }
  }

  // ======================================================================
  // TEMPORARY STRUCTURES USED BY THE ENGINE
  // ======================================================================
  virtual void createTemporaryStructures() {
    dependency_data_old = newA(DependencyData<VertexValueType>, n);
  }
  virtual void resizeTemporaryStructures() {
    dependency_data_old =
        renewA(DependencyData<VertexValueType>, dependency_data_old, n);
    initDependencyData(n_old, n);
  }
  virtual void freeTemporaryStructures() { deleteA(dependency_data_old); }
  virtual void initTemporaryStructures() { initTemporaryStructures(0, n); }
  virtual void initTemporaryStructures(long start_index, long end_index) {}

  // ======================================================================
  // VERTEX SUBSETS USED BY THE ENGINE
  // ======================================================================
  void createVertexSubsets() {
    frontier = newA(bool, n);
    all_affected_vertices = newA(bool, n);
    changed = newA(bool, n);
  }
  void resizeVertexSubsets() {
    frontier = renewA(bool, frontier, n);
    all_affected_vertices = renewA(bool, all_affected_vertices, n);
    changed = renewA(bool, changed, n);
    initVertexSubsets(n_old, n);
  }
  void freeVertexSubsets() {
    deleteA(frontier);
    deleteA(all_affected_vertices);
    deleteA(changed);
  }
  void initVertexSubsets() { initVertexSubsets(0, n); }
  void initVertexSubsets(long start_index, long end_index) {
    parallel_for(long j = start_index; j < end_index; j++) {
      frontier[j] = 0;
      all_affected_vertices[j] = 0;
      changed[j] = 0;
    }
  }
  void initial_edgearray(){
    tree_leaf = new edgeArray*[ingestor.numberOfSnapshots];
    for (size_t i = 0; i < ingestor.numberOfSnapshots; i++)
    {
      tree_leaf[i] =  new edgeArray();
    }
    
    cout<<"edgeArray construct done"<<endl;
  }
  void initial_all_snapshot(){
    snapshot = new graph<vertex>*[ingestor.numberOfSnapshots];
    for (size_t i = 0; i < ingestor.numberOfSnapshots; i++)
    {
      vertex *v = newA(vertex, my_graph.n);
      unsigned long n = my_graph.n;
      unsigned long m = 0;      
      uintV *edges = newA(uintV, m);
      uintV *inEdges = newA(uintV, m);
      intE *offsets = newA(intE, n);
      intE *toffsets = newA(intE, n);
      AdjacencyRep<vertex> *mem = new AdjacencyRep<vertex>(v, n, m, edges, inEdges, offsets, toffsets);
      // bool *updated_vertices;      
      // updated_vertices = newA(bool, my_graph.n);
      snapshot[i] = new graph<vertex>(v, n, m, mem);  
      // for (uintV i = 0; i < maxVertices; i++) updated_vertices[i] = 0;
      // cout<<"In number "<<i<<" we have "<<snapshot[i]->n<<" vertices"<<" and "<< snapshot[i]->m<<" edges"<<endl;
    }
  }
  void free_snapshot(){
    for (size_t i = 0; i < ingestor.numberOfSnapshots; i++)
    {
      snapshot[i]->del();
    }
    cout<<"snapshots deletion complete"<<endl;    
  }
  void free_edge_array(){
    for (size_t i = 0; i < ingestor.numberOfSnapshots; i++)
    {
      tree_leaf[i]->del();
    }
    cout<<"tree delete done"<<endl;
  }
  void processVertexAddition(long maxVertex) {
    n_old = n;
    n = maxVertex + 1;
    resizeDependencyData();
    resizeTemporaryStructures();
    resizeVertexSubsets();
  }

  void testPrint() {
    cout << setprecision(VAL_PRECISION);
    for (auto curr : debug_vertices) {
      cout << "Vertex " << curr << "\n";
      cout << "Indegree " << my_graph.V[curr].getInDegree() << "\n";
      cout << "Outdegree " << my_graph.V[curr].getOutDegree() << "\n";
      cout << "DependencyData<VertexValueType> " << dependency_data[curr]
           << "\n";
      cout << "DependencyDataOld " << dependency_data_old[curr] << "\n";
    }
  }

  void printOutput() {
    string output_file_path = config.getOptionValue("-outputFile", "/tmp/");
    bool should_print = true;
    if (output_file_path.compare("/tmp/") == 0) {
      should_print = false;
    }
    if (should_print) {
      string curr_output_file_path =
          output_file_path + to_string(current_batch);
      std::cout << "Printing to file : " << curr_output_file_path << "\n";
      ofstream output_file;
      output_file.open(curr_output_file_path, ios::out);
      output_file << fixed;
      output_file << setprecision(VAL_PRECISION2);
      for (uintV v = 0; v < n; v++) {
        // output_file << v << " " << my_graph.V[v].getInDegree() << " "
        //             << my_graph.V[v].getOutDegree() << " ";
        // printAdditionalData(output_file, v, global_info);
        output_file << v <<"\t"<< dependency_data[v].parent <<"\t" << dependency_data[v].level<<
          dependency_data[v].value  <<"\n";
      }
    }
    cout << "\n";
    current_batch++;
  }

  void run() {
    // cout<<ingestor.number_of_batches<<endl;
    initialCompute();
    ingestor.validateAndOpenFifo();
    while (ingestor.processNextBatch()) {
      edgeArray &edge_additions = ingestor.getEdgeAdditions();
      edgeArray &edge_deletions = ingestor.getEdgeDeletions();
      deltaCompute(edge_additions, edge_deletions);
    }
  }


  void initialCompute() {
    timer t1, full_timer;
    full_timer.start();
    active_vertices_bitset.reset();

    parallel_for(uintV v = 0; v < n; v++) {
      if (frontierVertex(v, global_info)) {
        active_vertices_bitset.schedule(v);
        dependency_data[v].level = 0;
        dependency_data[v].parent = v;
      }
    }

    traditionalIncrementalComputation();
    cout << "Initial graph processing : " << full_timer.stop() << "\n";
    printOutput();
  }

  // TODO : Write a lock based reduce function. Add functionality to use the
  // lock based reduce function depending on the size of DependendencyData              
  bool reduce(const uintV &u, const uintV &v, const EdgeData &edge_data,
              const DependencyData<VertexValueType> &u_data,
              DependencyData<VertexValueType> &v_data, GlobalInfoType &info) {
    DependencyData<VertexValueType> newV, oldV;
    DependencyData<VertexValueType> incoming_value_curr = u_data;

    bool ret = edgeFunction(u, v, edge_data, incoming_value_curr.value, newV.value, info);
    if (!ret) {
      return false;
    }
    newV.level = incoming_value_curr.level + 1;
    newV.parent = u;

    bool update_successful = true;
    do {
      oldV = v_data;
      // If oldV is lesser than the newV computed frm u, we should update.
      // Otherwise, break
      if ((shouldPropagate(oldV.value, newV.value, global_info)) ||
          ((oldV.value == newV.value) && (oldV.level <= newV.level))) {
        update_successful = false;
        break;
      }
    } while (!CAS(&v_data, oldV, newV));
    return update_successful;
  }


  int edgeListIncrementalComputation(graph<vertex> &edgeGraph){
    // cout<<"-----------------------------------------------------------------"<<endl;
    // cout<<"inside this function"<<active_vertices_bitset.anyScheduledTasks()<<endl;
    cout<<"--------------------------"<<endl;
    cout<<"print schedule status  "<<active_vertices_bitset.anyScheduledTasks()<<endl;
    cout<<"--------------------------"<<endl;
    long iterations = 0;
    while (active_vertices_bitset.anyScheduledTasks()) {
    // cout<<"-----------------------------------------------------------------"<<endl;
      iterations++;

      // cout<<"compute start"<<endl;
      active_vertices_bitset.newIteration();
      parallel_for(uintV u = 0; u < n; u++) {
        if (active_vertices_bitset.isScheduled(u)) {
          // process all its outNghs

          intE Small_outDegree = edgeGraph.V[u].getOutDegree();
          granular_for(i, 0, Small_outDegree, (Small_outDegree > 1024), {
            uintV v = edgeGraph.V[u].getOutNeighbor(i);
#ifdef EDGEDATA
            EdgeData *edge_data = edgeGraph.V[u].getOutEdgeData(i);
#else
            EdgeData *edge_data = &emptyEdgeData;
#endif
            bool ret = reduce(u, v, *edge_data, dependency_data[u], dependency_data[v],
                              global_info);
            if (ret) {
              active_vertices_bitset.schedule(v);
            }
          })
          // cout<<"small graph is done"<<endl;
          intE outDegree = my_graph.V[u].getOutDegree();
          granular_for(i, 0, outDegree, (outDegree > 1024), {
            uintV v = my_graph.V[u].getOutNeighbor(i);
#ifdef EDGEDATA
            EdgeData *edge_data = my_graph.V[u].getOutEdgeData(i);
#else
            EdgeData *edge_data = &emptyEdgeData;
#endif
            bool ret = reduce(u, v, *edge_data, dependency_data[u], dependency_data[v],
                              global_info);
            if (ret) {
              active_vertices_bitset.schedule(v);
            }
          });
          // cout<<"large graph is done"<<endl;

          

        }
      }
    }
    cout << "iterations: " << iterations << endl;


  }

  int traditionalIncrementalComputation() {
    cout<<"--------------------------"<<endl;
    cout<<"print schedule status  "<<active_vertices_bitset.anyScheduledTasks()<<endl;
    cout<<"--------------------------"<<endl;    
    while (active_vertices_bitset.anyScheduledTasks()) {
      active_vertices_bitset.newIteration();
      parallel_for(uintV u = 0; u < n; u++) {
        if (active_vertices_bitset.isScheduled(u)) {
          // process all its outNghs
          intE outDegree = my_graph.V[u].getOutDegree();
          granular_for(i, 0, outDegree, (outDegree > 1024), {
            uintV v = my_graph.V[u].getOutNeighbor(i);
#ifdef EDGEDATA
            EdgeData *edge_data = my_graph.V[u].getOutEdgeData(i);
#else
            EdgeData *edge_data = &emptyEdgeData;
#endif
            bool ret = reduce(u, v, *edge_data, dependency_data[u], dependency_data[v],
                              global_info);
            if (ret) {
              active_vertices_bitset.schedule(v);
            }
          });
        }
      }
    }
  }

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
      // cout<<ret<<endl;
      if (ret) {
      cout<<ret<<endl;
        
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
  
  void none_del_delta_compute(edgeArray &edge_additions){
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
    global_info.add_processUpdates(edge_additions);    
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
      // cout<<ret<<endl;                        
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

    graph <vertex> tmp = graph_From_edges(edge_additions, ingestor.my_graph);
    
      // cout<<"tmp graph has "<<tmp.m<<endl;
      // cout<<active_vertices_bitset.anyScheduledTasks()<<endl;

      edgeListIncrementalComputation(tmp);    
    // cout<<dependency_data[0]<<endl;
    tmp.del();
    cout << "Finished batch : " << full_timer.stop() << "\n";
    printOutput();    

  }


  // void test(){

  // }
};

#endif
