#include "../unit_test.hpp"

bool UNIT_TEST_Graph::graph29() {
  string name = "graph29";
  //! data ------------------------------------
  DGraphModel<char> model(&charComparator, &vertex2str);
  char vertices[] = {'A', 'B', 'C', 'D'};
  for (int idx = 0; idx < 4; idx++) {
    model.add(vertices[idx]);
  }
  model.connect('A', 'B');
  model.connect('A', 'B', 5);
  //! expect ----------------------------------
  string expect =
      "Error: Vertex (X): is not found\n\
Error: Vertex (Y): is not found\n\
==================================================\n\
Vertices:   \n\
V(A, in: 0, out: 1)\n\
V(B, in: 1, out: 0)\n\
V(C, in: 0, out: 0)\n\
V(D, in: 0, out: 0)\n\
------------------------------\n\
Edges:      \n\
E(A,B,5)\n\
==================================================\n";

  //! output ----------------------------------
  stringstream output;

  try {
    model.connect('A', 'X');
  } catch (const exception& e) {
    output << "Error: " << string(e.what()) << endl;
  }

  try {
    model.connect('Y', 'A');
  } catch (const exception& e) {
    output << "Error: " << string(e.what()) << endl;
  }

  output << model.toString();

  //! remove data -----------------------------
  model.clear();

  //! result ----------------------------------
  return printResult(output.str(), expect, name);
}
