#include <ppl.h>
#include <vector>
#include <concurrent_vector.h>
#include <omp.h>
#include <iostream>
#include <windows.h>
#include <mutex>
using namespace std;
using namespace concurrency;

class CriticalSection {
 public:
  CriticalSection() {
    ::InitializeCriticalSection(&cs);
  }
  ~CriticalSection() {
    ::DeleteCriticalSection(&cs);
  }

  void lock() {
    ::EnterCriticalSection(&cs);
  }
  void unlock() {
    ::LeaveCriticalSection(&cs);
  }
 private:
  CRITICAL_SECTION cs;
};

class Benaphore {
 private:
  LONG m_counter;
  HANDLE m_semaphore;

 public:
  Benaphore() {
    m_counter = 0;
    m_semaphore = CreateSemaphore(NULL, 0, 1, NULL);
  }

  ~Benaphore() {
    CloseHandle(m_semaphore);
  }

  void lock() {
    if (_InterlockedIncrement(&m_counter) > 1) { // x86/64 guarantees acquire semantics
      WaitForSingleObject(m_semaphore, INFINITE);
    }
  }

  void unlock() {
    if (_InterlockedDecrement(&m_counter) > 0) { // x86/64 guarantees release semantics
      ReleaseSemaphore(m_semaphore, 1, NULL);
    }
  }
};

//https://msdn.microsoft.com/en-us/library/windows/desktop/aa904937(v=vs.85).aspx
class SlimeRWLock {
 private:
  SRWLOCK srwlock;
 private:
  SlimeRWLock(const SlimeRWLock &);
  SlimeRWLock &operator=(const SlimeRWLock &);
 public:
  SlimeRWLock() {
    ::InitializeSRWLock(&srwlock);
  }
  ~SlimeRWLock() {
  }
  void lock() {
    ::AcquireSRWLockExclusive(&srwlock);
  }
  void unlock() {
    ::ReleaseSRWLockExclusive(&srwlock);
  }
};
int N = 100000;
int sum = 0;//avoid optimization
void seq_vector() {
  std::vector<int> vi;
  //vi.reserve(N);
  for (int i = 0; i < N; ++i) {
    vi.push_back(i);
  }
  sum += vi[0];
}
void concurrency_cs() {
  std::vector<int> vi;
  //vi.reserve(N);
  critical_section cs;
  parallel_for(0, N, [&](int i) {
    critical_section::scoped_lock l(cs);
    vi.push_back(i);
  });
  sum += vi[0];
}

void concurrency_vector() {
  concurrent_vector<int> vi;
  //vi.reserve(N);
  Concurrency::parallel_for(0, N, [&](int i) {
    vi.push_back(i);
  });
  sum += vi[0];
}

void use_CRITICAL_SECTION() {
  std::vector<int> vi;
  //vi.reserve(N);
  CriticalSection cs;
  parallel_for(0, N, [&](int i) {
    std::lock_guard<CriticalSection> lock(cs);
    vi.push_back(i);
  });
  sum += vi[0];
}

void use_raw_CRITICAL_SECTION() {
  std::vector<int> vi;
  //vi.reserve(N);
  CRITICAL_SECTION cs;
  ::InitializeCriticalSection(&cs);
  parallel_for(0, N, [&](int i) {
    ::EnterCriticalSection(&cs);
    vi.push_back(i);
    ::LeaveCriticalSection(&cs);
  });
  ::DeleteCriticalSection(&cs);
  sum += vi[0];
}

void use_mutex() {
  std::vector<int> vi;
  std::mutex mu;
  parallel_for(0, N, [&](int i) {
    std::lock_guard<std::mutex> lock(mu);
    vi.push_back(i);
  });
  sum += vi[0];
}

template<class Locker>
void use_Lock() {
  std::vector<int> vi;
  Locker mu;
  parallel_for(0, N, [&](int i) {
    std::lock_guard<Locker> lock(mu);
    vi.push_back(i);
  });
  sum += vi[0];
}
////Benaphore mu;
//template<>
//void use_Lock<Benaphore>() {
//  std::vector<int> vi;
//  Benaphore mu;
//  parallel_for(0, N, [&](int i) {
//    std::lock_guard<Benaphore> lock(mu);
//    vi.push_back(i);
//  });
//  sum += vi[0];
//}
void test_time(char *function_name, std::function<void()> f) {
  double t0 = omp_get_wtime();
  f();
  double t1 = omp_get_wtime();
  printf("%s used time: %f ms\n", function_name, (t1 - t0) * 1000);
}
int main(int argc, char *argv[]) {
  cout << "sizeof(void*)=" << sizeof(void *) << endl;
#define NAME(func) #func
#define TEST(func) test_time(NAME(func),func);
  TEST(seq_vector);
  TEST(concurrency_vector);
  TEST(use_raw_CRITICAL_SECTION);
  TEST(use_CRITICAL_SECTION);
  TEST(concurrency_cs);
  TEST(use_mutex);

  cout << "template with lock\n";
  TEST(use_Lock<SlimeRWLock>);
  TEST(use_Lock<CriticalSection>);
  TEST(use_Lock<Benaphore>);
  TEST(use_Lock<mutex>);
  //cout << sum << endl;//avoid optimization
  return 0;
}
/*
results:
seq_vector used time: 0.437749 ms
concurrency_vector used time: 3.801487 ms
use_raw_CRITICAL_SECTION used time: 18.862535 ms
use_CRITICAL_SECTION used time: 22.012109 ms
concurrency_cs used time: 471.608812 ms
use_mutex used time: 780.871602 ms
template with lock
use_Lock<SlimeRWLock> used time: 3.479825 ms
use_Lock<CriticalSection> used time: 20.131729 ms
use_Lock<Benaphore> used time: 229.707786 ms
use_Lock<mutex> used time: 816.414024 ms
Press any key to continue . . .
*/