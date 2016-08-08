//https://sourcemaking.com/design_patterns/decorator/cpp/1
#include <iostream>
#include <memory>
using namespace std;
class I
{
 public:
  virtual ~I() { cout << "I dtor" << "   " << endl; }
  virtual void do_it() = 0;
};

class A : public I
{
 public:
  ~A()
  {
    cout << "A dtor" << "   ";
  }
  /*virtual*/
  void do_it()
  {
    cout << 'A';
  }
};

class D : public I
{
 public:
  D(I *inner) : m_wrappee(inner)
  {
    //m_wrappee = inner;
  }
  ~D()
  {
    cout << "D dtor" << "   ";
    //delete m_wrappee;
  }
  /*virtual*/
  void do_it()
  {
    m_wrappee->do_it();
  }
 private:
  //I *m_wrappee;
  std::shared_ptr<I> m_wrappee;
};

class X : public D
{
 public:
  X(I *core) : D(core) {}
  ~X()
  {
    cout << "X dtor" << "   ";
  }
  /*virtual*/
  void do_it()
  {
    D::do_it();
    cout << 'X';
  }
};

class Y : public D
{
 public:
  Y(I *core) : D(core) {}
  ~Y()
  {
    cout << "Y dtor" << "   ";
  }
  /*virtual*/
  void do_it()
  {
    D::do_it();
    cout << 'Y';
  }
};

class Z : public D
{
 public:
  Z(I *core) : D(core) {}
  ~Z()
  {
    cout << "Z dtor" << "   ";
  }
  /*virtual*/
  void do_it()
  {
    D::do_it();
    cout << 'Z';
  }
};
#if 0
#define  VIRTUAL virtual
#else
#define  VIRTUAL
#endif
class B1
{
 public:
  B1()
  {
    cout << "B1()" << endl;
  }
  VIRTUAL ~B1()
  {
    cout << "~B1()" << endl;
  }
};
class D1: public B1
{
 public:
  D1()
  {
    cout << "D1()" << endl;
  }
  VIRTUAL ~D1()
  {
    cout << "~D1()" << endl;
  }
};
int main()
{
  //B1 *pb1 = new D1();
  //delete pb1;
  //return 0;

  I *anX = new X(new A);
  I *anXY = new Y(new X(new A));
  I *anXYZ = new Z(new Y(new X(new A)));
  anX->do_it();
  cout << '\n';
  anXY->do_it();
  cout << '\n';
  anXYZ->do_it();
  cout << '\n';
  delete anX;
  delete anXY;
  delete anXYZ;
}