////https://sourcemaking.com/design_patterns/composite/cpp/2
#include <iostream>
#include <vector>
using namespace std;

class Componete {
 public:
  virtual void traverse() = 0;
};

class Primitive : public Componete {
  int value;
 public:
  Primitive(int val) {
    value = val;
  }
  void traverse() {
    cout << value << " ";
  }
};

class Composite : public Componete {
  vector<Componete *>children;
  int value;
 public:
  Composite(int val) {
    value = val;
  }
  void add(Componete *c) {
    children.push_back(c);
  }
  void traverse() {
    cout << value << " ";
    for (size_t i = 0; i < children.size(); i++) {
      children[i]->traverse();
    }
  }
};

class Row : public Composite {
 public:
  Row(int val) : Composite(val) {}

  void traverse() {
    cout << "Row";
    Composite::traverse();
  }
};

class Column : public Composite {
 public:
  Column(int val) : Composite(val) {}
  void traverse() {
    cout << "Col";
    Composite::traverse();
  }
};

int main() {
  Row first(1);                 // Row1
  Column second(2);             //   |
  Column third(3);              //   +-- Col2
  Row fourth(4);                //   |     |
  Row fifth(5);                 //   |     +-- 7
  first.add(&second);           //   +-- Col3
  first.add(&third);            //   |     |
  third.add(&fourth);           //   |     +-- Row4
  third.add(&fifth);            //   |     |     |
  first.add(&Primitive(6));     //   |     |     +-- 9
  second.add(&Primitive(7));    //   |     +-- Row5
  third.add(&Primitive(8));     //   |     |     |
  fourth.add(&Primitive(9));    //   |     |     +-- 10
  fifth.add(&Primitive(10));    //   |     +-- 8
  first.traverse();             //   +-- 6
  cout << '\n';
  return 0;
}