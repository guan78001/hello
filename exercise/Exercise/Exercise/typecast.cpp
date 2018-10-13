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
  //const_castȥ��const����, ����ת���������
  A *pa = const_cast<A*>(&a);

  {
    B *pb = new B;
    D *pd1 = static_cast<D*>(pb);
    D *pd2 = dynamic_cast<D*>(pb);//��������ת����dynamic_castҪ�����pb�Ƕ�̬���ͣ����麯����

    cout << "pd=" << pb << endl;
    cout << "pd1=" << pd1 << endl;
    cout << "pd2=" << pd2 << endl;

    delete pb;
  }

  {
    D *pd = new D;
    B *pd1 = static_cast<B*>(pd);
    B *pd2 = dynamic_cast<B*>(pd);

    cout << "pd=" << pd << endl;
    cout << "pd1=" << pd1 << endl;
    cout << "pd2=" << pd2 << endl;
    delete pd;
  }
  return 0;
}