#include <omp.h>
#include <iostream>
#include <algorithm>
#include <mutex>
#include <thread>
#include <chrono>
#include <vector>
#include <sstream>
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

void test_nest2() {
  #pragma omp parallel for
  for (int i = 0; i < 8; i++) {
    int tid = omp_get_thread_num();
    #pragma omp parallel for
    for (int j = 0; j < 4; j++) {
      //Lock lock(io_mutex);
      printf("i=%d,tid=%d,  j=%d, thread_id=%d\n", i, tid, j, omp_get_thread_num());
    }
  }
}
void test_nest() {
  omp_set_nested(1);
  #pragma omp parallel for
  for (int i = 0; i < 8; i++) {
    int tid = omp_get_thread_num();
    if (i == 0) {
      for (int k = 0; k < 1000000; k++) {
        printf("k=%d\n", k);
      }
    }
    #pragma omp parallel for
    for (int j = 0; j < 4; j++) {
      //Lock lock(io_mutex);
      printf("i=%d,tid=%d,  j=%d, thread_id=%d\n", i, tid, j, omp_get_thread_num());
    }
  }

  cout << "\n\n";
  test_nest2();
}

void test_lock(int n) {
  vector<thread> ths;
  int N = 4096;
  for (int i = 0; i < N; i++) {
    ths.push_back(thread([&]() {
      //int id = i;
      //printf("i=%d,n=%d\n", id, n);
      //this_thread::sleep_for(chrono::seconds(1));
      #pragma omp critical
      {n += i; }
      //printf("i=%d,n=%d\n", id, n);
    }));
  }
  for (int i = 0; i < ths.size(); i++) {
    ths[i].join();
  }

  int x = 0;
  #pragma omp parallel for
  for (int i = 0; i < N; i++) {
    #pragma omp critical
    {x += i; }
  }
  int sum = 0;
  for (int i = 0; i < N; i++) {
    sum += i;
  }
  cout << "target=" << sum << endl;
  cout << "actual_th=" << n << endl;
  cout << "actual_omp=" << x << endl;
}
void Print() {
  cout << "Print" << endl;
}
void Print2() {
  cout << "Print2" << endl;
}
int Print3() {
  cout << "Print3" << endl;
  return 0;
}


int x = 0;
void func(int tid) {
  #pragma omp critical
  {
    x++;
    std::stringstream ss;
    ss << std::this_thread::get_id();
    ss >> tid;
    this_thread::sleep_for(chrono::seconds(1));
    printf("tid=%d,omp_id=%d,x=%d\n", tid, omp_get_thread_num(), x);
  }

}
void test_critical_outside() {
#if 0
  std::vector<std::thread> vecs;
  for (int i = 0; i < 8; i++) {
    int tid = omp_get_thread_num();
    vecs.emplace_back(func, tid);
  }
  for (int i = 0; i < 8; i++) {
    vecs[i].join();
  }
#else
  #pragma omp parallel for
  for (int i = 0; i < 8; i++) {
    int tid = omp_get_thread_num();
    func(tid);
  }
#endif
}
int main() {
  test_critical_outside();
  return 0;
  //test1(4, 4);
  //test2();
  //test2_1();
  //test2_2();
  //test2_3();
  //test_nest();
  test_lock(0);
  return 0;
}