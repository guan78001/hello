#include <iostream>
#include <assert.h>
using namespace std;

class Solution {
 public:
  void convert(int &in, unsigned int &un, unsigned int &flag) {
    if (in >= 0) {
      un = in;
    } else {
      flag += 1;
      if (in == INT_MIN) {
        un = (unsigned int)INT_MAX + 1;
      } else {
        un = -in;
      }
    }
  }
  void tobits(unsigned int val,bool bits[],int &n) {
    n = 0;
    while (val) {
      bits[n++] = val & 1;
      val >>= 1;
    }
  }

  int divide(int dividend, int divisor) {
    if (0 == divisor) return INT_MAX;

    unsigned int a, b, flag = 0;
    convert(dividend, a, flag);
    convert(divisor, b, flag);

    bool bits_a[32] = { 0 };
    int na = 0;
    tobits(a, bits_a, na);
    unsigned int cur = 0;
    unsigned int q = 0;
    for (int i = na - 1; i >= 0; i--) {
      cur <<= 1;
      cur += bits_a[i];
      q <<= 1;
      if (cur>=b) {
        q += 1;
        cur -= b;
      } else {
        q += 0;
      }
    }
    if (flag==1) {
      return -(int)q;
    }
    if (q>INT_MAX) {
      return INT_MAX;
    }
    return q;
  }
};

int main() {
  //unsigned int n = -1;
  //cout << n << endl;
  //return 0;
  Solution sln;
  //int a = sln.divide(INT_MIN, -1);
// int b = sln.divide(10, 2);
  assert(INT_MAX == sln.divide(INT_MIN, -1));
  assert((INT_MIN+1) == sln.divide(INT_MAX, -1));
  assert(0 == sln.divide(10, 11));
  assert(1 == sln.divide(10, 10));
  assert(10 == sln.divide(10, 1));
  assert(5 == sln.divide(10, 2));
  assert(-5 == sln.divide(-10, 2));
  assert(-5 == sln.divide(10, -2));
  //cout << a<<" "<<b << endl;
}
