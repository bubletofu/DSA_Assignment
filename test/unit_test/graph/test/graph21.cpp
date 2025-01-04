#include "../unit_test.hpp"

bool UNIT_TEST_Graph::graph21() {
  string name = "graph21";
  //! data ------------------------------------
  UGraphModel<char> model(&charComparator, &vertex2str);
  char vertices[] = {'A', 'B', 'C', 'D'};
  for (int idx = 0; idx < 4; idx++) {
    model.add(vertices[idx]);
  }
  model.connect('A', 'B');
  model.add('A');
  model.add('B');

  //! expect ----------------------------------
  string expect =
      "==================================================\n\
Vertices:   \n\
V(A, in: 1, out: 1)\n\
V(B, in: 1, out: 1)\n\
V(C, in: 0, out: 0)\n\
V(D, in: 0, out: 0)\n\
------------------------------\n\
Edges:      \n\
E(A,B,0)\n\
E(B,A,0)\n\
==================================================\n";

  //! output ----------------------------------
  stringstream output;
  output << model.toString();

  //! remove data -----------------------------
  model.clear();

  //! result ----------------------------------
  return printResult(output.str(), expect, name);
}
