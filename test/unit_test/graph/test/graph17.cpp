#include "../unit_test.hpp"

bool UNIT_TEST_Graph::graph17() {
  string name = "graph17";
  //! data ------------------------------------
  UGraphModel<char> model(&charComparator, &vertex2str);

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
