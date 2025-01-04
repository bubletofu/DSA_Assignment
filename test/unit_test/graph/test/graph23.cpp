#include "../unit_test.hpp"

bool UNIT_TEST_Graph::graph23() {
  string name = "graph23";
  //! data ------------------------------------
  UGraphModel<char> model(&charComparator, &vertex2str);
  char vertices[] = {'A', 'B', 'C', 'D'};
  for (int idx = 0; idx < 4; idx++) {
    model.add(vertices[idx]);
  }
  model.connect('A', 'B', 5);

  //! expect ----------------------------------
  string expect =
      "5\n\
Error: Vertex (X): is not found\n\
Error: Vertex (Y): is not found\n\
Error: Edge (E(A,C)): is not found\n\
==================================================\n\
Vertices:   \n\
V(A, in: 1, out: 1)\n\
V(B, in: 1, out: 1)\n\
V(C, in: 0, out: 0)\n\
V(D, in: 0, out: 0)\n\
------------------------------\n\
Edges:      \n\
E(A,B,5)\n\
E(B,A,5)\n\
==================================================\n";

  //! output ----------------------------------
  stringstream output;
  output << model.weight('A', 'B') << endl;

  try {
    model.weight('A', 'X');
  } catch (const exception& e) {
    output << "Error: " << string(e.what()) << endl;
  }

  try {
    model.weight('Y', 'A');
  } catch (const exception& e) {
    output << "Error: " << string(e.what()) << endl;
  }

  try {
    model.weight('A', 'C');
  } catch (const exception& e) {
    output << "Error: " << string(e.what()) << endl;
  }

  output << model.toString();

  //! remove data -----------------------------
  model.clear();

  //! result ----------------------------------
  return printResult(output.str(), expect, name);
}
