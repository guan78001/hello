#include <iostream>
using namespace std;
#include "..\Dll_A\A.h"
//#pragma comment(lib,"D:\\github\\hello\\performance\\PerfromanceTest\\Release\\Dll_A.lib")
#pragma comment(lib,"D:\\github\\hello\\performance\\PerfromanceTest\\Release\\Dll_G.lib")
int main() {
  A a;
  a.func();
  return 0;
}