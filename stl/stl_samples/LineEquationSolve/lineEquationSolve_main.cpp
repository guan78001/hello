#include <iostream>
using namespace std;

#include "lineqn.h"

template<class T, int N>
void print(T a[N],  char *str = nullptr) {
  if (str) printf("array %s: ", str);
  for (int i = 0; i < N; i++) {
    cout << a[i] << " ";
  }
  cout << endl;
}
template<class T, int N>
void print2(T a[N][N], char *str = "") {
  if (str) printf("array %s:\n", str);
  for (int i = 0; i < N; i++) {
    print<T, N>(a[i]);
  }
}
void solve() {
  double a[3][3] = {
    1.0, 1, 0,
    0, 0, 4.0,
    1, 2.0, 0,
  };
  cout << "solve equation ax=b:" << endl;
  print2<double, 3>(a, "a");
  double b[3] = {1, 2, 1};
  print<double, 3>(b, "b");

  int index[3];
  double y[3];
  double x[3];
  cout << "\nludcmp:" << endl;
  ludcmp<double, 3>(a, index, y);
  print2<double, 3>(a, "a");
  print<double, 3>(y, "y");
  print<int, 3>(index, "index");
  print<double, 3>(y, "y");
  cout << "\nlubksb:" << endl;
  lubksb<double, 3>(a, index, b);
  print<double, 3>(b, "x");
}

//solve equation ax=b, get x in b.
template<class T, int N>
bool solve(T a[N][N], T b[N]) {
  T d;
  int index[N];
  bool res = ludcmp<T, N>(a, index, &d);
  if (res) {
    lubksb<T, N>(a, index, b);
  }
  return res;
}

//input matrix a, get invert matrix y. ay=I
template<class T, int N>
void invert(T a[N][N], T y[N][N]) {
  int index[N];
  T col[N];
  T d;
  ludcmp<T, N>(a, index, &d);
  for (int j = 0; j < N; j++ ) {
    for (int i = 0; i < N; i++) col[i] = 0.0;
    col[j] = 1.0;
    lubksb<double, N>(a, index, col);
    for (int i = 0; i < N; i++) y[i][j] = col[i];
  }
}

//get det of matrix a
template<class T, int N>
T det(T a[N][N]) {
  int index[N];
  T col[N];
  T d = 1.0;
  ludcmp<T, N>(a, index, &d);
  for (int j = 0; j < N; j++) {
    d *= a[j][j];
  }
  return d;
}

void test_solve() {
  double a[3][3] = {
    1.0, 0, 0,
    0, 2.0, 0,
    0, 0, 4.0,
  };
  double b[3] = { 1, 1, 1 };

  print2<double, 3>(a, "a");
  print<double, 3>(b, "b");
  bool res = solve<double, 3>(a, b);
  print<double, 3>(b, "x");
}
void test_invert() {
  double a[3][3] = {
    1.0, 0, 0,
    0, 2.0, 0,
    0, 0, 4.0,
  };
  double y[3][3];
  invert<double, 3>(a, y);
  print2<double, 3>(a, "a");
  print2<double, 3>(y, "inverse_a");
}
void test_det() {
  double a[3][3] = {
    1.0, 0, 0,
    0, 2.0, 0,
    0, 0, 4.0,
  };
  print2<double, 3>(a, "a");
  printf("det a=%f\n", det<double, 3>(a));
}
void test_LUDecom() {
  //double a[3][3] = {
  //  1, 2, 3,
  //  4, 5, 6,
  //  7, 8, 0
  //};

  //double a[3][3] = {
  //  10, -7, 0,
  //  5, -1, 5,
  //  -3, 2, 6
  //};// index [0,1,2]
  //double a[3][3] = {
  //  10, -7, 0,
  //  -3, 2, 6,
  //  5, -1, 5,
  //};// index [0,2,2]
  //double a[3][3] = {
  //  5, -1, 5,
  //  -3, 2, 6,
  //  10, -7, 0,
  //};// index [0,2,2]
  //double a[3][3] = {
  //  -3, 2, 6,
  //  5, -1, 5,
  //  10, -7, 0,
  //};// index [1,2,2]
  double a[3][3] = {
    1, 2, 1,
    2, 3, 2,
    4, 4, 3
  };// index [0,2,2]
  int index[3];
  double d;
  print2<double, 3>(a, "a");
  ludcmp<double, 3>(a, index, &d);
  print2<double, 3>(a, "a-lu");
  print<int, 3>(index, "index");
}
int main() {
  //solve();
  test_LUDecom();
  test_invert();
  test_solve();
  test_det();
  return 0;
}