#pragma once

#ifdef ADDDLL_EXPORTS
#define ADDDLL_API __declspec(dllexport)
#else
#define ADDDLL_API __declspec(dllimport)
#endif

class ADDDLL_API AddDLL {
 public:
  void Print();
  int Add(int a, int b);
};