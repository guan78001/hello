#include <memory>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <exception>
#include <omp.h>
using namespace std;

extern "C" void func(int x) {
  cout << __FUNCDNAME__ << endl;
  cout << __FUNCTION__ << endl;
  cout << __FUNCSIG__ << endl;
}

void funcpp(int x) {
  cout << __FUNCDNAME__ << endl;
  cout << __FUNCTION__ << endl;
  cout << __FUNCSIG__ << endl;
}
class A {
 public:
  A() { cout << __FUNCTION__ << endl; }
  A(const A &rhs) { cout << "A(const A &rhs)  " << __FUNCDNAME__ << " " << __FUNCSIG__ << endl; }
  A &operator=(const A &rhs) { cout << __FUNCTION__ << endl; }
  A &operator=(A &rhs) { cout << __FUNCTION__ << endl; }
  ~A() { cout << __FUNCTION__ << endl; }
};

class MyClass {
 private:
  int counter;
  mutable int x;
 public:
  void Foo() {
    std::cout << "Foo" << std::endl;
  }

  void Foo() const {
    std::cout << "Foo const" << std::endl;
    x = 1;
  }

  virtual ~MyClass() {
    std::cout << "~MyClass()" << std::endl;
  }
};

class MyClass2: public MyClass {
 private:
  int counter;
  mutable int x;
 public:
  //int Foo() {
  //  std::cout << "Foo" << std::endl;
  //  return 0;
  //}

  int Foo(int x) {
    std::cout << "Foo" << std::endl;
    return 0;
  }
  //void Foo() const {
  //  std::cout << "Foo const" << std::endl;
  //  x = 1;
  //}

  virtual ~MyClass2() {
    std::cout << "~MyClass2()" << std::endl;
  }
};
struct Point {
  float x, y;
  Point(double _x = 0, double _y = 0) : x(_x), y(_y) {}
  bool operator<(const Point &rhs) const {
    return x < rhs.x || (x == rhs.x && y < rhs.y);
  }
};
struct A1 {
  int a;
  virtual ~A1() {}
};
struct B {
  int b;
  virtual ~B() {}
};
struct C: public A1, B {
  int c;
  virtual ~C() {}
};
class E {
 public:
  E(int e) {
    m_e = e;
  }
  char c;
  int m_e;
  double b;
};
void Func1(E e) {
  cout << "Func1(E e),sizeof(E)=" << sizeof(e) << endl;
}
//void Func1(int e) {
//  cout << "Func1(int e)" << endl;
//}
void print(char *str) {
  printf("%s\n", str);
}
int main() {
#define MYPRINT(str) print(str);
  MYPRINT("112");
  MYPRINT("1231");
  {
    #pragma omp parallel for

    for (int i = 0; i < 8; i++) {
      printf("i=%d th=%d\n", i, omp_get_thread_num());
    }
    return 0;
  }

  {
    Func1(1);
    Func1(E(2));
    return 0;
    //div 0 is not exception
    //try {
    //  int x = 0;
    //  int y = 1 / x;
    //} catch (std::logic_error e) {

    //  cerr << e.what();
    //} catch (...) {
    //  printf("exception\n");
    //}
    C *pc = new C;
    A1 *pa = pc;
    B *pb = (B *)pc;
    pc = 0;
    B *pdb = dynamic_cast<B *>(pc);
    B *psb = static_cast<B *>(pc);
    //C *pdc = dynamic_cast<B *>pb;//error
    C *psc = static_cast<C *>(pc);
    pa = 0;
    C &prdc = dynamic_cast<C &>(*pa);
    C &prsc = static_cast<C &>(*pa);
    cout << prdc.a;
    cout << prsc.a;
    cout << pa << " " << pb << " " << pc << endl;
    return 0;
  }
  {
    vector<Point> v;
    sort(v.begin(), v.end());
    map<Point, int> m;
    Point p1(1, 1);
    m[p1] = 0;
    Point p2(2, 2);
    m[p2] = 1;
    Point p3(1, 0);
    m[p3] = 1;
    cout << m.size();
  }
  //func(1);
  //funcpp(1);
  //A a;
  //A b = a;
  //return 0;
  {
    MyClass2 mc;
  }
  cout << endl;
  {
    MyClass cc;
    const MyClass &ccc = cc;
    cc.Foo();
    ccc.Foo();
    MyClass2 mc;
  }

  if (1) {
    cout << "virtual destructor" << endl;
    MyClass2 *p2 = new MyClass2();
    MyClass *p = p2;
    MyClass2 *xp2 = static_cast<MyClass2 *>(p);
    MyClass2 *p21 = dynamic_cast<MyClass2 *>(p);
    MyClass *p02 = dynamic_cast<MyClass *>(p2);
    cout << p << " " << p2 << endl;
    delete p;
  }
}


//int main() {
//  std::auto_ptr<int> p(new int[4]);
//  return 0;
//}
