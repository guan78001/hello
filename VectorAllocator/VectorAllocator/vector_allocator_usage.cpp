#include <stdio.h>
#include <iostream>
#include <vector>
#include "SimpleVector.h"
using namespace std;
#include "ChunkPoolAllocator.hpp"

class A {
 public:
  static A *instance() {
    static A a;
    return &a;
  }
 protected:
  A() {}
  ~A() {
    cout << "~A" << endl;
  };
};

void test1() {
  A *p = A::instance();
  vector<int, ChunkPoolAllocator<int>> v;
  for (int i = 0; i < 10; i++) {
    v.push_back(i);
  }

  for (int i = 0; i < v.size(); i++) {
    cout << v[i] << endl;
  }
}

void print(SimpleVector<int> &v, char *msg = nullptr) {
  if (msg) {
    printf("%s\n", msg);
  }
  printf("size()=%d,capacity()=%d\n", v.size(), v.capacity());
  for (int i = 0; i < v.size(); i++) {
    cout << v[i] << endl;
  }
  printf("\n");
}
int main() {
  //vector<int> sv;
  //sv.reserve(0);
  //cout << sv.capacity() << endl;
  //sv.push_back(0);
  //sv.insert(sv.end(), 0);

  SimpleVector<int> v;
  print(v, "init");
  v.insert(v.end(), 1);
  print(v, "insert");

  for (int i = 0; i < 4; i++) {
    v.push_back(i);
  }
  print(v, "push_back");

  v.resize(8);
  print(v, "after_resize()");

  v.insert(v.begin(), -1);
  print(v, "after_insert");

  v.insert(v.begin(), -2);
  print(v, "after_insert");

  v.insert(v.end(), 5);
  print(v, "after_insert");
  return 0;
}