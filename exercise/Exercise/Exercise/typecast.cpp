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
    D *pd1 = static_cast<D*>(pb);//ת������ʱ������nullptr������ȫ
    D *pd2 = dynamic_cast<D*>(pb);//��������ת����dynamic_castҪ�����pb�Ƕ�̬���ͣ����麯����������ת������nullptr

    cout << "pd=" << pb << endl;
    cout << "pd1=" << pd1 << endl;
    cout << "pd2=" << pd2 << endl;

    delete pb;
  }

  {
    D *pd = new D;
    //��������ת��static_cast��dynamic_cast��һ����
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