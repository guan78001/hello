#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <functional>
#include <vector>
using namespace std;

#define VAR_TABLE \
VAR(int, x, 0)/*comments*/\
VAR(int, y, 1)\
VAR(int, z, 2)\
//end

struct Param {
#define VAR(type,name,value) type name = value;
  VAR_TABLE;
#undef VAR
  int xx;/*test comments*/int yy;/*test comments*/
  void read() {
    std::ifstream in("c:/temp/in.txt");
    auto Func = [&]() {};
    std::map < std::string, function < void()>  > func_table{
#define  VAR(type, name, value) {#name, [&]()->void { in >> name; }},
      VAR_TABLE
#undef  VAR
    };
    while (in) {
      string s;
      in >> s;
      auto it = func_table.find(s);
      if (it != func_table.end()) {
        it->second();
      }
    }
  }
  void write() {
    cout << "write:\n";
#define VAR(type,name,value) cout<<#name<<"="<<name<<endl;
    VAR_TABLE
#undef VAR
  }
};

int main() {
#define VAR(type,name,value) type name = value;
  VAR_TABLE;
  cout << "default value:\n";
  cout << x << " " << y << " " << z << endl;
#undef  VAR

//
//  cout << "input:\n";
//#define VAR(type,name,value) cout<<#name<<"="; cin>>name;
//  VAR_TABLE;
//
//  cout << "output:\n";
//#undef VAR
//#define VAR(type,name,value) cout<<#name<<"="<<name<<endl;
//  VAR_TABLE;
//
#undef  VAR_TABLE

  Param obj;
  obj.x = 4;
  obj.write();

  // read from file
  cout << "read from file:\n";
  obj.read();
  cout << obj.x << " " << obj.y << " " << obj.z << endl;
  obj.write();
  obj.xx;
  obj.yy;
#define V1 int v1;//v1
  V1
  v1 = 0;
  return 0;
}