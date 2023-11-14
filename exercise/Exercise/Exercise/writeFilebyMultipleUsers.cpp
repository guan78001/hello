#include <stdio.h>
#include <fstream>
#include <string>
#include <iostream>
#include <memory>
#include <sstream>
using namespace std;
struct FILECloser {
  void operator()(FILE *file) {
    if (file) fclose(file);
  }
};
class A {

};
void test_open_twice() {
  //outer w, inner a , missing inner print
  FILE *fp = fopen("test_open_twice.txt", "w");
  fprintf(fp, "outer1\n");
  fprintf(fp, "outer2\n");
  fflush(fp);
  {
    FILE *fp = fopen("test_open_twice.txt", "a");
    if (fp) {
      fprintf(fp, "inner\n");
      fprintf(fp, "inner\n");
      fflush(fp);
      fclose(fp);
    } else {
      printf("fopen inner failed.\n");
    }
  }
  fprintf(fp, "outer3\n");
  fprintf(fp, "outer4\n");
  fclose(fp);
}
int main() {
  test_open_twice();
//{
//  std::ofstream out2("ios_ate.txt", std::ios::ate);
//  out2 << "This line will be ate to the end of the file";
//  out2.close();
//}
//multiple user use ofstream astd::ios::app to a file, no problem
//std::ofstream os("Exercise_ofstream.txt", std::ios::app);
//while (1) {
//  string str;
//  cin >> str;
//  //os << str << endl;//imply flush immediantely
//  os << str << "\n";//cache then flush
//}
//FILE *fp = fopen("Exercise_FILE.txt", "a");
//unique_ptr<FILE, FILECloser> ptr(fp);
//if (!fp) {
//  printf("open file failed.\n");
//  return 0;
//}
//fprintf(fp, "open file\n");
//while (1) {
//  string str;
//  cin >> str;
//  if (str == "exit" || str == "quit") break;
//  fprintf(fp, "%s\n", str.c_str());

//  //fflush(fp);
//}
//fflush(fp);
//fclose(fp);
}