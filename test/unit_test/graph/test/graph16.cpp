#include "../unit_test.hpp"

bool UNIT_TEST_Graph::graph16() {
  string name = "graph16";
  //! data ------------------------------------
  DGraphModel<char> model(&charComparator, &vertex2str);

  //! expect ----------------------------------
  string expect =
      "==================================================\n\
Vertices:   \n\
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
