#include <iostream>
#include <tuple>
#include <map>
using namespace std;

void test1() {
  map<int, int> m;
  m[1] = 1;
  map<int, int>::iterator iter;
  int i, j;
  tie(i, j);
  tuple<int, int> t;
  t = make_tuple(1, 2);
  tie(i, j) = t;
  cout << i << " " << j << endl;
  cout << std::get<1>(t) << " " << std::get<0>(t) << endl;

}
std::tuple<int, int> foo_tuple() {
  //return{ 1, -1 };  // Error until C++17
  return std::make_tuple(1, -1); // Always works
}
int main() {
  test1();
  return 0;
}