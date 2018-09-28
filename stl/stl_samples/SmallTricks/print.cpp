#include <stdio.h>
#include <windows.h>
#include <iostream>
using namespace std;

void color() {
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  // you can loop k higher to see more color choices
  for (int k = 1; k < 255; k++) {
    // pick the colorattribute k you want
    SetConsoleTextAttribute(hConsole, k);
    cout << k << " I want to be nice today!" << endl;
  }
  SetConsoleTextAttribute(hConsole, 7);
}

int main() {
  color();
  printf("\n\n");
  for (int i = 0; i < 10; i++) {
    printf("i=%d", i);
    Sleep(100);
    printf("\r");
  }
  return 0;
}