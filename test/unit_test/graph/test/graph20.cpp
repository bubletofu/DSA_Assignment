#include "../unit_test.hpp"

bool UNIT_TEST_Graph::graph20() {
  string name = "graph20";
  //! data ------------------------------------
  UGraphModel<char> model(&charComparator, &vertex2str);
  char vertices[] = {'A', 'B', 'C', 'D'};
  for (int idx = 0; idx < 4; idx++) {
    model.add(vertices[idx]);
  }
  model.connect('A', 'B');
  model.disconnect('A', 'B');
  model.connect('A', 'B');
  model.disconnect('A', 'B');
  model.connect('A', 'B');
  model.disconnect('A', 'B');

  //! expect ----------------------------------
  string expect =
      "==================================================\n\
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
  output << model.toString();

  //! remove data -----------------------------
  model.clear();

  //! result ----------------------------------
  return printResult(output.str(), expect, name);
}
