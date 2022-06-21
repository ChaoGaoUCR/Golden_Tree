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

#include "../core/common/utils.h"
#include "../core/graphBolt/KickStarterEngine.h"
#include "../core/main.h"
#include <math.h>

#define MAX_PARENT 4294967295

// ======================================================================
// WCCINFO
// ======================================================================
class WccInfo {
public:
  uintV source_vertex;

  WccInfo() : source_vertex(0) {}

  WccInfo(uintV _source_vertex) : source_vertex(_source_vertex) {}

  void copy(const WccInfo &object) { source_vertex = object.source_vertex; }

  void processUpdates(edgeArray &edge_additions, edgeArray &edge_deletions) {}

  void cleanup() {}

  void init() {}
};

// ======================================================================
// VERTEXVALUE INITIALIZATION
// ======================================================================
template <class VertexValueType, class GlobalInfoType>
inline void initializeVertexValue(const uintV &v,
                                  VertexValueType &v_vertex_value,
                                  const GlobalInfoType &global_info) {
  // if (v != global_info.source_vertex) {
  //   v_vertex_value = 0;
  // } else {
  //   v_vertex_value = 1;
  // }
  v_vertex_value = v;
}

// ======================================================================
// ACTIVATE VERTEX FOR FIRST ITERATION
// ======================================================================
template <class GlobalInfoType>
inline bool frontierVertex(const uintV &v, const GlobalInfoType &global_info) {
  if (v == global_info.source_vertex) {
    return true;
  } else {
    return false;
  }
}

// ======================================================================
// EDGE FUNCTION
// ======================================================================
template <class VertexValueType, class EdgeDataType, class GlobalInfoType>
inline bool
edgeFunction(const uintV &u, const uintV &v, const EdgeDataType &edge_weight,
             const VertexValueType &u_value, VertexValueType &v_value,
             GlobalInfoType &global_info) {
  // if (u_value == 0) {
  //   return false;
  // } else {
  //   v_value = 1;
  //   return true;
  // }
  if (u_value == v_value)
  {
    return false;
  }
  else{
    v_value = u_value;
    return true;
  }
  
}

// ======================================================================
// SHOULDPROPAGATE
// ======================================================================
// shouldPropagate condition for deciding if the value change in
// updated graph violates monotonicity
template <class VertexValueType, class GlobalInfoType>
inline bool shouldPropagate(const VertexValueType &old_value,
                            const VertexValueType &new_value,
                            GlobalInfoType &global_info) {
  // return (old_value == 1) && (new_value == 0);
  return (old_value!=new_value);
}

// ======================================================================
// HELPER FUNCTIONS
// ======================================================================
template <class GlobalInfoType>
void printAdditionalData(ofstream &output_file, const uintV &v,
                         GlobalInfoType &info) {}

// ======================================================================
// COMPUTE FUNCTION
// ======================================================================
template <class vertex> void compute(graph<vertex> &G, commandLine config) {
  double whole_time = 0;
  // for (size_t i = 0; i < 5; i++)
  // {
  long n = G.n;
  int source_vertex = config.getOptionLongValue("-source", 0);
  WccInfo global_info(source_vertex);

  // cout << "Initializing engine ....\n";
  KickStarterEngine<vertex, uint16_t, WccInfo> engine(G, global_info, config);
  engine.init();
  // cout << "Finished initializing engine\n";
  // engine.run();
  uintE number = engine.ingestor.change_edge_number;
    // uintE number;
    // cout<<"enter the number for basic hop: ";
    // cin>>number;  
  whole_time += engine.test_run(number);
  // ~engine();
  // engine.del_KickStarterEngine();
  // }
  // cout<<whole_time/5<<endl;

 
}
