#include <iostream>
#include <vector>
using namespace std;

void StandardMethod() {

  int aCPP[] = { 1, 2, 3, 4, 5 };
  int bCPP[] = { 6, 7, 8, 9, 10 };
  int sumCPP[5];

  for (int idx = 0; idx < 5; idx++) {
    sumCPP[idx] = aCPP[idx] + bCPP[idx];
  }

  for (int idx = 0; idx < 5; idx++) {
    std::cout << sumCPP[idx] << "\n";
  }
}

#include <amp.h>
#include <iostream>
using namespace concurrency;

const int size = 5;

void CppAmpMethod() {
  int aCPP[] = { 1, 2, 3, 4, 5 };
  int bCPP[] = { 6, 7, 8, 9, 10 };
  int sumCPP[size];

  // Create C++ AMP objects.
  array_view<const int, 1> a(size, aCPP);
  array_view<const int, 1> b(size, bCPP);
  array_view<int, 1> sum(size, sumCPP);
  sum.discard_data();

  parallel_for_each(
    // Define the compute domain, which is the set of threads that are created.
    sum.extent,
    // Define the code to run on each thread on the accelerator.
  [ = ](index<1> idx) restrict(amp) {
    sum[idx] = a[idx] + b[idx];
  }
  );

  // Print the results. The expected output is "7, 9, 11, 13, 15".
  for (int i = 0; i < size; i++) {
    std::cout << sum[i] << "\n";
  }
}
template<class T>
void print(std::vector<T> &vec) {
  for (T n : vec) {
    cout << n << endl;
  }
}
void test1() {
  int arr[4] = { 1, 2, 3, 4 };

  std::vector<array<int, 1>*> vec(2);


  vec[0] = new array<int, 1>(4, arr);
  vec[1] = new array<int, 1>(4, arr);

  array<int, 1> &obj = *vec[0];
  parallel_for_each(
    obj.extent,
  [&](index<1> idx) restrict(amp) {
    //obj[idx] = obj[idx] * 2;
    obj(idx) = obj(idx) * 2;
  }
  );

  std::vector<int> out;
  out = obj;

  print(out);
}
void test2() {

  array<int, 2> arr(2, 3);
  parallel_for_each(
    arr.extent,
  [&](index<2>idx) restrict(amp) {
    int x = idx[0];
    int y = idx[1];
    arr[x][y] = 100 * x + y;
    //arr(idx) = x + y;
  }
  );
  std::vector<int>out = arr;
  print(out);

  cout << "=========" << endl;
  std::vector<int> out2(arr.extent.size());
  for (int i = 0; i < out2.size(); i++) {
    out2[i] = 1 + arr[i / 3][i % 3];
  }
  print(out2);
}
void test3() {
}
int main() {
  test2();
  return 0;

  StandardMethod();
  CppAmpMethod();
  return 0;
}