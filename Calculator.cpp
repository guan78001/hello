#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <sstream>
using namespace std;

struct Item
{
  union
  {
    double value;
    char op;
  };
  bool is_op;

  Item() { value = 0; is_op = true; }
  Item(double _val) { value = _val; is_op = false; }
  Item(char _op)  { op = _op; is_op = true; }
};
std::string OperList = { "(+-*/)" };
std::map<std::pair<char, char>, int> oper_oder;
void init_oper_order(std::map<std::pair<char, char>, int> &oper_oder)
{
  auto def = [&](char a, char b, int order) {oper_oder[std::make_pair(a, b)] = order; };

  def('(', '(', 0);
  def('(', '+', 0);
  def('(', '-', 0);
  def('(', '*', 0);
  def('(', '/', 0);
  def('(', ')', 1);

  def('+', '(', 0);
  def('+', '+', 1);
  def('+', '-', 1);
  def('+', '*', 0);
  def('+', '/', 0);
  def('+', ')', 1);

  def('-', '(', 0);
  def('-', '+', 1);
  def('-', '-', 1);
  def('-', '*', 0);
  def('-', '/', 0);
  def('-', ')', 1);

  def('*', '(', 0);
  def('*', '+', 1);
  def('*', '-', 1);
  def('*', '*', 0);
  def('*', '/', 0);
  def('*', ')', 1);

  def('/', '(', 0);
  def('/', '+', 1);
  def('/', '-', 1);
  def('/', '*', 0);
  def('/', '/', 0);
  def('/', ')', 1);

  def(')', '(', -1);
  def(')', '+', 1);
  def(')', '-', 1);
  def(')', '*', 1);
  def(')', '/', 1);
  def(')', ')', 1);
}
int cmpOrder2(char a, char b)
{
  int arr[6][6] =
  {
    //   { (, + ,-, *, /, ) }
    /*(*/{ 0, 0, 0, 0, 0, 1 },
    /*+*/{ 0, 1, 1, 0, 0, 1 },
    /*-*/{ 0, 1, 1, 0, 0, 1 },
    /***/{ 0, 1, 1, 0, 0, 1 },
    /*/*/{ 0, 1, 1, 0, 0, 1 },
    /*)*/{ 0, 0, 0, 0, 0, 1 },
  };
  std::string str = { "(+-*/)" };
  size_t ai = str.find(a);
  size_t bi = str.find(b);
  if (ai == str.npos || bi == str.npos)
  {
    return -1;
  }
  return arr[ai][bi];
}
int cmpOrder(char a, char b)
{
  return cmpOrder2(a, b);
  auto val = std::make_pair(a, b);
  auto iter = oper_oder.find(val);
  if (iter != oper_oder.end())
  {
    return iter->second;
  }
  return -1;
}

bool isOp(char c)
{
  return OperList.find(c) != OperList.npos;
}
double calc2(double a, char op, double b)
{
  switch (op)
  {
    case '+':
      return a + b;
    case '-':
      return a - b;
    case '*':
      return a * b;
    case '/':
      return a / b;
    default:
      cout << "Invalid operator " << op << endl;
      return -1;
  }
}
double calc(std::vector<Item> &items)
{
  if (items.empty()) return 0;
  if (items.size() == 1) return items[0].value;
  //if (items.size() == 2)
  //{
  //  if (items[0].op == '-')
  //  {
  //    return -items[1].value;
  //  }
  //  cout << "Invalid expression" << endl;
  //  return 0;
  //}
  if (items.size() == 3 && items[1].is_op)
  {
    return calc2(items[0].value, items[1].op, items[2].value);
  }
  if (items.size() == 3 && items[0].op == '(')
  {
    return items[1].value;
  }
  while (items.size() > 3)
  {
    std::vector<int> opers;
    for (int i = 0; i < items.size(); i++)
    {
      if (items[i].is_op)
      {
        opers.push_back(i);
        if (opers.size() > 1)
        {
          const int cmp = cmpOrder(items[opers[opers.size() - 2]].op, items[opers[opers.size() - 1]].op);
          if (cmp == 1)
          {
            break;
          }
          else if (cmp == -1)
          {
            cout << "Invalid expression1" << endl;
            return -1;
          }
        }
      }
    }
    if (opers.empty())
    {
      cout << "Invalid expression2" << endl;
      return -2;
    }
    int idx = opers[opers.size() - 2];
    if (items[idx].op == '(')
    {
      items[idx] = items[idx + 1];
      items.erase(items.begin() + idx + 1, items.begin() + idx + 3);
    }
    else
    {
      double a = items[idx - 1].value;
      double b = items[idx + 1].value;
      double c = calc2(a, items[idx].op, b);
      items[idx - 1] = Item(c);
      items.erase(items.begin() + idx, items.begin() + idx + 2);
    }
  }
  return calc(items);
  return 0;
}
void convert(const std::string &str, std::vector<Item> &items)
{
  double value = 0;
  bool isData = false;
  bool hasFraction = false;
  double ratio = 1.0;
  for (int i = 0; i < str.size(); i++)
  {
    if (str[i] == '.')
    {
      hasFraction = true;
      ratio = 1.0;
    }
    else if (str[i] >= '0' && str[i] <= '9')
    {
      isData = true;
      if (hasFraction)
      {
        ratio *= 0.1;
        value = value + (str[i] - '0') * ratio;
      }
      else
      {
        value = value * 10 + str[i] - '0';
      }
    }
    else
    {
      if (isData)
      {
        items.push_back(Item(value));
        isData = false;
        hasFraction = false;
        value = 0;
      }
      if (isOp(str[i]))
      {
        items.push_back(Item(str[i]));
      }
    }
  }
  if (isData)
  {
    items.push_back(Item(value));
    isData = false;
    value = 0;
  }
}
double calc(const std::string &str)
{
  std::vector<Item> items;
  convert(str, items);
  if (items[0].op != '(')
  {
    items.insert(items.begin(), Item('('));
    items.push_back(Item(')'));
  }
  int len = items.size();
  for (int i = 1; i < len; i++)
  {
    if (items[i].op == '-' && items[i - 1].op == '(')
    {
      items.insert(items.begin() + i, Item(0.0));
      len++;
      i++;
    }
  }
  return calc(items);
}

#define TEST(str)  cout << str<<"="<<calc(str) << endl;;
int main()
{
  init_oper_order(oper_oder);
  TEST("-12.1");
  TEST("-12+2.2");
  TEST("(-1.02)+(1.01)+(1+2*(-1+2*1))");
  //TEST("12");
  //TEST("(12)");
  //TEST("12+23");
  //TEST("12*3");
  //TEST("(12*3)");
  //TEST("(5*(1+1))");
  //TEST("((5)*(2+2)*(1+1))");
  //TEST("(1-2+12*3)");
  //TEST("(1-2*2+5/10+3+12*3)");
  //TEST("(2+2*3)");
  return 0;
}
