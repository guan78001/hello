#include<fstream>

using namespace std;

fstream ofs;


int main() {
  char *p = "124";
  ofs.open("d:\\test.txt", ios::out/* | ios::trunc*/);
  //ofs << "Your content here";
  ofs.close(); //Using microsoft incremental linker version 14
}