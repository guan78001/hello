#include <iostream>
#include <vector>
using namespace std;

class B {
 public:
  B() {
    cout << "B()\n";
  }
  ~B() {
    cout << "~B()\n";
  }
  //B(const B& rhs) {
  //  printf("B(const B& rhs)\n");
  //}
  //B& operator=(const B& rhs) {
  //  cout << "B operator=\n";
  //  return *this;
  //}
  std::vector<int> b;
};

class IA {
 public:
  IA() {
    cout << "IA()\n";
  }
  ~IA() {
    cout << "~IA()\n";
  }
};

class A:public IA {
 public:
  A() {
    cout << "A()\n";
    memset(arr, 0, sizeof(arr));
  }
  ~A() {

  }
  //A& operator=(const A& rhs) {
  //  cout << "A operator=\n";
  //  return *this;
  //}
  A(const A &) = default;
 public:
  float arr[16];
  vector<int> v;
  std::vector<B> b;
  B m_b;
};
typedef enum : short {E1, E2} MY_TYPE;
struct A1 {
  MY_TYPE type;
  //int n;
};

void func(const vector<A> & vec_a) {
  //vec_a[0].arr[0] = 1;//error
  //vec_a.push_back(A());//error
}
int main() {
  const char const ch = 'a';
  {
    A a;
    return 0;
  }
  vector<A> vec_a(1);

  A a;
  B b;
  b.b.push_back(1);
  b.b.push_back(2);
  a.b.push_back(b);
  A a2;
  a2 = a;
  cout << a2.b.size() << endl;
  cout << a2.b[0].b.size() << endl;
  //cout << sizeof(A1) << endl;
  //A a;
  //a.arr[0] = 1;
  //A b = a;
  //cout << b.arr[0] << endl;
  return 0;
}