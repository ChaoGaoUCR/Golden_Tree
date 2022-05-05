#ifndef Tree_INGESTOR_H
#define Tree_INGESTOR_H

#include "../common/parseCommandLine.h"
#include "../common/utils.h"
#include "../graph/IO.h"
#include "../graph/graph.h"
#include <string>

/**
 * Used to extract values from the binary stream
 **/
struct StreamEdge
{
  uintV source;
  uintV destination;
  char edgeType;
};

template <class vertex> 
graph<vertex> graph_From_edges(edgeArray &edge_additions, graph <vertex> &mygraph){
    // need mygraph for max vertex reading
      vertex *v = newA(vertex, 0);
      unsigned long n = 0;
      unsigned long m = 0;
      uintV *edges = newA(uintV, m);
      uintV *inEdges = newA(uintV, m);
      intE *offsets = newA(intE, n);
      intE *toffsets = newA(intE, n);
      AdjacencyRep<vertex> *mem = new AdjacencyRep<vertex>(v, n, m, edges, inEdges, offsets, toffsets);
      bool *updated_vertices;      

      updated_vertices = newA(bool, edge_additions.maxVertex);
      graph <vertex> tmp_graph = graph<vertex>(v, n, m, mem);
      tmp_graph.addVertices(mygraph.n);
      for (uintV i = 0; i < mygraph.n; i++) updated_vertices[i] = 0;      
      tmp_graph.addEdges(edge_additions, updated_vertices);
      // cout<<tmp_graph.m<<endl;
      return tmp_graph;          
}

template <class vertex>
class tree_ingestor
{
// private:
//     /* data */
public:
    commandLine config;
    graph<vertex> &my_graph;
    long n;
    long numberOfSnapshots;
    bool typeOfTree; //0 is binary and 1 is normal tree
    float portionOfModification;    


    tree_ingestor(/* args */);
    ~tree_ingestor();
};
