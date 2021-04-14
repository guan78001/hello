#include <thread>
#include <mutex>
#include <iostream>
#include <condition_variable>
#include <vector>
#include <atomic>
#include <map>
using namespace std;

void use_mutex() {
  vector<int> v;
  std::mutex mu;
  #pragma omp parallel for
  for (int i = 0; i < 10; i++) {
    //std::lock_guard<std::mutex> lock(mu);
    mu.lock();
    v.push_back(i);
    mu.unlock();
  }
  for (int i = 0; i < v.size(); i++) {
    cout << v[i] << " ";
  }
  cout << endl;
}

std::vector<int> v;
std::mutex mu;
std::condition_variable cv;
std::atomic_bool isWorking = true;
void input(int n) {
  {
    std::unique_lock<std::mutex> lock(mu);
    v.push_back(n);
    cout << "input: " << n << endl;
  }
  cv.notify_all();
}

void output() {
  while (true) {
    std::unique_lock<std::mutex> lock(mu);
    cv.wait(lock, []() {return v.size() > 0 || !isWorking; });

    if (!isWorking) return;
    cout << "output: " << v.back() << endl;
    v.pop_back();

    lock.unlock();
    cv.notify_all();
  }
}
void stop() {
  std::unique_lock<std::mutex> lock(mu);
  cv.wait(lock, []() {return v.size() == 0; });
  isWorking = false;
  cv.notify_all();
}

void use_cv() {
  thread th3([]() {output(); });
  thread th1([]() {input(1); });
  thread th2([]() {input(2); });
  th1.join();
  th2.join();

  stop();
  th3.join();
}

void thread_sample() {
  std::thread th([]() { cout << this_thread::get_id() << std::endl; });
  cout << this_thread::get_id() << std::endl;
  th.join();
}
void use_stl_in_threads() {
  map<int, int> m;
  int n = 4;
  //init first
  for (int i = 0; i < n; i++) m[i] = 0;

  #pragma omp parallel for num_threads(n)
  for (int i = 0; i < n; i++) {
    m[i] = 1;
  }

  std::vector<int> v(n);
  #pragma omp parallel for num_threads(n)
  for (int i = 0; i < n; i++) {
    v[i] = 1;
  }
}
int main() {
  //thread_sample();
  //use_mutex();
  //use_cv();

  use_stl_in_threads();
  return 0;
}