#include <ppl.h>
#include <vector>
#include <concurrent_vector.h>
#include <omp.h>
#include <iostream>
#include <windows.h>
#include <mutex>
using namespace std;
using namespace concurrency;

int N = 100000;
void concurrency_cs() {
  std::vector<int> vi;
  critical_section cs;
  parallel_for(0, N, [&](int i) {
    critical_section::scoped_lock l(cs);
    vi.push_back(i);
  });
  cout << vi[0] << endl;
}

void concurrency_vector() {
  concurrent_vector<int> vi;
  Concurrency::parallel_for(0, N, [&](int i) {
    vi.push_back(i);
  });
  cout << vi[0] << endl;
}

void use_CRITICAL_SECTION() {
  std::vector<int> vi;
  CRITICAL_SECTION cs;
  ::InitializeCriticalSection(&cs);
  parallel_for(0, N, [&](int i) {
    ::EnterCriticalSection(&cs);
    vi.push_back(i);
    ::LeaveCriticalSection(&cs);
  });
  ::DeleteCriticalSection(&cs);
  cout << vi[0] << endl;
}

void use_mutex() {
  std::vector<int> vi;
  std::mutex mu;
  parallel_for(0, N, [&](int i) {
    std::lock_guard<std::mutex> lock(mu);
    vi.push_back(i);
  });
  cout << vi[0] << endl;
}
void test_time(char *function_name, std::function<void()> f) {
  double t0 = omp_get_wtime();
  f();
  double t1 = omp_get_wtime();
  printf("%s used time: %f ms\n", function_name, (t1 - t0) * 1000);
}
int main(int argc, char *argv[]) {
#define NAME(func) #func
#define TEST(func) test_time(NAME(func),func);

  TEST(concurrency_cs);
  TEST(concurrency_vector);
  TEST(use_CRITICAL_SECTION);
  TEST(use_mutex);
  return 0;
}