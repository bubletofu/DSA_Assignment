#include "../unit_test.hpp"

bool UNIT_TEST_Graph::graph22() {
  string name = "graph22";
  //! data ------------------------------------
  UGraphModel<char> model(&charComparator, &vertex2str);
  char vertices[] = {'A', 'B', 'C', 'D'};
  for (int idx = 0; idx < 4; idx++) {
    model.add(vertices[idx]);
  }

  //! expect ----------------------------------
  string expect =
      "1 0\n\
==================================================\n\
Vertices:   \n\
V(A, in: 0, out: 0)\n\
V(B, in: 0, out: 0)\n\
V(C, in: 0, out: 0)\n\
V(D, in: 0, out: 0)\n\
------------------------------\n\
Edges:      \n\
==================================================\n";

  //! output ----------------------------------
  stringstream output;
  output << model.contains('A') << " ";
  output << model.contains('E') << endl;
  output << model.toString();

  //! remove data -----------------------------
  model.clear();

  //! result ----------------------------------
  return printResult(output.str(), expect, name);
}
