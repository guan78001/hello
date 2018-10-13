#include "A.h"
#include <iostream>
using namespace std;

A::A() {
}


A::~A() {
}

void A::func() {
  cout << __FUNCTION__ << endl;
}
