#include "Boundary.h"
#include <stdio.h>
#include <memory>
Boundary::Boundary() {
  printf("Boundary()\n");
}


Boundary::~Boundary() {
  printf("~Boundary()\n");
}

#if AVOID_DLL_BOUNDARY
void *Boundary::operator new(size_t nSize) {
  printf("\noperator new\n");
  void *p = malloc(nSize);
  return p;
}
void Boundary::operator delete(void *p) {
  printf("\noperator delete\n");
  free(p);
}
#endif