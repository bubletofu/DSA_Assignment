#include "../unit_test.hpp"

bool UNIT_TEST_Graph::graph26() {
  string name = "graph26";
  //! data ------------------------------------
  DGraphModel<char> model(&charComparator, &vertex2str);
  char vertices[] = {'A', 'B', 'C', 'D'};
  for (int idx = 0; idx < 4; idx++) {
    model.add(vertices[idx]);
  }
  model.connect('A', 'B', 5);
  model.connect('A', 'C', 1);

  //! expect ----------------------------------
  string expect =
      "0 1\n\
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
  output << model.inDegree('A') << " " << model.inDegree('B') << endl;
  try {
    model.inDegree('X');
  } catch (const exception& e) {
    output << "Error: " << string(e.what()) << endl;
  }

  output << model.toString();

  //! remove data -----------------------------
  model.clear();

  //! result ----------------------------------
  return printResult(output.str(), expect, name);
}
