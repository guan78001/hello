#include <stdio.h>
#include <functional>
#include <omp.h>
#include <windows.h>

__declspec(noinline) bool is_prime(int n) {
  for (int x = 2; x < n; ++x) {
    if (n % x == 0 && n != x) return false;
  }
  return true;
}

void test1() {
  enum { N = 200000 };
  long count = 0;
//#pragma loop(hint_parallel(4))
  #pragma omp parallel for
  for (int i = 3; i < 200000; ++i) {
    if (is_prime(i)) {
      //InterlockedIncrement(&count);
      //#pragma omp atomic
      //#pragma omp critical
      ++count;
    }
  }
  printf("count=%d\n", count);
}
void test2() {
  int A[2000] = { 0 };
//#pragma loop(hint_parallel(4))
  //#pragma omp parallel for
  for (int i = 0; i < 1024; ++i) {
    A[i] = A[i] + 1;
  }

  //for (int i = 1000; i < 2000; ++i) {
  //  A[i] = A[i] + 1;
  //}
  printf("A[0]=%d,A[1000]=%d\n", A[0], A[10]);
}
void Test(std::function<void()> func, char *str) {
  double t0 = omp_get_wtime();
  func();
  double t1 = omp_get_wtime();
  printf("%s used time %f ms\n", str, (t1 - t0) * 1000);
}
void test_omp_order() {
  #pragma omp parallel for ordered
  for (int i = 0; i < 8; i++) {
    printf("i=%d threadId=%d\n", i, omp_get_thread_num());
  }
}
void test_parallel() {
  #pragma omp parallel num_threads(4)
  {
    #pragma omp single
    // Only a single thread can read the input.
    printf_s("read input\n");

    // Multiple threads in the team compute the results.
    printf_s("compute results\n");

    #pragma omp single
    // Only a single thread can write the output.
    printf_s("write output\n");
  }
}
int main() {
  //test_omp_order();
  //test_parallel();
  //Test(test1, "test1");
  Test(test2, "test2");
  return 0;
}