#pragma once
#ifdef DLL_BOUNDARY_EXPORTS
#define Boundary_API _declspec(dllexport)
#else
#define Boundary_API _declspec(dllimport)
#endif
#define AVOID_DLL_BOUNDARY 0
class Boundary_API Boundary {
 public:
  Boundary();
  ~Boundary();

  // avoid dll boundary problems
#if AVOID_DLL_BOUNDARY
  void *operator new(size_t size);
  void operator delete(void *p);
#endif

};

