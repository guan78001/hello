#pragma once

#ifdef DLL_A_EXPORTS
#define DLL_A_API __declspec(dllexport)
#else
#define DLL_A_API __declspec(dllimport)
#endif

class DLL_A_API A {
 public:
  A();
  ~A();
  void func();
};

