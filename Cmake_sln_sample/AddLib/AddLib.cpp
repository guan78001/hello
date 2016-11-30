#include "AddLib.h"
#include <stdio.h>
void AddLib::Print() {
  printf("This is AddLib::Print\n");
}

int AddLib::Add(int a, int b) {
  return a + b;
}