// range heap example
#include <iostream>     // std::cout
#include <algorithm>    // std::make_heap, std::pop_heap, std::push_heap, std::sort_heap
#include <vector>       // std::vector
#include <functional>
void print(std::vector<int> &v) {
  std::cout << "vector data: ";
  for (int i : v) {
    std::cout << i << " ";
  }
  std::cout << std::endl;
}

void bigheap() {
  int myints[] = { 10, 20, 30, 5, 15 };
  std::vector<int> v(myints, myints + 5);

  std::make_heap(v.begin(), v.end());
  std::cout << "initial max heap   : " << v.front() << '\n';
  print(v);

  std::pop_heap(v.begin(), v.end());
  v.pop_back();
  std::cout << "max heap after pop : " << v.front() << '\n';
  print(v);

  v.push_back(99); std::push_heap(v.begin(), v.end());
  std::cout << "max heap after push: " << v.front() << '\n';

  std::sort_heap(v.begin(), v.end());

  std::cout << "final sorted range :";
  for (unsigned i = 0; i < v.size(); i++)
    std::cout << ' ' << v[i];

  std::cout << '\n';
}

bool isgreater(int a, int b) {
  return a > b;
}
void smallheap() {
  std::cout << "---------small heap-------------" << std::endl;
  int myints[] = { 10, 20, 30, 5, 15 };
  std::vector<int> v(myints, myints + 5);

  //std::make_heap(v.begin(), v.end(), std::greater<int>());
  std::make_heap(v.begin(), v.end(), isgreater);
  std::cout << "initial max heap   : " << v.front() << '\n';
  print(v);

  std::pop_heap(v.begin(), v.end(), isgreater);
  v.pop_back();
  std::cout << "max heap after pop : " << v.front() << '\n';
  print(v);

  v.push_back(99);
  std::push_heap(v.begin(), v.end(), isgreater);
  std::cout << "max heap after push: " << v.front() << '\n';

  std::sort_heap(v.begin(), v.end(), isgreater);

  std::cout << "final sorted range :";
  for (unsigned i = 0; i < v.size(); i++)
    std::cout << ' ' << v[i];

  std::cout << '\n';
}

int main() {
  bigheap();
  smallheap();

  return 0;
}