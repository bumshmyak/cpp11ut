#include <gtest/gtest.h>
#include <type_traits>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <initializer_list>
#include <algorithm>
#include <array>
#include <functional>
#include <complex>

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::map;
using std::for_each;
using std::array;
using std::function;
using std::swap;
using std::complex;

TEST(FunctionalityTest, RawStrings) {
  string s = R"(\w\\\w)";
  // cout << s << endl;
}



