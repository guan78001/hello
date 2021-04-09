#include <omp.h>
#include <iostream>
#include <thread>
using namespace std;

//example
//https://docs.microsoft.com/en-us/cpp/parallel/openmp/a-examples?view=msvc-160#a6-the-lastprivate-clause

//!for usage
void Use_for() {
  int n = 16;
  printf("id | threadId\n");
  printf("-------------\n");
  int sum = 0;
  #pragma omp parallel for reduction(+:sum)
  for (int i = 0; i < n; i++ ) {
    sum += i;
    //#pragma omp critical (name)
    printf("%2d | %d\n", i, omp_get_thread_num());
  }
  printf("sum=%d\n", sum);
}

void Use_for_static() {
  int n = 32;
  printf("id | threadId\n");
  printf("-------------\n");
  #pragma omp parallel for schedule(static)
  for (int i = 0; i < n; i++ ) {
    printf("%2d | %d\n", i, omp_get_thread_num());
  }
}

void Use_for_dynamic() {
  printf("\n%s\n", __FUNCTION__);
  int n = 32;
  printf("id | threadId\n");
  printf("-------------\n");
  #pragma omp parallel for schedule(dynamic,1)
  for (int i = 0; i < n; i++ ) {
    printf("%2d | %d\n", i, omp_get_thread_num());
  }
}

void Use_for_guided() {
  printf("\n%s\n", __FUNCTION__);
  int n = 32;
  printf("id | threadId\n");
  printf("-------------\n");
  #pragma omp parallel for schedule(guided,1)
  for (int i = 0; i < n; i++ ) {
    printf("%2d | %d\n", i, omp_get_thread_num());
  }
}

//sections usage
void Use_sections() {
  printf("section | threadId\n");
  printf("------------------\n");
  #pragma omp parallel sections
  {
    #pragma omp section
    {
      printf("%8d|%8d\n", 0, omp_get_thread_num());
    }
    #pragma omp section
    {
      printf("%8d|%8d\n", 1, omp_get_thread_num());
    }
    #pragma omp section
    {
      printf("%8d|%8d\n", 2, omp_get_thread_num());
    }
  }
}

void Use_atomic() {
  printf("\n%s\n", __FUNCTION__);
  int n = 101;
  int sum = 0;
  int sum_atomic = 0;
  int sum_reduction = 0;
  #pragma omp parallel for reduction(+:sum_reduction)
  for (int i = 0; i < n; i++) {
    sum += i;
    sum_reduction += i;
#pragma atomic
    sum_atomic += i;
  }
  printf("sum=%d\n", sum);
  printf("sum_atomic=%d\n", sum_atomic);
  printf("sum_reduction=%d\n", sum_reduction);
}
void Sleep(int xMiniSeconds) {
  this_thread::sleep_for(std::chrono::milliseconds(xMiniSeconds));
}
void Use_nowait() {
  printf("\n%s\n", __FUNCTION__);
  int n = 4;
  #pragma omp parallel num_threads(2)
  {
    #pragma omp for nowait
    for (int i = 0; i < n; i++) {
      printf("%2d | %d, step1\n", i, omp_get_thread_num());
      Sleep(i + 10);
    }
    //#pragma omp for nowait
    for (int i = n; i < 2 * n; i++) {
      printf("%2d | %d, step2\n", i, omp_get_thread_num());
      Sleep(i + 10);
    }
  }
}
void CheckOpenMPStatus() {
# ifdef _OPENMP
  printf_s("OpenMP Enabled.\n");
#else
  printf_s("OpenMP Not Enabled.\n");
# endif
}

int main() {
  CheckOpenMPStatus();
  Use_for();
  Use_sections();

  Use_for_static();
  Use_for_dynamic();
  Use_for_guided();

  Use_atomic();
  Use_nowait();
  return 0;
}