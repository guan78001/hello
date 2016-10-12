#include <iostream>

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
int main() {
  std::cout << "StandardMethod\n";
  StandardMethod();
  std::cout << "CppAmpMethod\n";
  CppAmpMethod();
  return 0;
}