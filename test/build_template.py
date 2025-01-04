import os

template = '''#include "../unit_test.hpp"

bool UNIT_TEST_Sort_topo::sort_topo{index:02}() {{
  string name = "sort_topo{index:02}";
  //! data ------------------------------------

  //! expect ----------------------------------
  string expect = "";

  //! output ----------------------------------
  stringstream output;

  //! remove data -----------------------------

  //! result ----------------------------------
  return printResult(output.str(), expect, name);
}}
'''

# Generate files from sort_topo01.cpp to sort_topo20.cpp
for i in range(22 ,26):
    filename = f"unit_test/sort_topo/test/sort_topo{i:02}.cpp"
    if not os.path.exists(filename):
        with open(filename, 'w') as file:
            content = template.format(index=i)
            file.write(content)
        print(f"Created: {filename}")
    else:
        print(f"Skipped (already exists): {filename}")