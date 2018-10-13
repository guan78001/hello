#include <vector>
#include <iostream>
using namespace std;

void print(const vector<int> &v, char *str = "") {
  cout << str << endl;
  for (auto i : v) {
    cout << i << " ";
  }
  cout << endl;

}

template <class ForwardIterator>
void rotate2(ForwardIterator first, ForwardIterator middle,
             ForwardIterator last) {
  ForwardIterator next = middle;
  while (first != next) {
    swap(*first++, *next++);
    if (next == last) next = middle;
    else if (first == middle) middle = next;
  }
}

template <class ForwardIterator>
void rotate3(ForwardIterator first, ForwardIterator middle,
             ForwardIterator last) {
  ForwardIterator next = middle;
  while (first != next) {
    swap(*first++, *next++);
    if (next == last) next = middle;
    else if (first == middle) middle = next;
  }
}
void rotate1() {
  vector<int> v = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  print(v, "init");
  rotate(begin(v), begin(v) + 3, end(v));
  print(v, "after rotate");
}
int main() {
  rotate1();
  return 0;
}