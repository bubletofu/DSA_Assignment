#include "../unit_test.hpp"

bool UNIT_TEST_Graph::graph28() {
  string name = "graph28";
  //! data ------------------------------------
  UGraphModel<char> model(&charComparator, &vertex2str);
  char vertices[] = {'A', 'B', 'C', 'D'};
  for (int idx = 0; idx < 4; idx++) {
    model.add(vertices[idx]);
  }
  model.connect('A', 'B', 5);

  //! expect ----------------------------------
  string expect =
      "1 0\n\
Error: Vertex (X): is not found\n\
Error: Vertex (Y): is not found\n\
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
  output << model.connected('A', 'B') << " " << model.connected('A', 'C')
         << endl;

  try {
    model.connected('A', 'X');
  } catch (const exception& e) {
    output << "Error: " << string(e.what()) << endl;
  }

  try {
    model.connected('Y', 'A');
  } catch (const exception& e) {
    output << "Error: " << string(e.what()) << endl;
  }

  output << model.toString();

  //! remove data -----------------------------
  model.clear();

  //! result ----------------------------------
  return printResult(output.str(), expect, name);
}
