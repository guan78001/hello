#define  _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <memory>
#include <cstdio>
#include <fstream>
#include <cassert>
#include <functional>

struct B {
  virtual void bar() { std::cout << "B::bar\n"; }
  virtual ~B() = default;
};
struct D : B {
  D() { std::cout << "D::D\n"; }
  ~D() { std::cout << "D::~D\n"; }
  void bar() override { std::cout << "D::bar\n"; }
};

// a function consuming a unique_ptr can take it by value or by rvalue reference
std::unique_ptr<D> pass_through(std::unique_ptr<D> p) {
  p->bar();
  return p;
}

void close_file(std::FILE *fp) { std::fclose(fp); }

void myclose(FILE *fp) {
  std::cout << __FUNCTION__ << std::endl;
}
void test1() {
  //std::unique_ptr<std::FILE, decltype(&close_file)> fp(std::fopen("demo.txt", "r"),
  //    &close_file);
  std::unique_ptr<std::FILE, decltype(&std::fclose)> fp(std::fopen("demo.txt", "r"),
      &std::fclose);
  if (fp) // fopen could have failed; in which case fp holds a null pointer
    std::cout << (char)std::fgetc(fp.get()) << '\n';
// fclose() called here, but only if FILE* is not a null pointer
}
void test2() {
  std::unique_ptr<std::FILE, decltype(&myclose)> fp(std::fopen("demo.txt", "r"), &myclose);
}
void test3() {
  auto Func = [](FILE * fp) { std::cout << __FUNCTION__ << std::endl; fclose(fp); };
  std::unique_ptr<std::FILE, decltype(Func)> fp(std::fopen("demo.txt", "r"), Func);
}
void test4() {
  static auto fileDeleter = [](FILE * f) { fclose(f); };
  //auto close file without explicit fclsoe.
  typedef std::unique_ptr<FILE, decltype(fileDeleter)> SafeFileOpen;
  SafeFileOpen sfo(std::fopen("demo.txt", "r"), fileDeleter);
}
int main() {
  test4();
  return 0;

  std::cout << "unique ownership semantics demo\n";
  {
    auto p = std::make_unique<D>(); // p is a unique_ptr that owns a D
    auto q = pass_through(std::move(p));
    assert(!p); // now p owns nothing and holds a null pointer
    q->bar();   // and q owns the D object
  } // ~D called here

  std::cout << "Runtime polymorphism demo\n";
  {
    std::unique_ptr<B> p = std::make_unique<D>(); // p is a unique_ptr that owns a D
    // as a pointer to base
    p->bar(); // virtual dispatch

    std::vector<std::unique_ptr<B>> v;  // unique_ptr can be stored in a container
    v.push_back(std::make_unique<D>());
    v.push_back(std::move(p));
    v.emplace_back(new D);
    for (auto &p : v) p->bar(); // virtual dispatch
  } // ~D called 3 times

  std::cout << "Custom deleter demo\n";
  std::ofstream("demo.txt") << 'x'; // prepare the file to read
  {
    std::unique_ptr<std::FILE, decltype(&close_file)> fp(std::fopen("demo.txt", "r"),
        &close_file);
    if (fp) // fopen could have failed; in which case fp holds a null pointer
      std::cout << (char)std::fgetc(fp.get()) << '\n';
  } // fclose() called here, but only if FILE* is not a null pointer
  // (that is, if fopen succeeded)

  std::cout << "Custom lambda-expression deleter demo\n";
  {
    std::unique_ptr<D, std::function<void(D *)>> p(new D, [](D * ptr) {
      std::cout << "destroying from a custom deleter...\n";
      delete ptr;
    });  // p owns D
    p->bar();
  } // the lambda above is called and D is destroyed

  std::cout << "Array form of unique_ptr demo\n";
  {
    std::unique_ptr<D[]> p{ new D[3] };
  } // calls ~D 3 times
}