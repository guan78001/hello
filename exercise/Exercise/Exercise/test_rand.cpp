#include <random>
#include <iostream>
#include <ctime>
#include <omp.h>
#include <functional>
using namespace std;

int N = 1000;
double test1() {
  double sum = 0;
  srand(time(0));
  for (int i = 0; i < N; i++) {
    sum += rand() % 100;
  }
  return sum;
}
double test2() {
  double sum = 0;
  mt19937 mt(time(nullptr));
  //uniform_int_distribution<int> my_rand(0, 100);
  #pragma omp parallel for
  for (int i = 0; i < N; i++) {
    //sum += my_rand(mt);//slow
    sum += mt() % 100;//fastest
  }
  return sum;
}
void test_rand_mt19937() {
  double t0 = omp_get_wtime();
  auto sum1 = test1();
  double t1 = omp_get_wtime();
  auto sum2 = test2();
  double t2 = omp_get_wtime();
  printf("sum1=%f, sum2=%f, t1=%f t2=%f", sum1, sum2, t1 - t0, t2 - t1);
}
void test0() {
  //ÖØ¸´ÖÜÆÚ 2^19937-1
  //mt19937 mt(time(0));
  mt19937 mt(0);

  for (int i = 0; i < 10; i++) {
    cout << mt() << endl;
  }

  uniform_int_distribution<int> my_rand(0, 100);
  for (int i = 0; i < 10; i++) {
    cout << my_rand(mt) << endl;
  }
}
int main() {
  test_rand_mt19937();
  test0();

  return 0;
}