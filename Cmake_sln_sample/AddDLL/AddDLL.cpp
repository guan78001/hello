#include "AddDLL.h"
#include "../AddLib/AddLib.h"
#include "../AddLib2/AddLib2.h"
#include <stdio.h>
void AddDLL::Print() {
  printf("This is AddDLL::Print\n");
  AddLib obj1;
  obj1.Print();

  AddLib2 obj2;
  obj2.Print();
}

int AddDLL::Add(int a, int b) {
  AddLib obj;
  return obj.Add(1, 2);
}