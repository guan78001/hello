#include <vector>
#include <iostream>
using namespace std;

class A {
#define PRINT_FUNC cout << __FUNCTION__ << " " << __LINE__<<endl;
 public:
  A() { PRINT_FUNC };
  ~A() { PRINT_FUNC };
  A(const A &) { PRINT_FUNC };
  A(const A &&) { PRINT_FUNC };
  void print() {
    PRINT_FUNC;
  }
};
int main() {
  //A a;
  //A b = a;
  //A c1(A());// warning C4930: 'A c1(A (__cdecl *)(void))': prototyped function not called (was a variable definition intended?)
  A c((A()));//
  //A c2(move(A()));
  //A c;
  //c.print();
  //typedef int _Ty;
  //auto _Count = 4;
  //void *p = operator new (_Count * sizeof(_Ty));
  //int *p1 = reinterpret_cast<int *>(p);
  //for (auto i = 0; i < _Count; i++) {
  //  cout << p1[i] << endl;
  //}

  //int *p2 = new int[_Count];
  //for (auto i = 0; i < _Count; i++) {
  //  cout << p2[i] << endl;
  //}

  //std::vector< std::vector<char> > m_status;
  //m_status.push_back(std::vector<char>(4, 0));
  return 0;
}