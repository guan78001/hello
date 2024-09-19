#include <iostream>
#include <thread>
#include <omp.h>
using namespace std;
thread_local int thread_refineProgress = 0;

int main() {
  thread_refineProgress = 0;

  auto print = [&]() {
    #pragma omp critical
    {
      cout << std::this_thread::get_id() << " " << thread_refineProgress << endl;
    }
  };

  #pragma omp parallel for
  for (int i = 0; i < 16; i++) {
    thread_refineProgress = 0;
  }

  print();
  thread_refineProgress = 1;
  print();
  #pragma omp parallel for
  for (int i = 0; i < 16; i++) {
    //thread_refineProgress = i;
    print();
    thread_refineProgress += 100;
    print();
    thread_refineProgress += 1000;
    print();
  }
  print();

  return 0;
}