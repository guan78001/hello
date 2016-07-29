#include <sys/timeb.h>
#include <omp.h>
#include <stdio.h>
#include <windows.h>
#include <chrono>
#include <iostream>
using namespace std::chrono;

//https://msdn.microsoft.com/en-us/library/dn553408(v=vs.85).aspx#AppendixA
//other ref: http://www.blogbus.com/cjbskysea-logs/46688056.html
//The best precision timer recommended by MSDN.
class PerformanceMetricsImpl
{
  std::string name_;
  LARGE_INTEGER timestamp_;
  LARGE_INTEGER frequence_;

 public:
  PerformanceMetricsImpl()
  {
    QueryPerformanceCounter(&timestamp_);
    QueryPerformanceFrequency(&frequence_);
  }
  ~PerformanceMetricsImpl()
  {
    printf("frequence_.QuadPart=%d\n", frequence_.QuadPart);
  }
  void reset()
  {
    QueryPerformanceCounter(&timestamp_);
  }

  double duration()
  {
    LARGE_INTEGER current_timestamp_;
    QueryPerformanceCounter(&current_timestamp_);
    return (double)(current_timestamp_.QuadPart - timestamp_.QuadPart) / frequence_.QuadPart;
  }
};
//omp_timer has similar precision as QueryPerformanceCounter
struct OmpTimer
{
  OmpTimer()
  {
    t0 = omp_get_wtime();
  }
  ~OmpTimer()
  {
    double t1 = omp_get_wtime();
    printf("OmpTimer %f s\n", t1 - t0);
  }
  double t0;
};

double Time(void)
{
  struct _timeb t;
  _ftime64_s(&t);
  return double(t.time) + double(t.millitm) / 1000.0;
}
struct TimebTimer
{
  TimebTimer()
  {
    t0 = Time();
  }
  ~TimebTimer()
  {
    double t1 = Time();
    printf("TimebTimer %f s\n", t1 - t0);
  }

  double t0;
};

int main()
{
  for (int i = 0; i < 10; i++)
  {

    double omp0 = omp_get_wtime();
    PerformanceMetricsImpl impl;
    double tb0 = Time();

    const auto begin = high_resolution_clock::now();

    //Sleep(10);
    for (int i = 0; i < 100000; i++)
    {
      omp_get_wtime();
    }

    auto diff_chrono = high_resolution_clock::now() - begin;

    double tb1 = Time();
    double omp1 = omp_get_wtime();
    double diff_counter = impl.duration();



    double diff_omp = omp1 - omp0;
    double  diff_timb = tb1 - tb0;

    printf("diff_chrono=%f, diff_timb=%f, diff_omp=%f, diff_counter=%f\n",
           duration<double>(diff_chrono).count(), diff_timb, diff_omp, diff_counter);
  }

  return 0;
}
