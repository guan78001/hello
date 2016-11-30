#include <stdio.h>
#include <iostream>
using namespace std;

template<class T>
float div(T val) {
  cout << "***" << 1 / val << endl;
  return (float)1 / val;
}
class A {
 public:
  virtual void test() {
    cout << "A::test" << endl;
  }
  virtual ~A() {
    cout << "~A" << endl;
  }
};

class B: public A {
 public:
  virtual void test() {
    cout << "B::test" << endl;
  }
  virtual ~B() {
    cout << "~B" << endl;
  }
};
int main() {

  A *p = new  B;
  p->test();
  delete p;

  double *d = new double{ 1.2f };
  cout << *d << endl;
  cout << div(2) << endl;
  cout << div(2.0) << endl;
  return 0;
}