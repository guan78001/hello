#include "lib1.h"
#include <stdio.h>
#include "dll1.h"
#include <vector>

void disable_warning() {
  std::vector<int>v;
  int n = v.size();//warning C4267
}
void f1(void) throw(int) {}   // C4290
void input() {
  int n;
  scanf("%d", &n);//warning C4996
}
int main() {
  print(1);
  //TestOpenMP(8);

  return 0;
}