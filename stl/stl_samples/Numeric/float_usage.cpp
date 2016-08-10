#include <float.h>
#include <limits>
#include <iostream>
using namespace std;

int main() {
  double d = std::numeric_limits<double>::quiet_NaN();
  bool res = (d == d);
  cout << d << " d==d is" << res << endl;
}