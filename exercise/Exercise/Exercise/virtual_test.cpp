#include <iostream>
using namespace std;

class Base1{
public:
	virtual void foo1() {};
};

class Base2{
public:
	virtual void foo2() {};
};

class MI : public Base1, public Base2{
public:
	virtual void foo1() { cout << "MI::foo1" << endl; }
	virtual void foo2() { cout << "MI::foo2" << endl; }
};

class Base
{
public:
	Base(){ cout << "Base called..." << endl; }
	void print(){ cout << "Base print..." << endl; }
};
int main(){
	MI oMI;

	Base1* pB1 = &oMI;
	pB1->foo1();

	{
		Base2* pB2 = dynamic_cast<Base2*>(pB1); // ָ��ǿ��ת����û��ƫ��
		pB2->foo2();
	}
	
	Base2* pB2 = (Base2*)(pB1); // ָ��ǿ��ת����û��ƫ��
	pB2->foo2();

	pB2 = dynamic_cast<Base2*>(pB1); // ָ�붯̬ת��,dynamic_cast����ƫ��
	pB2->foo2();

	return 0;
}