#include <omp.h>
#include <iostream>
#include <algorithm>
#include <mutex>
using namespace std;

std::mutex io_mutex;
typedef std::unique_lock<std::mutex> Lock;
void test1(int Lines, int Columns) {
  #pragma omp parallel
  {
    for (int j = 0; j < Lines; j++) {
      {
        Lock lock(io_mutex);
        cout << j << " " << omp_get_thread_num() << endl;
      }

      #pragma omp for
      for (int k = 0; k < Columns; k++) {
        {
          Lock lock(io_mutex);
          cout << j << " " << k << " " << omp_get_thread_num() << endl;
        }
      }
    }
  }
}

void test2() {
  cout << __FUNCTION__ << endl;
  #pragma omp parallel for
  for (int k = 0; k < 4; k++) {
    int tid = omp_get_thread_num(); // this call inside loop prevents vectorization
    int nthreads = omp_get_num_threads(); // this call inside loop prevents vectorization
    {
      Lock lock(io_mutex);
      cout << k << " " << tid << " " << nthreads << endl;
    }
  }
}
void test2_1() {
  cout << __FUNCTION__ << endl;
  int tid;
  int nthreads;
  #pragma omp parallel for private(tid,nthreads)
  for (int k = 0; k < 4; k++) {
    tid = omp_get_thread_num(); // this call inside loop prevents vectorization
    nthreads = omp_get_num_threads(); // this call inside loop prevents vectorization
    {
      Lock lock(io_mutex);
      cout << k << " " << tid << " " << nthreads << endl;
    }
  }
}
void test2_2() {
  cout << __FUNCTION__ << endl;
  int tid;
  int nthreads;
  #pragma omp parallel private(tid,nthreads)
  {
    tid = omp_get_thread_num();
    nthreads = omp_get_num_threads();

    for (int k = tid; k < 4; k += nthreads) {
      Lock lock(io_mutex);
      cout << k << " " << tid << " " << nthreads << endl;
    }
  }
}

void test2_3() {
  cout << __FUNCTION__ << endl;
  int tid;
  int nthreads;
  #pragma omp parallel private(tid,nthreads)
  {
    tid = omp_get_thread_num();
    nthreads = omp_get_num_threads();

    int N = 4;
    int block = N / nthreads;
    int start = tid * block;
    int end = std::min(start + block, N);
    for (int k = start; k < end; k++) {
      Lock lock(io_mutex);
      cout << k << " " << tid << " " << nthreads << endl;
    }
  }
}
int mysum(int start, int end, int a) {
  int iret = 0;
#pragma loop_count min(3), max(10), avg(5)
  for (int i = start; i <= end; i++)
    iret += a;
  return iret;
}

void f(int a[], int b[], int c[], int d[]) {
#pragma simd vectorlength(4)
  for (int i = 1; i < 100; i++) {
    b[i] = a[i] + 1;
    d[i] = c[i] + 1;
  }
}
int main() {
  //test1(4, 4);
  test2();
  test2_1();
  test2_2();
  test2_3();
  return 0;
}