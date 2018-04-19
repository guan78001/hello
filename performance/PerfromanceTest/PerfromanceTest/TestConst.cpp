#include <iostream>
#include <thread>
#include <vector>
using namespace std;

class B {
 public:
  B() {
    #pragma omp critical
    cout << this_thread::get_id() << "  B()" << endl;
  }
  //~B() {
  //  cout << "~B()" << endl;
  //}
  //std::vector< B * > memory;
  int n = 2;
};
class A {
 public:
  //A(int _n = 0): n(_n) {}
  //~A() {}
  B b;
  void Get() {
    cout << "Get()" << endl;
  }

  void Get() const {
    cout << "Get() const" << endl;
  }
  int blockSize;
  int index, remains;
};

void func(const A &o = A()) {

}

__declspec(thread) int tls_i = 1;
__declspec(thread) A a;

int main() {

  {
    A a;
    const A *p = &a;
    p->Get();
    a.Get();
    return 0;
  }


  int n = 5;
  vector<thread> ths(n);
  for (int i = 0; i < n; i++) {
    auto Func = [](int x) {
      tls_i = x;
      a.b.n = x;
      #pragma omp critical
      {
        //cout << &tls_i << " " << tls_i << endl;
        cout << this_thread::get_id() << " " << &a.b.n << " " << a.b.n << endl;
      }
      #pragma omp critical
      {
        //cout << &tls_i << " " << tls_i *tls_i << endl;
        cout << this_thread::get_id() << " " << &a.b.n << " " << a.b.n *a.b.n << endl;
      }
    };
    ths[i] = thread(Func, i);
  }
  for (auto &th : ths) {
    th.join();
  }
  return 0;
}