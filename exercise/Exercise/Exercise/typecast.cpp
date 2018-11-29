#include <iostream>
using namespace std;
class A {
 public:
  void print() {
    cout << __FUNCTION__ << " " << __LINE__ << endl;
  }
};
class B {
 public:
  virtual ~B() {}
};
class D :public B {

};
int main() {
  const A a;
  //const_cast去除const属性, 其他转换编译错误。
  A *pa = const_cast<A*>(&a);

  {
    B *pb = new B;
    D *pd1 = static_cast<D*>(pb);//转换错误时不返回nullptr，不安全
    D *pd2 = dynamic_cast<D*>(pb);//向下类型转换，dynamic_cast要求参数pb是多态类型（有虚函数）。错误转换返回nullptr

    cout << "pd=" << pb << endl;
    cout << "pd1=" << pd1 << endl;
    cout << "pd2=" << pd2 << endl;

    delete pb;
  }

  {
    D *pd = new D;
    //向上类型转换static_cast，dynamic_cast都一样。
    B *pd1 = static_cast<B*>(pd);
    B *pd2 = dynamic_cast<B*>(pd);

    cout << "pd=" << pd << endl;
    cout << "pd1=" << pd1 << endl;
    cout << "pd2=" << pd2 << endl;
    delete pd;
  }

  {
    int *n = new int;
    double *d = reinterpret_cast<double*> (n);
  }
  return 0;
}