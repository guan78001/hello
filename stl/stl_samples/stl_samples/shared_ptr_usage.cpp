#include <memory>
#include <iostream>
using namespace std;

struct Item
{
  Item(int n = 0) : value(n)
  {
    //cout << "Item()" << endl;
  }
  ~Item()
  {
    //cout << "~Item()" << endl;
  }
  int value;
};

struct Container
{
  void set(int n)
  {
    if (!ptr)
    {
      ptr.reset(new Item(n));
    }

    if (!ptr_reset)
    {
      ptr_reset = ptr;
    }

    ptr_restore = ptr;

    ptr.reset(new Item(n));

  }
  void reset()
  {
    if (ptr_reset)
    {
      ptr = ptr_reset;
      ptr_reset.reset();
      ptr_restore.reset();
    }
  }
  void restore()
  {
    if (ptr_restore)
    {
      ptr = ptr_restore;
      ptr_restore.reset();
    }
  }
  int get()
  {
    if (ptr)
    {
      return ptr->value;
    }
  }
  std::shared_ptr<Item> ptr;
  std::shared_ptr<Item> ptr_reset;
  std::shared_ptr<Item> ptr_restore;
};

int main()
{
  Container c0;
  c0.set(1);
  Container c1 = c0;
  Container c2 = c1;
  cout << c0.get() << " " << c1.get() << "  " << c2.get() << endl;
  c0.set(2);
  cout << c0.get() << " " << c1.get() << "  " << c2.get() << endl;
  c0.set(3);
  cout << c0.get() << " " << c1.get() << "  " << c2.get() << endl;
  c1.set(4);
  cout << c0.get() << " " << c1.get() << "  " << c2.get() << endl;
  c0.restore();
  cout << c0.get() << " " << c1.get() << "  " << c2.get() << endl;
  c0.restore();
  cout << c0.get() << " " << c1.get() << "  " << c2.get() << endl;
  c1.reset();
  cout << c0.get() << " " << c1.get() << "  " << c2.get() << endl;
  c0.reset();
  cout << c0.get() << " " << c1.get() << "  " << c2.get() << endl;
  return 0;

  return 0;
}