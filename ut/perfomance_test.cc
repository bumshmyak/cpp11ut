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

using std::string;
using std::vector;
using std::cout;
using std::cerr;
using std::endl;
using std::map;
using std::for_each;
using std::array;
using std::function;
using std::swap;
using std::copy;

namespace constexpr_check {
constexpr int get_answer() { return 42; }
int a[get_answer()];
};

class IntegerSequence {
 public:
  IntegerSequence() :
      data_{nullptr},
      size_{0}
  { }
    
  IntegerSequence(std::initializer_list<int> init) {
    cout << "init list ctor" << endl;
    size_ = init.size();
    data_ = new int[size_];
    std::copy(init.begin(), init.end(), data_);
  }
  
  IntegerSequence(const IntegerSequence& sequence) {
    cout << "copy ctor" << endl;
    size_ = sequence.size_;
    data_ = new int[size_];
    std::copy(sequence.data_, sequence.data_ + size_, data_);
  }

  IntegerSequence& operator=(const IntegerSequence& sequence) {
    cout << "copy =" << endl;
    if (this == &sequence) {
      return *this;
    }
    delete data_;
    size_ = sequence.size_;
    data_ = new int[size_];
    std::copy(sequence.data_, sequence.data_ + size_, data_);
  }

  IntegerSequence(IntegerSequence&& sequence) :
      data_(nullptr),
      size_(0) {
    assert(false);
    cout << "move ctor" << endl;
    swap(data_, sequence.data_);
    swap(size_, sequence.size_);
  }

  IntegerSequence& operator=(IntegerSequence&& sequence) {
    cout << "move =" << endl;
    data_ = nullptr, size_ = 0;
    swap(data_, sequence.data_);
    swap(size_, sequence.size_);
    return *this;
  }

  ~IntegerSequence() {
    delete data_;
  }
  
 public:
  int* data_;
  int size_;
};

IntegerSequence get_integer_sequence() {
  IntegerSequence res;
  return res;
}

TEST(PerfomanceTest, MoveSemantics) {
  IntegerSequence first{1, 2, 3};
  IntegerSequence second(first);
  second = first;
  IntegerSequence third(get_integer_sequence());
  third = get_integer_sequence();
}
