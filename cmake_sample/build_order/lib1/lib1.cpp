#include "lib1.h"
#include <stdio.h>
void print(int n) {
#ifdef MACRO_N
  printf("MACRO_N=%d\n", MACRO_N);
#else
  printf("No MACRO_N defined.\n");
#endif
  printf("n=%d\n", n);
}