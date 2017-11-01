#include "Polynomial.h"

int main() {

  Polynomial<3> p;
  p = p.BSplineComponent(0);
  p.printnl();
  p = p.BSplineComponent(1);
  p.printnl();
  p = p.BSplineComponent(2);
  p.printnl();
  p = p.BSplineComponent(3);
  p.printnl();
  return 0;
}