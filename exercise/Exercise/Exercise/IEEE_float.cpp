#include <iostream>
using namespace std;

//https://www.h-schmidt.net/FloatConverter/IEEE754.html
//Value=sign*2^exponent*mantissa
//Exponent:[2^(-126)~2^127] value-127
//mantissa:[1:2-2^(-23)]
union IEEE_float {
  float f;
  unsigned int x;
  struct {
    unsigned int mantissa : 23;
    unsigned int exponent : 8;
    unsigned int sign : 1;
  };
  void print() {
    cout << "IEEE_float:" << endl;
    cout << "	f=" << f << endl;
    cout << "	x=" << hex << x << endl;
    cout << "	sign=" << sign << endl;
    cout << "	exponent=" << hex << exponent << endl;
    cout << "	mantissa=" << hex << mantissa << endl;
  }
};

void test() {
  IEEE_float v;
  v.f = 3;
  v.print();
}

union IEEE_N {
  unsigned int x;
  struct {
    unsigned int d : 8;
    unsigned int c : 8;
    unsigned int b : 8;
    unsigned int a : 8;
  };
  void print() {
    printf("IEEE_N: %0x %x %2x %0x\n", a, b, c, d);
  }
};

void endian() {
  unsigned int x = 0x12345678;
  printf("x=%x\n", x);
  unsigned char *pC = (unsigned char *)&x;
  //little endian: 0x78,0x56,0x34,0x12
  printf("byte order: %x %x %x %x\n", pC[0], pC[1], pC[2], pC[3]);

  IEEE_N n;
  n.x = x;
  //printf("%x %x %x %x\n", n.a, n.b, n.c, n.d);
  n.print();
}
int main() {
  endian();
  test();
  return 0;
}