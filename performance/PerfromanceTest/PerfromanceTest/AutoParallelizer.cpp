#include <stdio.h>
#include <functional>
#include <omp.h>
#include <ppl.h>
#include <windows.h>
#include <iostream>
#include <sstream>
#include <array>
#include <vector>
#include <random>
#include <array>
#include <chrono>
#include <thread>
#include <concurrent_vector.h>
#include <concurrent_unordered_map.h>
#include <ppltasks.h>
#include <agents.h>
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

void test_combinable() {
  combinable<int> working;
  parallel_for(1, 100, [&](int i) {
    working.local() += i;
  });

  int sum1 = working.combine([](int left, int right) {
    return left + right;
  });

  int sum2 = working.combine(std::plus<int>());
  cout << sum1 << " " << sum2 << endl;
}

void test_task1() {
  task<int> t0([]() { return 0; });
  t0.wait();
  cout << t0.get() << endl;
}
void test_task2() {
  task<int> t0([]() { return 0; });
  auto print = ([](int value) { cout << value << endl; });
  t0.then(print).wait();
}
void test_task2_1() {
  task<int> t0([]() { return 0; });
  auto print = ([](int value) { cout << value << endl; });
  t0.then(print).wait();
}
void test_task3() {
  auto t0 = create_task([]() { return 0; });
  auto print = ([](int value) { cout << value << endl; });
  t0.then(print).wait();
}
void test_task4() {
  task<void> t0([]() {cout << GetCurrentThreadId() << endl; return; });
  task<int> t1([]() {cout << GetCurrentThreadId() << endl; return 1; });
  t0.get();
  t1.get();
}
void test_task5() {
  vector<task<int>> v(1000);
  concurrent_unordered_map<int, int>m;
  for (size_t i = 0; i < v.size(); ++i) {
    v[i] = create_task([&]() {
      int threadId = GetCurrentThreadId();
      m[threadId]++;
      return 1;
    });
  }

  for (auto &item : v) item.get();

  cout << "items:" << m.size() << endl;
  cout << "thread, count" << endl;
  for (auto &item : m) {
    cout << item.first << " , " << item.second << endl;
  }
}

void test_when_all() {
  // Start multiple tasks.
  array<task<void>, 3> tasks = {
    create_task([] { wcout << L"Hello from taskA." << endl; }),
    create_task([] { wcout << L"Hello from taskB." << endl; }),
    create_task([] { wcout << L"Hello from taskC." << endl; })
  };

  auto joinTask = when_all(begin(tasks), end(tasks));
  //this_thread::sleep_for(std::chrono::microseconds(20));
  // Print a message from the joining thread.
  wcout << L"Hello from the joining thread." << endl;

  // Wait for the tasks to finish.
  joinTask.wait();
}
void test_when_any() {
  // Start multiple tasks.
  array<task<void>, 3> tasks = {
    create_task([] { wcout << L"Hello from taskA." << endl; }),
    create_task([] { wcout << L"Hello from taskB." << endl; }),
    create_task([] { wcout << L"Hello from taskC." << endl; })
  };

  auto joinTask = when_any(begin(tasks), end(tasks));
  //this_thread::sleep_for(std::chrono::microseconds(20));
  // Print a message from the joining thread.
  wcout << L"Hello from the joining thread." << endl;

  // Wait for the tasks to finish.
  joinTask.wait();
}

// Prints a message to the console.
template<typename T>
void print_message(T t) {
  wstringstream ss;
  ss << GetCurrentThreadId() << (L" Message from task: ") << t << endl;
  wcout << ss.str();
}

void test_task_group() {
  // A task_group object that can be used from multiple threads.
  task_group tasks;
  auto task1 = make_task([] { print_message(L"Hello"); });
  // Concurrently add several tasks to the task_group object.
  parallel_invoke(
  [&] {
    // Add a few tasks to the task_group object.
    tasks.run([] { print_message(L"Hello"); });
    tasks.run([] { print_message(42); });
  },
  [&] {
    // Add one additional task to the task_group object.
    tasks.run([] { print_message(3.14); });
  },
  task1
  );
  parallel_invoke(
    [] { print_message(L"Hello"); },
    [] { print_message(42); },
    [] { print_message(3.14); }
  );
  // Wait for all tasks to finish.
  tasks.wait();
}

void test_structured_task_group() {
  // Use the make_task function to define several tasks.
  auto task1 = make_task([] { print_message(L"task1,Hello"); });
  auto task2 = make_task([] {  print_message(24);  });
  auto task3 = make_task([] { print_message(3.14);  });

  // Create a structured task group and run the tasks concurrently.

  structured_task_group tasks;

  tasks.run(task1);
  tasks.run(task2);
  tasks.run_and_wait(task3);
  tasks.run(task3);
  tasks.wait();
}
void test_parallel_invoke() {
  // Use the make_task function to define several tasks.
  auto task1 = make_task([] { print_message(L"Hello"); });//task handle
  auto task2 = make_task([] { print_message(42);  });
  auto task3 = make_task([] { this_thread::sleep_for(chrono::milliseconds(2000));  print_message(3.14);  });
  parallel_invoke(task1, task2, task3);
  cout << "main threading." << endl;
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
  parallel_for(1, 6, [](int value) { std::cout << value << " "; });
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

// Creates a task that completes after the specified delay.
task<void> complete_after(unsigned int timeout) {
  printf_s("%s(timeout=%d ms) \n", __FUNCTION__, timeout);
  // A task completion event that is set when a timer fires.
  task_completion_event<void> tce;

  // Create a non-repeating timer.
  auto fire_once = new timer<int>(timeout, 0, nullptr, false);
  // Create a call object that sets the completion event after the timer fires.
  auto callback = new call<int>([tce](int) {
    tce.set();
  });

  // Connect the timer to the callback and start the timer.
  fire_once->link_target(callback);
  fire_once->start();

  // Create a task that completes after the completion event is set.
  task<void> event_set(tce);

  // Create a continuation task that cleans up resources and
  // and return that continuation task.
  return event_set.then([callback, fire_once]() {
    delete callback;
    delete fire_once;
  });
}

// Cancels the provided task after the specifed delay, if the task
// did not complete.
template<typename T>
task<T> cancel_after_timeout(task<T> t, cancellation_token_source cts, unsigned int timeout) {
  // Create a task that returns true after the specified task completes.
  task<bool> success_task = t.then([](T) {
    return true;
  });
  // Create a task that returns false after the specified timeout.
  task<bool> failure_task = complete_after(timeout).then([] {
    return false;
  });

  // Create a continuation task that cancels the overall task
  // if the timeout task finishes first.
  return (failure_task || success_task).then([t, cts](bool success) {
    if (!success) {
      // Set the cancellation token. The task that is passed as the
      // t parameter should respond to the cancellation and stop
      // as soon as it can.
      cts.cancel();
    }

    // Return the original task.
    return t;
  });
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
void test_parallel_transform() {
  std::vector<int> values = { 1, 2, 3, 4 };
  // Alternatively, use the negate class:
  parallel_transform(begin(values), end(values),
                     begin(values), negate<int>());
  for (size_t i = 0; i < values.size(); ++i) {
    cout << values[i] << endl;
  }
}
void test_paralle_reduce2() {
  // Create a vector of strings.
  vector<int> values;
  values.push_back(1);
  values.push_back(2);
  values.push_back(3);
  // Reduce the vector to one string in parallel.
  wcout << parallel_reduce(begin(values), end(values), 0) << endl;
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

  //test_paralle_reduce2();
  //test_parallel_transform();
  //test_combinable();
  //test_task1();
  //test_task2();
  //test_task3();
  //test_task4();
  //test_task5();
  //test_when_all();
  //test_when_any();
  //test_task_group();
  //test_structured_task_group();
  //test_parallel_invoke();
  complete_after(1000).wait();
  return 0;
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