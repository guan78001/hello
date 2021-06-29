#include "dll1.h"
#include <stdio.h>
#include <omp.h>
void TestOpenMP(int n) {
  printf("TestOpenMP n=%d\n", n);
  #pragma omp parallel for
  for (int i = 0; i < n; i++) {
    #pragma omp critical
    printf("i=%d, th=%d\n", i, omp_get_thread_num());
  }
}