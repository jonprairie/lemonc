#include <iostream>
#include <cstdlib>

#include <lemon/list_graph.h>
#include <lemon/lgf_reader.h>
#include <lemon/lgf_writer.h>
#include <lemon/matching.h>

#include "lemonc.h"

#ifdef __cplusplus
extern "C" {
#endif

using namespace std;
using namespace lemon;

int main() {
  int test = 5;
  int *plist1 = &test;
  int *plist2 = &test;
  int *length = &test;

  cout << "initialized variables" << endl;

  getMatching(&plist1, &plist2, length);

  cout << "finished matching" << endl;

  for(int i = 0; i < *length; ++i) {
    cout << plist1[i] << " vs " << plist2[i] << endl;
  }

  cout << "finished program" << endl;
    
  return 0;
}

void getMatching(int **plist1, int **plist2, int *length) {

  cout << "called getMatching" << endl;

  ListGraph g;
  ListGraph::EdgeMap<double> weights(g);

  graphReader(g, "test_graph.lgf")
    .edgeMap("weights", weights)
    .run();

  cout << "read graph" << endl;

  MaxWeightedMatching<ListGraph, ListGraph::EdgeMap<double>> mwm(g, weights);
  mwm.run();

  cout << "matching size: " << mwm.matchingSize() << endl;
  cout << "matching weight: " << mwm.matchingWeight() << endl;

  cout << "ran matching" << endl;

  int num_nodes = 0;
  for(ListGraph::NodeIt nd(g); nd != INVALID; ++nd) {
    ++num_nodes;
  }

  cout << "counted nodes: " << num_nodes << endl;

  *length = (num_nodes + 1) / 2; //round up

  cout << "calculated pairings length: " << *length << endl;

  int num_edges = 0;
  for(ListGraph::EdgeIt e(g); e != INVALID; ++e) {
    ++num_edges;
  }

  cout << "counted edges: " << num_edges << endl;

  *plist1 = (int*)malloc( sizeof(int) * *length );
  *plist2 = (int*)malloc( sizeof(int) * *length );
  int *used = (int*)malloc( sizeof(int) * num_nodes );

  cout << "allocated arrays" << endl;

  for(int i = 0; i < *length; ++i) {
    (*plist1)[i] = -1;
    (*plist2)[i] = -1;
  }

  for(int i = 0; i < num_nodes; ++i) {
    used[i] = 0;
  }

  printArray("plist1", *plist1, *length);
  printArray("plist2", *plist2, *length);
  printArray("used", used, num_nodes);

  cout << "initialized arrays" << endl;

  ListGraph::NodeIt nd(g);
  for(int i = 0; i < *length && nd != INVALID; ++nd) {
    int node_id = g.id(nd);

    if(!used[node_id]) {
      used[node_id] = 1;
      (*plist1)[i] = node_id;
      
      ListGraph::Node mate = mwm.mate(nd);
      cout << "mate id: " << g.id(mate) << endl;

      if(mate != INVALID) {
	int mate_id = g.id(mate);
	used[mate_id] = 1;
	(*plist2)[i] = mate_id;
      }
      ++i;
    }
  }

  printArray("plist1", *plist1, *length);
  printArray("plist2", *plist2, *length);
  printArray("used", used, num_nodes);

  cout << "made pairings" << endl;

  free(used);
}

void printArray(char *name, int *a1, int l) {
  cout << name << ":" << endl;
  for(int i = 0; i < l; ++i) {
    cout << " " << i << " = " << a1[i] << endl;
  }
}

#ifdef __cplusplus
}
#endif
