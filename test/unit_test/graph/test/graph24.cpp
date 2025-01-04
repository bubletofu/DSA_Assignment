#include "../unit_test.hpp"

bool UNIT_TEST_Graph::graph24() {
  string name = "graph24";
  //! data ------------------------------------
  DGraphModel<char> model(&charComparator, &vertex2str);
  char vertices[] = {'A', 'B', 'C', 'D'};
  for (int idx = 0; idx < 4; idx++) {
    model.add(vertices[idx]);
  }
  model.connect('A', 'B', 5);
  model.connect('A', 'C', 1);
  DLinkedList<char> outwardEdgesA = model.getOutwardEdges('A');
  DLinkedList<char> outwardEdgesB = model.getOutwardEdges('B');

  //! expect ----------------------------------
  string expect =
      "outwardEdgesA: B C \n\
outwardEdgesB: \n\
Error: Vertex (X): is not found\n\
==================================================\n\
Vertices:   \n\
V(A, in: 0, out: 2)\n\
V(B, in: 1, out: 0)\n\
V(C, in: 1, out: 0)\n\
V(D, in: 0, out: 0)\n\
------------------------------\n\
Edges:      \n\
E(A,B,5)\n\
E(A,C,1)\n\
==================================================\n";

  //! output ----------------------------------
  stringstream output;
  output << "outwardEdgesA: ";
  for (auto v : outwardEdgesA) {
    output << v << " ";
  }
  output << endl;

  output << "outwardEdgesB: ";
  for (auto v : outwardEdgesB) {
    output << v << " ";
  }
  output << endl;

  try {
    DLinkedList<char> outwardEdgesX = model.getOutwardEdges('X');
  } catch (const exception& e) {
    output << "Error: " << string(e.what()) << endl;
  }

  output << model.toString();

  //! remove data -----------------------------
  model.clear();

  //! result ----------------------------------
  return printResult(output.str(), expect, name);
}
