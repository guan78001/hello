#include <stdio.h>
#include <windows.h>

#include "perf_counter.h"
#include "rwlock.h"
#include "RWCriticalSection.h"
#include "RWLockFavorNeither.h"
#include "RWLockFavorWriters.h"
#include "SlimRWLock.h"
#include "Ruediger_Asche_RWLock.h"
#include "Jim_B_Robert_Wiener_RWLock.h"

//https://www.codeproject.com/Articles/32685/Testing-reader-writer-locks
HANDLE g_start_event;
volatile long g_stop_test;

const int writer_test_iterations = 1000000;
const int number_of_writers = 1;
const int reader_test_iterations = 4000000;
const int number_of_readers = 4;

template <typename lock_type>
class test_value_t {
 public:
  typedef ref_exclusive_lock_t<lock_type> exclusive_lock;
  typedef ref_shared_lock_t<lock_type> shared_lock;

  bool read(void) {
    shared_lock lock(m_lock);
    return (m_checksum == checksum(m_buffer, sizeof(m_buffer)));
  }
  bool write(void) {
    exclusive_lock lock(m_lock);
    set_buffer();
    return true;
  }
  test_value_t() {
    set_buffer();
  }
 private:
  USHORT checksum( LPVOID data, int len )
  //
  // The function checksum came from http://ava.org.ua/?2&17&get=87&read=87
  //
  {
    ULONG sum = 0;
    USHORT *dp =  (USHORT *) data;
    USHORT sum_s;
    int words = len >> 1;
    while ( words -- )  sum += * dp ++;
    if ( len & 1 ) sum += *(UCHAR *) dp;
    sum   = (USHORT) sum + (sum >> 16) & 0xffff;
    sum_s = (USHORT) sum + (USHORT)(sum >> 16);
    return sum_s != 0xffff ? ~sum_s : sum_s;
  }
  void set_buffer(void) {
    for (int i = 0; i < sizeof(m_buffer) / sizeof(m_buffer[0]); i ++) {
      m_buffer[i] = rand();
    }
    m_checksum = checksum(m_buffer, sizeof(m_buffer));
  }
  enum {BUF_SIZE = 64};
  int m_buffer[BUF_SIZE];
  USHORT m_checksum;
  lock_type m_lock;
};

//test_value_t <RWLockFavorWriters> g_value;
//test_value_t <RWLockFavorNeither> g_value;
//test_value_t <SlimRWLock> g_value;
//test_value_t <RWCriticalSection> g_value;
test_value_t <Jim_B_Robert_Wiener_RWLock> g_value;
//test_value_t <Ruediger_Asche_RWLock> g_value;


template <const int test_iteration>
class Test_worker {
 public:
  HANDLE start(void) {
    ::ResumeThread(m_handle);
    return m_handle;
  }
  double test_duration(void) {
    return m_perf_counter.get_duration();
  }
  LONGLONG test_iteration_done(void) {
    return m_perf_counter.get_counter();
  }
 private:
  static DWORD WINAPI worker_proc(LPVOID lpParam) {
    Test_worker &worker = *reinterpret_cast<Test_worker *>(lpParam);
    ::WaitForSingleObject(g_start_event, INFINITE );

    worker.m_perf_counter.start();
    int i;

    for (i = 0; i < test_iteration; i++) {
      if ( false == worker.simulate_work() ) {
        ::RaiseException( STATUS_NONCONTINUABLE_EXCEPTION, 0, 0, 0);
      } else {
      }

      if (TRUE == g_stop_test) {
        break;
      } else {
      }

    }

    worker.m_perf_counter.end();
    ::InterlockedExchange(&g_stop_test, TRUE);
    worker.m_perf_counter.set_iteration_done(i);
    return 0;
  }

  virtual bool simulate_work(void) = 0;

  HANDLE m_handle;
  Performance_counter_meter m_perf_counter;
  Test_worker &operator=( const Test_worker & ) {}
 protected:
  Test_worker() {
    m_handle = ::CreateThread(NULL, 0, worker_proc, (LPVOID)this, CREATE_SUSPENDED, NULL);
  }
};

class Writer: public Test_worker <writer_test_iterations> {
  virtual bool simulate_work(void) {
    return g_value.write();
  }
};

class Reader: public Test_worker <reader_test_iterations> {
  virtual bool simulate_work(void) {
    return g_value.read();
  }
};

int main() {
  g_start_event = ::CreateEvent(NULL, TRUE, FALSE, NULL);

  HANDLE threads[number_of_writers + number_of_readers];

  Writer *writer[number_of_writers];
  for (int i = 0; i < number_of_writers; i++) {
    writer[i] = new Writer();
    threads[i] = writer[i]->start();
  }

  Reader *reader[number_of_readers];
  for (int i = 0; i < number_of_readers; i++) {
    reader[i] = new Reader();
    threads[i + number_of_writers] = reader[i]->start();
  }

  ::SetEvent(g_start_event);
  ::WaitForMultipleObjects(sizeof(threads) / sizeof(threads[0]), threads, TRUE, INFINITE);

  for (int i = 0; i < number_of_readers; i++) {
    printf("reader iterations done | per operation(ms),%lld,%f\n",
           reader[i]->test_iteration_done(),
           1000 * reader[i]->test_duration() / reader[i]->test_iteration_done());

  }

  for (int i = 0; i < number_of_writers; i++) {
    printf("writer iterations done | per operation(ms),%lld,%f\n",
           writer[i]->test_iteration_done(),
           1000 * writer[i]->test_duration() / writer[i]->test_iteration_done());
  }

  return 0;
}

