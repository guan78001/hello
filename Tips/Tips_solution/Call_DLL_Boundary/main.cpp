#include "../DLL_Boundary/Boundary.h"
#include <stdio.h>
int main() {
  printf("new Boundary\n");
  Boundary *obj = new Boundary;
  printf("delete obj\n");
  delete obj;

  return 0;
}