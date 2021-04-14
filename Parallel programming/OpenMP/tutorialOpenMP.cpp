#include <omp.h>
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
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
  int n = 16;
  printf("id | threadId\n");
  printf("-------------\n");
  #pragma omp parallel for schedule(static) num_threads(4)
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
void Use_private() {
  int x = 5;
  #pragma omp parallel private(x)
  {
    x = x + 1; // dangerous
    printf("private: x is %d\n", x);
  }
  printf("after: x is %d\n", x); // also dangerous
}
void Use_atomic1() {
  printf("\n%s\n", __FUNCTION__);

  int odd1 = 0;
  int odd2 = 0;
  int odd3 = 0;
  #pragma omp parallel for reduction(+:odd3)
  for (int i = 1; i <= 100; i++) {
    if (i % 2) {
      #pragma omp atomic
      odd1++;
      #pragma omp critical
      {
        odd2++;
      }

      odd3++;
    }
  }
  printf("odd1=%d, odd2=%d, odd3=%d\n", odd1, odd2, odd3);
}
void Use_atomic() {
  printf("\n%s\n", __FUNCTION__);
  int n = 1001;
  int sum = 0;
  int sum_atomic_omp = 0;
  int sum_reduction = 0;
  atomic<int> sum_atomic_std = 0;
  #pragma omp parallel for reduction(+:sum_reduction)
  for (int i = 0; i < n; i++) {
    sum += i;
    sum_reduction += i;
    sum_atomic_std += i;
    #pragma omp atomic
    sum_atomic_omp += i;
  }
  printf("sum=%d\n", sum);
  printf("sum_atomic_std=%d\n", (int)sum_atomic_std);
  printf("sum_atomic=%d\n", sum_atomic_omp);
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
    #pragma omp for nowait // missing some output
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
void Use_Lock() {
  omp_lock_t my_lock;
  omp_init_lock(&my_lock);
  #pragma omp parallel num_threads(4)
  {
    int tid = omp_get_thread_num( );
    int i, j;

    for (i = 0; i < 5; ++i) {
      omp_set_lock(&my_lock);
      printf_s("Thread %d - starting locked region\n", tid);
      printf_s("Thread %d - ending locked region\n", tid);
      omp_unset_lock(&my_lock);
    }
  }

  omp_destroy_lock(&my_lock);
}

void CheckOpenMPStatus() {
# ifdef _OPENMP
  printf_s("OpenMP Enabled.\n");
#else
  printf_s("OpenMP Not Enabled.\n");
# endif
}
void Use_single() {
  #pragma omp parallel
  {
    #pragma omp single
    printf("Single %d.\n", omp_get_thread_num());

    printf("step1,threadId=%d\n", omp_get_thread_num());
    #pragma omp barrier
    printf("step2, threadId=%d\n", omp_get_thread_num());
  }
}
int main() {
  //CheckOpenMPStatus();
  //Use_for();
  //Use_sections();

  Use_for_static();
  //Use_for_dynamic();
  //Use_for_guided();

  //Use_private();
  //Use_atomic();
  //Use_atomic1();
  //Use_nowait();
  //Use_Lock();
  Use_single();
  return 0;
}