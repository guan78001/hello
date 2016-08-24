//http://stackoverflow.com/questions/11413855/why-is-transposing-a-matrix-of-512x512-much-slower-than-transposing-a-matrix-of
//Why is transposing a matrix of 512x512 much slower than transposing a matrix of 513x513?
#define SAMPLES 1000
#define MATSIZE 513
#include <omp.h>
#include <stdio.h>
struct OmpTimer {
  OmpTimer() {
    t0 = omp_get_wtime();
  }
  OmpTimer(char *str) {
    m_str = str;
    t0 = omp_get_wtime();
  }
  ~OmpTimer() {
    if (m_str) {
      printf("%s used time %f\n", m_str, Duration(1000));
    }
  }
  double Duration(int unit = 1000) {
    double t1 = omp_get_wtime();
    return (t1 - t0) * unit;
  }
  double t0;
  char *m_str = nullptr;
};
//513:0.8ms, 512:2.4ms

#include <iostream>
int mat[MATSIZE][MATSIZE];

void transpose() {
  for (int i = 0; i < MATSIZE; i++)
    for (int j = 0; j < MATSIZE; j++) {
      int aux = mat[i][j];
      mat[i][j] = mat[j][i];
      mat[j][i] = aux;
    }
}
void test_transpose() {
  //initialize matrix
  for (int i = 0; i < MATSIZE; i++)
    for (int j = 0; j < MATSIZE; j++)
      mat[i][j] = i + j;

  OmpTimer timer;
  for (int i = 0; i < SAMPLES; i++)
    transpose();
  double time = timer.Duration();

  std::cout << "Average for a matrix of " << MATSIZE << ": " << time / SAMPLES  << " ms\n";
}

//Example 1: Memory accesses and performance
void test1(int n = 0) {
  int len = 64 * 1024 * 1024;
  int *arr = new int[len];

  for (int n = 0; n < 10; n++) {


    // Loop 1
    {
      OmpTimer t1("Loop1");
      for (int i = 0; i < len; i++) arr[i] *= 3;
    }

    // Loop 2, half of 1
    {
      OmpTimer t2("Loop2");
      for (int i = 0; i < len / 2; i++) arr[i] *= 3;
    }

    // Loop 3, similar as 1
    {
      OmpTimer t2("Loop3");
      for (int i = 0; i < len; i += 16) arr[i] *= 3;
    }

  }

  delete[] arr;
}
void test2() {
  static int A, B, C, D, E, F, G;
  {
    {
      OmpTimer t1("ABCD");
      for (int i = 0; i < 200000000; i++) {
        A++; B++; C++; D++;
      }
    }

    {
      OmpTimer t1("AC");
      for (int i = 0; i < 200000000; i++) {
        A++;  C++;
      }
    }
    {
      OmpTimer t1("ACEG");
      for (int i = 0; i < 200000000; i++) {
        A++; C++; E++; G++;
      }
    }

  }
}
int main() {
  test1(1);
  test2();
  //test_transpose();
}