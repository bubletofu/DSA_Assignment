#ifndef UNIT_TEST_Graph_HPP
#define UNIT_TEST_Graph_HPP

#include "graph/DGraphModel.h"
#include "graph/UGraphModel.h"
#include "library.hpp"

bool charComparator(char &lhs, char &rhs);
string vertex2str(char &v);

class UNIT_TEST_Graph {
 public:
  UNIT_TEST_Graph() {
    // TODO unit test new
    registerTest("graph01", &UNIT_TEST_Graph::graph01);
    registerTest("graph02", &UNIT_TEST_Graph::graph02);
    registerTest("graph03", &UNIT_TEST_Graph::graph03);
    registerTest("graph04", &UNIT_TEST_Graph::graph04);
    registerTest("graph05", &UNIT_TEST_Graph::graph05);
    registerTest("graph06", &UNIT_TEST_Graph::graph06);
    registerTest("graph07", &UNIT_TEST_Graph::graph07);
    registerTest("graph08", &UNIT_TEST_Graph::graph08);
    registerTest("graph09", &UNIT_TEST_Graph::graph09);
    registerTest("graph10", &UNIT_TEST_Graph::graph10);
    registerTest("graph11", &UNIT_TEST_Graph::graph11);
    registerTest("graph12", &UNIT_TEST_Graph::graph12);
    registerTest("graph13", &UNIT_TEST_Graph::graph13);
    registerTest("graph14", &UNIT_TEST_Graph::graph14);
    registerTest("graph15", &UNIT_TEST_Graph::graph15);
    registerTest("graph16", &UNIT_TEST_Graph::graph16);
    registerTest("graph17", &UNIT_TEST_Graph::graph17);
    registerTest("graph18", &UNIT_TEST_Graph::graph18);
    registerTest("graph19", &UNIT_TEST_Graph::graph19);
    registerTest("graph20", &UNIT_TEST_Graph::graph20);
    registerTest("graph21", &UNIT_TEST_Graph::graph21);
    registerTest("graph22", &UNIT_TEST_Graph::graph22);
    registerTest("graph23", &UNIT_TEST_Graph::graph23);
    registerTest("graph24", &UNIT_TEST_Graph::graph24);
    registerTest("graph25", &UNIT_TEST_Graph::graph25);
    registerTest("graph26", &UNIT_TEST_Graph::graph26);
    registerTest("graph27", &UNIT_TEST_Graph::graph27);
    registerTest("graph28", &UNIT_TEST_Graph::graph28);
    registerTest("graph29", &UNIT_TEST_Graph::graph29);
    registerTest("graph30", &UNIT_TEST_Graph::graph30);
    registerTest("graph31", &UNIT_TEST_Graph::graph31);
    registerTest("graph32", &UNIT_TEST_Graph::graph32);
    registerTest("graph33", &UNIT_TEST_Graph::graph33);
    registerTest("graph34", &UNIT_TEST_Graph::graph34);
  }

 private:
  // TODO unit test new
  bool graph01();
  bool graph02();
  bool graph03();
  bool graph04();
  bool graph05();
  bool graph06();
  bool graph07();
  bool graph08();
  bool graph09();
  bool graph10();
  bool graph11();
  bool graph12();
  bool graph13();
  bool graph14();
  bool graph15();
  bool graph16();
  bool graph17();
  bool graph18();
  bool graph19();
  bool graph20();
  bool graph21();
  bool graph22();
  bool graph23();
  bool graph24();
  bool graph25();
  bool graph26();
  bool graph27();
  bool graph28();
  bool graph29();
  bool graph30();
  bool graph31();
  bool graph32();
  bool graph33();
  bool graph34();

 public:
  static map<string, bool (UNIT_TEST_Graph::*)()> TESTS;
  // ANSI escape codes for colors
  const string green = "\033[32m";
  const string red = "\033[31m";
  const string cyan = "\033[36m";
  const string reset = "\033[0m";  // To reset to default color

  // print result test case
  bool printResult(string output, string expect, string name) {
    if (expect == output) {
      cout << green << "test " + name + " --------------- PASS" << reset
           << "\n";
      return true;
    } else {
      cout << red << "test " + name + " --------------- FAIL" << reset << "\n";
      cout << "\texpect : " << expect << endl;
      cout << "\toutput : " << output << endl;
      return false;
    }
  }
  // run 1 test case
  void runTest(const std::string &name) {
    auto it = TESTS.find(name);
    if (it != TESTS.end()) {
      (this->*(it->second))();
    } else {
      throw std::runtime_error("Test with name '" + name + "' does not exist.");
    }
  }
  // run all test case
  void runAllTests() {
    vector<string> fails;
    for (const auto &test : TESTS) {
      if (!(this->*(test.second))()) {
        fails.push_back(test.first);
      }
    }

    cout << cyan << "\nResult -------------------------" << reset << endl;
    // Print the results
    if (fails.empty()) {
      cout << green << "All tests passed!" << reset << endl;
    } else {
      int totalTests = TESTS.size();
      int failedTests = fails.size();
      int passedTests = totalTests - failedTests;
      double passRate =
          (totalTests > 0)
              ? (static_cast<double>(passedTests) / totalTests) * 100.0
              : 0.0;
      cout << red << "Some tests failed:";
      for (const auto &fail : fails) {
        cout << "  " << fail;
      }
      cout << cyan << "\nPass rate: " << passRate << "%" << reset << endl;
    }
  }
  static void registerTest(string name, bool (UNIT_TEST_Graph::*function)()) {
    if (TESTS.find(name) != TESTS.end()) {
      throw std::runtime_error("Test with name '" + name + "' already exists.");
    }
    TESTS[name] = function;
  }
};

#endif  // UNIT_TEST_Graph_HPP