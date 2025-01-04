#include "../unit_test.hpp"

bool UNIT_TEST_Graph::graph31() {
  string name = "graph31";
  //! data ------------------------------------
  DGraphModel<char> model(&charComparator, &vertex2str);
  char vertices[] = {'A', 'B', 'C', 'D'};
  for (int idx = 0; idx < 4; idx++) {
    model.add(vertices[idx]);
  }
  model.connect('A', 'B', 5);
  model.disconnect('A', 'B');
  //! expect ----------------------------------
  string expect =
      "Error: Vertex (X): is not found\n\
Error: Vertex (Y): is not found\n\
Error: Edge (E(A,B)): is not found\n\
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

  try {
    model.disconnect('A', 'X');
  } catch (const exception& e) {
    output << "Error: " << string(e.what()) << endl;
  }

  try {
    model.disconnect('Y', 'A');
  } catch (const exception& e) {
    output << "Error: " << string(e.what()) << endl;
  }

  try {
    model.disconnect('A', 'B');
  } catch (const exception& e) {
    output << "Error: " << string(e.what()) << endl;
  }

  output << model.toString();

  //! remove data -----------------------------
  model.clear();

  //! result ----------------------------------
  return printResult(output.str(), expect, name);
}
