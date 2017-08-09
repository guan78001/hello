// A simple program that computes the square root of a number
#include <stdio.h>
#include "AddDLL/AddDLL.h"
#include "AddLib/AddLib.h"
#include <windows.h>
int main (int argc, char *argv[]) {
  printf("sizeof(void*)=%d\n", sizeof(void *));

  AddDLL obj;
  obj.Print();

  AddLib obj2;
  obj2.Print();

  char path[512];
  GetCurrentDirectory(512, path);
  printf("path=%s\n", path);
  return 0;
}