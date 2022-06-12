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
// #include <cilk-lib.h>
// #include <mutex>
// #include <tbb/mutex.h>

#define MAX_LEVEL 65535
#define MAX_PARENT 4294967295

#ifdef EDGEDATA
#else
struct EmptyEdgeData{};
typedef EmptyEdgeData EdgeData;
// typedef pair <edgeArray, edgeArray>;

EdgeData emptyEdgeData;
#endif
template <class vertex>
void none_order_overlapping(graph<vertex> &big_graph, graph<vertex> &small_graph){
  // RWLock edgelock;
  // edgelock.init();
  uintE count = 0;
  // edge *res = newA(edge, small_graph.m);
  parallel_for(uintV i=0; i< small_graph.n; i++){
    parallel_for(uintE j =0; j<small_graph.V[i].getOutDegree(); j++){
      // edge tmp = edge(i, small_graph.V[i].getOutNeighbor(j));
      if (big_graph.single_check_edge_in_graph(edge(i, small_graph.V[i].getOutNeighbor(j)))){
        // count++;
        pbbs::fetch_and_add(&count, 1);
      }
    }
  }
  cout<<"number of overalapping is "<< count<<endl;


  edge *res = newA(edge, count);
  uintE count_new=0;
  for(uintV i=0; i< small_graph.n; i++){
    for(uintE j =0; j<small_graph.V[i].getOutDegree(); j++){
      // edge tmp = edge(i, small_graph.V[i].getOutNeighbor(j));
      if (big_graph.single_check_edge_in_graph(edge(i, small_graph.V[i].getOutNeighbor(j)))){
        // cout<<i<<" "<<small_graph.V[i].getOutNeighbor(j)<<endl;
        // pbbs::fetch_and_add(&count_new,1);
        count_new++;
        res[count_new-1] = edge(i, small_graph.V[i].getOutNeighbor(j));
        // pbbs::fetch_and_add(&count_new,1);
      }
    }
  }
  uintE wrong_number = 0;
  parallel_for(uintE i=0; i<count; i++){
    if (!small_graph.single_check_edge_in_graph(res[i]) || !big_graph.single_check_edge_in_graph(res[i]))
    {
      // cout<<"number " <<i<<" not in small graph or big graph, smaple wrong"<<endl;
      pbbs::fetch_and_add(&wrong_number,1);
    }
  }
  cout<<"wrong_number is "<<wrong_number<<endl;
  bool *update = newA(bool, small_graph.n);
  bool *update1 = newA(bool, big_graph.n);
  parallel_for(uintV i =0; i< small_graph.n; i++){
    update[i] =0;
  }  
  parallel_for(uintV i =0; i< big_graph.n; i++){
    update1[i] =0;
  }
  edgeArray res1 = edgeArray(res, count, big_graph.n);
  edgeArray res2 = edgeArray(res, count, big_graph.n);
  edgeDeletionData del_tmp = edgeDeletionData(big_graph.n);
  edgeDeletionData del_tmp1 = edgeDeletionData(big_graph.n);
  del_tmp.updateWithEdgesArray(res1);
  del_tmp1.updateWithEdgesArray(res2);
  cout<<"we need to delet "<< del_tmp.numberOfDeletions<<" edges from small graph"<<endl;
  cout<<"we need to delet "<< del_tmp1.numberOfDeletions<<" edges from big graph"<<endl;
  cout<<"before delete small graph has "<<small_graph.m<< " edges"<<endl;  
  cout<<"before delete big graph has "<<big_graph.m<< " edges"<<endl; 
  small_graph.del_edges(del_tmp, update, false);
  big_graph.del_edges(del_tmp1, update1, false);
  cout<<"after delete small graph has "<<small_graph.m<< " edges"<<endl;  
  cout<<"after delete big graph has "<<big_graph.m<< " edges"<<endl;
}
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

  // edgeArray *path;

  DependencyData<VertexValueType> *dependency_data;
  DependencyData<VertexValueType> *dependency_data_old;
  
  DependencyData<VertexValueType> **all_level_dependency;
  struct GraphDependencyData{
  DependencyData<VertexValueType> *_dependency_data;
  uintV size;
  GraphDependencyData(): size(0){_dependency_data = nullptr;}
  GraphDependencyData(uintV _size):
    size(_size){
    _dependency_data = newA(DependencyData<VertexValueType>, _size);
  }
  void init(uintV _size){
    size = _size;
    _dependency_data = newA(DependencyData<VertexValueType>, _size);
  }
  void del(){
    // cout<<"single dependency delete"<<endl;
    deleteA(_dependency_data);
  }

  };

  GraphDependencyData graph_single;


  struct TreeDependency{
    int SnapshotNumber;
    GraphDependencyData **TreeDependencyData;
    TreeDependency():SnapshotNumber(0){TreeDependencyData=nullptr;}

    void init(uintV verticesNumber, int snapshots){
      SnapshotNumber = snapshots;
      TreeDependencyData = newA(GraphDependencyData*, SnapshotNumber);
      for (int i = 0; i < SnapshotNumber; i++)
      {
        TreeDependencyData[i] = newA(GraphDependencyData, i+1);
      }

      for(size_t k = 0; k < SnapshotNumber; k++){
        for(size_t j = 0; j < k+1; j++){
          TreeDependencyData[k][j].init(verticesNumber);
        }
      }
    }
    
    void del(){
      // TreeDependencyData = newA(GraphDependencyData*, SnapshotNumber);

      for(size_t i = 0; i < SnapshotNumber; i++){
        for(size_t j = 0; j < i+1; j++){
          // TreeDependencyData[i][j].init(verticesNumber);
          cout<<"in level "<<i<<" and index "<<j<<" we have deleted"<<endl;          
          TreeDependencyData[i][j].del();

        }
      }
    }      
    
    void tree_test(){
      for(size_t i = 0; i < SnapshotNumber; i++){
        for(size_t j = 0; j < i+1; j++){
          // TreeDependencyData[i][j].init(verticesNumber);
          // TreeDependencyData[i][j].del();
          cout<<"in level "<<i<<" and index "<<j<<" we have dependency with "<<
          TreeDependencyData[i][j].size<<" locations"<<endl;
        }
      }
    }
    };
  
  TreeDependency TreeVertexDependency;



  // TODO : Replace with more efficient vertexSubset using bitmaps
  bool *frontier;
  bool *all_affected_vertices;
  bool *changed;

  // graph<vertex> *multi;
  // graph<vertex>** snapshot;
  edgeArray* insertion_list;
  edgeArray* deletion_list;

  int level;
  edgeArray** insert_tree;
  edgeArray** delet_tree;
  BitsetScheduler active_vertices_bitset;

  // Stream Ingestor
  Ingestor<vertex> ingestor;
  int current_batch;

  graph <vertex> *path;

  GoldenTreeEngine(graph<vertex> &_my_graph, GlobalInfoType &_global_info,
                    commandLine _config)
      : my_graph(_my_graph), global_info(_global_info), global_info_old(),
        config(_config), ingestor(_my_graph, _config), current_batch(0),
        active_vertices_bitset(my_graph.n) {
    // &tmpGraph = new graph<vertex>;         
    n = my_graph.n;
    n_old = 0;
    // multi = multi_graph<vertex>(ingestor.numberOfSnapshots, my_graph.n);
    level = ingestor.numberOfSnapshots-2;
    

  }

  void init() {
    createDependencyData();
    createTemporaryStructures();
    createVertexSubsets();
    initVertexSubsets();
    initTemporaryStructures();
    initDependencyData();
    // initial_all_snapshot();
    // graph_single.init(my_graph.n);
    // vertices_dependency_construction();
    // TreeVertexDependency.init(my_graph.n ,ingestor.numberOfSnapshots);
    // initial_edgearray();
    // Tree.tree_test();
    // create_path_list();
    // edgeArray        

  }

  ~GoldenTreeEngine() {
    freeDependencyData();
    freeTemporaryStructures();
    freeVertexSubsets();
    // graph_single.del();
    // de_vertices_dependency_construction();
    global_info.cleanup();
    // free_snapshot();
    // free_edge_array();
    // TreTreeVertexDependencye.del();
    // del_path();
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
  void create_path_list(){
    path = newA(graph<vertex>, my_graph.whole_level);
    // cout<<"path init"<<endl;
  }
  void push_one_path(edgeArray &tmp ){
    my_graph.whole_level++;
    path = renewA(graph<vertex>, path, my_graph.whole_level);
    path[my_graph.whole_level-1] = graph_From_edges(tmp, my_graph);
  }
  void del_path(){
    for (size_t i = 0; i < my_graph.whole_level; i++)
    {
      path[i].del();
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
  


  void all_level_construct(){
    insert_tree = newA(edgeArray*, level+1);
    delet_tree = newA(edgeArray*, level+1);
    for (size_t i = 0; i < level+1; i++)
    {
      delet_tree[i] = newA(edgeArray, i+1);
      insert_tree[i] = newA(edgeArray, i+1);
    }
    parallel_for (size_t i = 0; i < level+1; i++)
    {
      insert_tree[level][i] = insertion_list[i];
      delet_tree[level][i] = deletion_list[i];

    }
    int count = level-1;
    while (count>=0)
    {
      parallel_for (size_t i = 0; i < count+1; i++)
      {
        insert_tree[count][i] = insert_tree[count+1][i];
        delet_tree[count][i] = delet_tree[count+1][i+1];
        
      }
      count--;
    }

    
    
    
  } 

  void all_level_deconstruct(){
    for (size_t i = 0; i < level; i++)
    {
      for (size_t j = 0; j < i+1; j++)
      {
        insert_tree[i][j].del();
        delet_tree[i][j].del();
      }
      
    }
    
  }

  // void newLevelConstruction(edgeArray** insertion, edgeArray** deletion, int level,
  // edgeArray** new_level){
  //   for (int i = 0; i < 2*(level-1); i=i+2)
  //   {
  //     new_level[i] = insertion[(i/2)];
  //     new_level[i+1] = deletion[(i/2)+1];
  //   }
  // }

  void count_for_test(){
    for (size_t i = 0; i < ingestor.numberOfSnapshots-1; i++)
    {
      cout<<"from snapshot "<<i<<" to "<<"snapshot "<<i+1<<" we delete "<<deletion_list[i].size<<" edges"<<endl;
      cout<<"from snapshot "<<i<<" to "<<"snapshot "<<i+1<<" we insert "<<insertion_list[i].size<<" edges"<<endl;

    }
    
  }

  void initial_edgearray(){
    // insertion_list = new edgeArray*[ingestor.numberOfSnapshots-1];
    // deletion_list = new edgeArray*[ingestor.numberOfSnapshots-1];
    insertion_list = newA(edgeArray, ingestor.numberOfSnapshots-1);
    deletion_list = newA(edgeArray, ingestor.numberOfSnapshots-1);
    parallel_for (size_t i = 0; i < ingestor.numberOfSnapshots-1; i++)
    {
      // insertion_list[i] =  new edgeArray();
      insertion_list[i] = my_graph.random_bacth_insert(5*(i+1));
      // deletion_list[i] = new edgeArray();
      // deletion_list[i] = new edgeArray();
      deletion_list[i] =  my_graph.random_bacth_sample(i+1);
    }
    
    cout<<"edgeArray construct done"<<endl;


  }



  void free_edge_array(){
    for (size_t i = 0; i < ingestor.numberOfSnapshots; i++)
    {
      insertion_list[i].del();
      deletion_list[i].del();
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

  void printOutput(string output_file_path) {
    // string output_file_path = config.getOptionValue("-outputFile", "/tmp/");
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
    // printOutput();
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
    // cout << "iterations: " << iterations << endl;


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
    // n_old = n;
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
      // dependency_data_old[v] = dependency_data[v];
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
    // printOutput();    

  }
  
  void mutation_free_one_edge_list_generation(uintE number_of_insertion){
    my_graph.push_one_edge_array(number_of_insertion);
    edgeArray tmp_insert = my_graph.insertion_array[my_graph.whole_level-1];
    push_one_path(tmp_insert);
    // cout<<"we push "<<path[my_graph.whole_level-1].m<<" to engine"<<endl;
    tmp_insert.del();
  }
  void mutation_free_one_edge_list_push(edgeArray &tmp_array){
    // my_graph.push_one_edge_array(number_of_insertion);
    // edgeArray tmp_insert = my_graph.insertion_array[my_graph.whole_level-1];
    push_one_path(tmp_array);
    // cout<<"we push "<<path[my_graph.whole_level-1].m<<" to engine"<<endl;
    // cout<<"now we have "<<my_graph.whole_level<<" edgelist in system"<<endl;
    // tmp_insert.del();
  }  

  uintE GetOutDegree_for_one_vertex(uintV src){
    uintE outdegree = 0;
    outdegree += my_graph.V[src].getOutDegree();
    for (size_t i = 0; i < my_graph.whole_level; i++)
    {
      outdegree += path[i].V[src].getOutDegree();
    }
    return outdegree;

  }

  void untraditionalIncrementalComputation(){
    timer incre_timer;
    // long iterations = 0;
    // cout<<"in iteration "<<iterations<<" we have "<<active_vertices_bitset.numTasks()<<" tasks"<<endl;   
    // incre_timer.start();
    // cout<<"*****************"<<endl;
    // incre_timer.start();
    while (active_vertices_bitset.anyScheduledTasks()) {
      // uintV tmp = active_vertices_bitset.numTasks();
      // cout<<"in iteration "<<iterations<<" we have "<<active_vertices_bitset.numTasks()<<" tasks"<<endl;     
      active_vertices_bitset.newIteration();
      // incre_timer.start(); 
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

      parallel_for(int i=0; i< my_graph.whole_level; i++){
        intE outde = path[i].V[u].getOutDegree();
        parallel_for(uintV k = 0; k< outde; k++){
          uintV dest = path[i].V[u].getOutNeighbor(k);
#ifdef EDGEDATA
          EdgeData *edge_data = path[i].V[u].getOutEdgeData(k);
#else
          EdgeData *edge_data = &emptyEdgeData;          
#endif    
          bool ret = reduce(u, dest, *edge_data, dependency_data[u], dependency_data[dest],
              global_info);
          if (ret)
          { 
            active_vertices_bitset.schedule(dest);
            // cout<<"push new event!"<<endl;
          }
          
        }
      }
        }
      }
    }
  }
  
  void mutation_free_one_edge_list_computation(edgeArray &edge_add){
    timer full_time;
    // Handle newly added vertices
    n_old = n;
    if (edge_add.maxVertex >= n) {
      processVertexAddition(edge_add.maxVertex);
    }    
  
    // Reset values before incremental computation
    // active_vertices_bitset.reset();
    parallel_for(uintV v = 0; v < n; v++) {
      frontier[v] = 0;
      all_affected_vertices[v] = 0;
    }
    // ======================================================================
    // PHASE 1 - Process additions
    // ======================================================================
    // cout<<"the batch we are processing contains "<<edge_add.size<<" number of edges"<<endl;
    // full_time.start();
    parallel_for(long i = 0; i < edge_add.size ; i++) {
      uintV source = edge_add.E[i].source;
      uintV destination = edge_add.E[i].destination;
#ifdef EDGEDATA
      EdgeData *edge_data = edge_add.E[i].edgeData;
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
    // cout<<"vertex activate time "<<full_time.stop()<<endl;

  //   uintV test = 0;
  //   parallel_for(uintV v = 0; v < n; v++) {
  //     if (all_affected_vertices[v] == 1) {
  //       pbbs::fetch_and_add(&test, 1);
  //     }
  //   }    
  // cout<<"number of affected nodes are "<<test<<endl;

    // ======================================================================
    // PHASE 2 - Traditional processing
    // ======================================================================
    // For all affected vertices, start traditional processing
    // full_time.start();
    active_vertices_bitset.reset();
    parallel_for(uintV v = 0; v < n; v++) {
      if (all_affected_vertices[v] == 1) {
        active_vertices_bitset.schedule(v);
      }
    }
    // cout<<"bitscheduler set time "<<full_time.stop()<<endl;
    // full_time.start();
    untraditionalIncrementalComputation();
    // cout<<"incremental computation time "<<full_time.stop()<<endl;
    // cout<<endl;
  }
  void runtime_test(){
    edgeArray* insertion_array;
    edgeArray* deletion_array;


  }
  void test_run() {
    initialCompute();
    timer full_time;
    double time_count;
    for (size_t i = 0; i < ingestor.numberOfSnapshots; i++)
    {
      parallel_for(uintV i = 0; i < n; i++) ingestor.updated_vertices[i] = 0;
      uintV number = 70000;
      cout<<"the number here is "<<number<<endl;
      ingestor.edge_additions = my_graph.random_bacth_insert(number);
      my_graph.add_edgess(ingestor.edge_additions, ingestor.updated_vertices);
      full_time.start();
      deltaCompute(ingestor.edge_additions, ingestor.edge_deletions);
      time_count += full_time.stop();
      cout<<"time = "<<time_count<<endl;
    }
    
  }
  void deendency_copy_new_to_old(){
  parallel_for (size_t i = 0; i < n; i++)
  {
    dependency_data_old[i].parent = dependency_data[i].parent;
    dependency_data_old[i].value = dependency_data[i].value;
    dependency_data_old[i].level = dependency_data[i].level;
  }    
  }
  void dependency_copy_old_to_new(){
  parallel_for (size_t i = 0; i < n; i++)
  {
    dependency_data[i].parent = dependency_data_old[i].parent;
    dependency_data[i].value = dependency_data_old[i].value;
    dependency_data[i].level = dependency_data_old[i].level;
  }     
  }
  void golden_test(uintV insert_number){
  // parallel_for (size_t i = 0; i < n; i++)
  // {
  //   dependency_data_old[i].parent = dependency_data[i].parent;
  //   dependency_data_old[i].value = dependency_data[i].value;
  //   dependency_data_old[i].level = dependency_data[i].level;
  // }
  
  // cout<<ingestor.numberOfSnapshots<<endl;
  insert_number *= 1;
  // cout<<"finish initial compute"<<endl;
  timer full_time;
  double time_count;
  deendency_copy_new_to_old();//always hold this copy  

  for (size_t i = 0; i < ingestor.numberOfSnapshots; i++)
  {
    create_path_list();
    dependency_copy_old_to_new();
    edgeArray tmp_insert = my_graph.random_bacth_insert_seed(insert_number, time(NULL));
    mutation_free_one_edge_list_push(tmp_insert);
    full_time.start();
    mutation_free_one_edge_list_computation(tmp_insert);
    time_count += full_time.stop();
    // cout<<"time = "<<time_count<<endl<<endl;    
    del_path();
    my_graph.insertion_array = nullptr;
    my_graph.whole_level = 0;
  }
   
    cout<<"time = "<<time_count<<endl<<endl;    
  //   for (size_t i = 1; i < ingestor.numberOfSnapshots; i++)
  // {
  //   create_path_list();
  //   dependency_copy_old_to_new();
  //   edgeArray tmp_insert = my_graph.random_bacth_insert_seed(insert_number*i, i+1);
  //   mutation_free_one_edge_list_push(tmp_insert);
  //   full_time.start();
  //   mutation_free_one_edge_list_computation(tmp_insert);
  //   time_count += full_time.stop();
  //   // cout<<"time = "<<time_count<<endl<<endl;    
  //   del_path();
  //   my_graph.insertion_array = nullptr;
  //   my_graph.whole_level = 0;
  // }
  //   cout<<"whole time = "<<time_count<<endl<<endl;    

  //   time_count = 0;
  //   cout<<"direct compute start"<<endl;
  // for (size_t i = 0; i < ingestor.numberOfSnapshots; i++)
  // {
  //   dependency_copy_old_to_new();
  //   create_path_list();
  //   edgeArray tmp_insert = my_graph.random_bacth_insert_seed(insert_number*ingestor.numberOfSnapshots, i+1);
  //   mutation_free_one_edge_list_push(tmp_insert);
  //   full_time.start();
  //   mutation_free_one_edge_list_computation(tmp_insert);
  //   time_count += full_time.stop();
  //   // cout<<"time = "<<time_count<<endl<<endl;    
  //   del_path();
  //   my_graph.insertion_array = nullptr;
  //   my_graph.whole_level = 0;
  // }
  //   cout<<"direct time = "<<time_count<<endl<<endl;    
  //   cout<<"average time for "<<insert_number*ingestor.numberOfSnapshots<< " direct insertion is "<<time_count/ingestor.numberOfSnapshots<<endl;
    

  }
  // void unit_test(uintE insert_number){
  //   timer full_time;
  //   double time_count;
  //   edgeArray tmp_insert = my_graph.random_bacth_insert(insert_number);
  //   mutation_free_one_edge_list_push(tmp_insert);
  //   full_time.start();
  //   mutation_free_one_edge_list_computation(tmp_insert);
  //   time_count += full_time.stop(); 
  //   cout<<insert_number<<" takes "<<time_count<<endl;   
  // }
};

#endif
