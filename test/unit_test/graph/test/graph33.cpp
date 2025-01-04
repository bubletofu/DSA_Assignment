#include "../unit_test.hpp"

bool UNIT_TEST_Graph::graph33() {
  string name = "graph33";
  //! data ------------------------------------
  UGraphModel<char> model(&charComparator, &vertex2str);
  char vertices[] = {'A', 'B', 'C', 'D'};
  for (int idx = 0; idx < 4; idx++) {
    model.add(vertices[idx]);
  }
  model.connect('A', 'B');
  model.connect('A', 'C');
  model.connect('A', 'D');
  model.remove('B');
  model.remove('A');

  //! expect ----------------------------------
  string expect =
      "Error: Vertex (X): is not found\n\
==================================================\n\
Vertices:   \n\
V(C, in: 0, out: 0)\n\
V(D, in: 0, out: 0)\n\
------------------------------\n\
Edges:      \n\
==================================================\n";

  //! output ----------------------------------
  stringstream output;

  try {
    model.remove('X');
  } catch (const exception& e) {
    output << "Error: " << string(e.what()) << endl;
  }

  output << model.toString();

  //! remove data -----------------------------
  model.clear();

  //! result ----------------------------------
  return printResult(output.str(), expect, name);
}
