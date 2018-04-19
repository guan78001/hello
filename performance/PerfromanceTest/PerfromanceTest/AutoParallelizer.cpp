#include <stdio.h>
#include <functional>
#include <omp.h>
#include <ppl.h>
#include <windows.h>
#include <iostream>
#include <iostream>
#include <array>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <concurrent_vector.h>
#include <concurrent_unordered_map.h>
using namespace concurrency;
using namespace std;
__declspec(noinline) bool is_prime(int n) {
  if (n < 2) return false;

  for (int x = 2; x < n; ++x) {
    if (n % x == 0 /*&& x < n*/) return false;
  }
  return true;
}

enum { N = 100000 };
void test_sequential() {
  long count = 0;
  for (int i = 2; i < N; ++i) {
    if (is_prime(i)) {
      ++count;
    }
  }
  printf("test_sequential, prime count=%d in [1..%d]\n", count, N);
}
void test_openMP() {
  long count = 0;
//#pragma loop(hint_parallel(4))
  #pragma omp parallel for schedule(dynamic,1) reduction(+:count)
  for (int i = 2; i < N; ++i) {
    if (is_prime(i)) {
      //InterlockedIncrement(&count);
      //#pragma omp atomic
      //#pragma omp critical
      ++count;
    }
  }
  printf("test_openMP, prime count=%d in [1..%d]\n", count, N);
}
void test_parallel_for() {
  long count = 0;
  //std::atomic<int> count = 0;
  //affinity_partitioner ap;
  parallel_for(int(2), int(N), [&](int i) {
    if (is_prime(i)) {
      InterlockedIncrement(&count);
      //++count;
    }
  }
  //,ap
  //, affinity_partitioner()
  , auto_partitioner() //fastest
  //, simple_partitioner() //compile error
  //, static_partitioner()
              );
  printf("test_parallel_for prime count=%d in [1..%d]\n", count, N);
}

void test2() {
  int A[2000] = { 0 };
//#pragma loop(hint_parallel(4))
  //#pragma omp parallel for
  for (int i = 0; i < 1024; ++i) {
    A[i] = A[i] + 1;
  }

  //for (int i = 1000; i < 2000; ++i) {
  //  A[i] = A[i] + 1;
  //}
  printf("A[0]=%d,A[1000]=%d\n", A[0], A[10]);
}
void Test(std::function<void()> func, char *str) {
  double t0 = omp_get_wtime();
  func();
  double t1 = omp_get_wtime();
  printf("%s used time %f ms\n", str, (t1 - t0) * 1000);
}
void test_omp_order() {
  #pragma omp parallel for ordered
  for (int i = 0; i < 8; i++) {
    printf("i=%d threadId=%d\n", i, omp_get_thread_num());
  }
}
void test_parallel() {
  #pragma omp parallel num_threads(4)
  {
    #pragma omp single
    // Only a single thread can read the input.
    printf_s("read input\n");

    // Multiple threads in the team compute the results.
    printf_s("compute results\n");

    #pragma omp single
    // Only a single thread can write the output.
    printf_s("write output\n");
  }
}
void simple_parallel_for() {
  // Print each value from 1 to 5 in parallel.
  parallel_for(1, 6, [](int value) {
    std::cout << value << " ";
  });
}
void simple_parallel_for_each() {
  std::array<int, 5> values = { 1, 2, 3, 4, 5 };
  parallel_for_each(begin(values), end(values), [](int value) {
    std::cout << value << " ";
  });
  cout << endl;
}
template <typename T>
T twice(const T &t) {
  return t + t;
}

void simple_parallel_invoke() {
  // Define several values.
  int n = 54;
  double d = 5.6;
  wstring s = L"Hello";

  // Call the twice function on each value concurrently.
  parallel_invoke(
    [&n] { n = twice(n); },
    [&d] { d = twice(d); },
    [&s] { s = twice(s); }
  );

  wcout << n << L' ' << d << L' ' << s.c_str() << endl;
}

void test_paralle_map_reduce() {
  // File 1
  vector<wstring> v1;
  v1.push_back(L"word1"); //1
  v1.push_back(L"word1"); //2
  v1.push_back(L"word2");
  v1.push_back(L"word3");
  v1.push_back(L"word4");

  // File 2
  vector<wstring> v2;
  v2.push_back(L"word5");
  v2.push_back(L"word6");
  v2.push_back(L"word7");
  v2.push_back(L"word8");
  v2.push_back(L"word1"); //3

  vector<vector<wstring>> v;
  v.push_back(v1);
  v.push_back(v2);

  concurrent_unordered_map<wstring, size_t> result;
  for_each(begin(v), end(v), [&result](const vector<wstring> &words) {
    parallel_for_each(begin(words), end(words), [&result](const wstring & word) {
      InterlockedIncrement(&result[word]);
    });
  });

  wcout << L"\"word1\" occurs " << result.at(L"word1") << L" times. " << endl;
}
void test_paralle_reduce() {
  // Create a vector of strings.
  vector<wstring> words;
  words.push_back(L"aa ");
  words.push_back(L"bb ");
  words.push_back(L"cc ");
  words.push_back(L"dd ");
  words.push_back(L"ee, ");
  words.push_back(L"ff ");
  words.push_back(L"gg ");
  words.push_back(L"hh.");

  // Reduce the vector to one string in parallel.
  wcout << parallel_reduce(begin(words), end(words), wstring()).c_str() << endl;
}
void test_sort() {
  // Create and sort a large vector of random values.
  vector<int> values(25000000);
  generate(begin(values), end(values), mt19937(42));
  sort(begin(values), end(values));
}
void test_parallel_sort() {
  // Create and sort a large vector of random values.
  vector<int> values(25000000);
  generate(begin(values), end(values), mt19937(42));
  parallel_sort(begin(values), end(values));
}
void test_parallel_buffered_sort() {
  // Create and sort a large vector of random values.
  vector<int> values(25000000);
  generate(begin(values), end(values), mt19937(42));
  parallel_buffered_sort(begin(values), end(values));
}
void test_parallel_radixsort() {
  // Create and sort a large vector of random values.
  vector<int> values(25000000);
  generate(begin(values), end(values), mt19937(42));
  parallel_radixsort(begin(values), end(values));
}
void test_concurrent_vector() {
  // Create a concurrent_vector object that contains a few
  // initial elements.
  concurrent_vector<int> v;
  v.push_back(2);
  v.push_back(3);
  v.push_back(4);

  // Perform two tasks in parallel.
  // The first task appends additional elements to the concurrent_vector object.
  // The second task computes the sum of all elements in the same object.

  parallel_invoke(
  [&v] {
    for (int i = 0; i <= 100; ++i) {
      v.push_back(i);
    }
  },
  [&v] {
    int ms = 1;
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));

    combinable<int> sums;
    for (auto i = begin(v); i != end(v); ++i) {
      sums.local() += *i;
    }
    wcout << L"sum = " << sums.combine(plus<int>()) << endl;
  }
  );
}
int main() {
  //simple_parallel_for();
  //simple_parallel_for_each();
  //simple_parallel_invoke();
  //test_paralle_reduce();
  //test_paralle_map_reduce();
  //return 0;
  //test_omp_order();
  //test_parallel();
  //test_concurrent_vector();
  //return 0;
#define TEST(Func) Test(Func,#Func);
  //Test(test_sequential, "test_sequential");
  TEST(test_sequential);
  TEST(test_openMP);
  TEST(test_parallel_for);

  TEST(test_sort);
  TEST(test_parallel_sort);
  TEST(test_parallel_buffered_sort);
  TEST(test_parallel_radixsort);

  return 0;
}