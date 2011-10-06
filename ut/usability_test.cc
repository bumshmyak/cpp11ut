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
#include <tuple>
#include <type_traits>
#include <cassert>

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::map;
using std::for_each;
using std::array;
using std::function;
using std::tuple;

template <bool FloatingPointParameter>
struct MadSkills {
  template <class T>
  static void Show(const T& parameter) {
    cout << "parameter is of floating point type" << endl;
  }
}; 

template <>
struct MadSkills<false> {
  template <class T>
  static void Show(const T& parameter) {
    cout << "non floating point parameter" << endl;
   }
}; 

template <class T>
void ShowMadSkills(const T& parameter) {
  MadSkills<std::is_floating_point<T>::value>::Show(parameter);
}

TEST(UsabilityTest, TypeInference) {
  auto x = 42;
  auto pi = 3.14;
  auto c = 'x';
  auto str = string("xxx");
  
  static_assert(std::is_same<decltype(x), int>::value, "42 is not int");
  static_assert(!std::is_same<decltype(x), unsigned int>::value,
                "42 is unsigned int");

  static_assert(std::is_same<decltype(pi), double>::value,
                "3.14 is not double");

  static_assert(std::is_same<decltype(c), char>::value,
                "'x' is not char");
  
  static_assert(std::is_same<decltype(str), string>::value,
                "str is std::string");

  vector< vector<int> > bigdata;
  auto first = bigdata.begin();

  auto seq = std::make_tuple(1, string("first"), 1.0);
  static_assert(std::is_same<decltype(seq),
                             std::tuple<int, string, double>>::value,
                  "not a tuple");
}

int get_initializer_list_size(std::initializer_list<int> init) {
  return init.size();
}

TEST(UsabilityTest, InitializerList) {
  // traditional POD type init
  {
    struct PODType {
      int first;
      double second;
    };

    PODType p = {42, 3.14};
    EXPECT_EQ(42, p.first);
    EXPECT_EQ(3.14, p.second);

    PODType pa[] = {{1, 1.5}, {2, 2.5}};
    for (int i = 0; i < 2; ++i) {
      EXPECT_EQ(i + 1, pa[i].first);
      EXPECT_EQ(i + 1.5, pa[i].second);
    }
  }

  // similar std containers init
  vector<int> a = {1, 2, 3};
  EXPECT_EQ(3, a.size());

  // my class init
  class SequenceClass {
   public:
    SequenceClass(std::initializer_list<int> list) :
        data_(list.begin(), list.end())
    { }
    int size() {
      return data_.size();
    }
   private:
    vector<int> data_;
  };

  {
    SequenceClass sequence = {1, 2, 3};
    EXPECT_EQ(3, sequence.size());
  }

  EXPECT_EQ(3, get_initializer_list_size({1, 2, 3}));

  {
    std::initializer_list<int> l = {1, 2, 3, 4};
    SequenceClass sequence(l);
    EXPECT_EQ(4, sequence.size());
  }
}

TEST(UsabilityTest, UniformInit) {
  {
    string a{"123"};
    string b{a};
    EXPECT_EQ(a, b);
    int c[] = {1, 2, 3};
  }

  {
    struct PODType {
      int first;
      double second;
    };

    PODType p{42, 3.14};
    EXPECT_EQ(42, p.first);
    EXPECT_EQ(3.14, p.second);
  }

  {
    class Person {
     public:
      Person(const string& name, int age) :
          name_{name},
        age_{age}
      { }
      string get_name() {
        return name_;
      }
      int get_age() {
        return age_;
      }
     private:
      string name_;
      int age_;
    };

    Person person{"jack", 23};
    EXPECT_EQ("jack", person.get_name());
    EXPECT_EQ(23, person.get_age());
    Person another = {"jim", 23};
  }

  std::vector<int> the_vec{4};
  EXPECT_EQ(1, the_vec.size());

  {
    int x = {1.1};
  }
}

class F {
 public:
  F(int left, int right, int& count) :
      left_(left),
      right_(right),
      count_(count)
  { }
  void operator()(int x) const {
    if (x >= left_ && x <= right_) {
      ++count_;
    }
  }
 private:
  int left_;
  int right_;
  int& count_;
};

vector<int> vmap(const vector<int>& input,
                 std::function<int (int)>& unary_operation) {
   vector<int> res;
   std::transform(input.begin(), input.end(),
                  back_inserter(res),
                  unary_operation);
   return res;
}

TEST(UsabilityTest, Lambda) {
  const int N = 10;
  vector<int> a(N, 1);

  {
    int left = 1;
    int right = 1;
    int count = 0;
    for_each(a.begin(), a.end(), [left, right, &count](int x) {
        if (x >= left && x <= right) {
          ++count;
        }
      });
    EXPECT_EQ(10, count);
  }

  {
    int left = 1;
    int right = 1;
    int count = 0;
    for_each(a.begin(), a.end(), F(left, right, count));
    EXPECT_EQ(10, count);
  }

  {
    vector<int> a;
    int last = 0;
    std::generate_n(back_inserter(a), N, [last]() mutable {
        return ++last;
      });
  }
  

  {
    int sum = 0;
    for_each(a.begin(), a.end(), [&sum](int x) {
        sum += x;
      });
    EXPECT_EQ(N, sum);
  }

  {
    int sum = 0;
    for_each(a.begin(), a.end(), [&](int x) {
        sum += x;
      });
    EXPECT_EQ(N, sum);
  }

  {
    for_each(a.begin(), a.end(), [](int& x) {
        x = -x;
      });
  }

  {
    int count = 0;
    for_each(a.begin(), a.end(), [&](int x) {
      ++count;
      });
    EXPECT_EQ(a.size(), count);
  }

  // map
  {
    vector<int> seq = {1, 2, 3, 4, 5};
    vector<int> res;
    std::transform(seq.begin(), seq.end(),
                   back_inserter(res),
                   [](int x) {
                     return x * x;
                   });
  }

  // reduce
  {
    vector<int> seq = {1, 2, 3, 4, 5};
    int res = 0;
    for_each(seq.begin(), seq.end(), [&res](int x) {
        res += x;
      });
  }

  // filter
  {
    vector<int> seq = {1, 2, 3, 4, 5};
    vector<int> res;
    copy_if(seq.begin(), seq.end(), back_inserter(res), [](int x) {
        return (x % 2 == 0);
      });
  }
}

TEST(UsabilityTest, RangeBasedForLoop) {
  array<int, 3> a = {1, 2, 3};
  for (auto& x : a) {
    x *= 2;
  }
  
  map<string, string> m =
      {{"first", "first"},
       {"second", "second"}};
  
  for (const auto& i : m) {
    EXPECT_EQ(i.first, i.second);
  }
}

TEST(UsabilityTest, MadSkillsTest) {
  ShowMadSkills(123);
}
