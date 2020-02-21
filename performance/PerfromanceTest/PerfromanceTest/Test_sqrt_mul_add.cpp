#include <omp.h>
#include <iostream>
#include <functional>
using namespace std;

void Test(std::function<void()> func, char *str) {
  double t0 = omp_get_wtime();
  func();
  double t1 = omp_get_wtime();
  printf("%s used time %f ms\n", str, (t1 - t0) * 1000);
}
#define TEST(Func) Test(Func,#Func);
float f;
int N = 100000;
void test_div() {
  f = 0;
  for (int i = 1; i < N; i++) {
    f += 0.12345f / ((float)i + 0.5);
  }
}
void test_mul() {
  f = 0;
  for (int i = 1; i < N; i++) {
    f += 0.12345f * ((float)i + 0.5);
  }
}
void test_sqrt() {
  f = 0;
  for (int i = 1; i < N; i++) {
    f += sqrt(((float)i + 0.5));
  }
}
int main() {
  TEST(test_div);
  TEST(test_mul);
  TEST(test_sqrt);
}