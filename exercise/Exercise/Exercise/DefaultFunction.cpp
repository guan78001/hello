#include <iostream>
#include <vector>
using namespace std;

class A {
 public:
  A() {
    memset(arr, 0, sizeof(arr));
  }
  ~A() {

  }
  A(const A &) = default;
 public:
  float arr[16];
};
typedef enum : short {E1, E2} MY_TYPE;
struct A1 {
  MY_TYPE type;
  //int n;
};
int main() {
  vector<int> v(1);
  v.insert(v.begin() + 1, 2);

  //cout << sizeof(A1) << endl;
  //A a;
  //a.arr[0] = 1;
  //A b = a;
  //cout << b.arr[0] << endl;
  return 0;
}