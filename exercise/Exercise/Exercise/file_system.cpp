#include <filesystem>
#include <string>
#include <sstream>
#include <omp.h>
#include <iostream>
namespace fs = std::experimental::filesystem;
using namespace std;

template<class T>
void TestTime(const string &str, T t) {
  double t0 = omp_get_wtime();
  t();
  double t1 = omp_get_wtime();
  cout << str << " used time: " << t1 - t0 << "s" << endl;
}
void test_copy() {
  std::error_code error;
  string srcFolder = "c:/temp/1";
  string dstFolder = "c:/temp/2";
  string filename = "f.txt";


  TestTime("copy_file", [&]() {
    for (int i = 0; i < 1000; i++) {
      fs::copy_file(srcFolder + "/" + filename, dstFolder + "/" + filename, fs::copy_options::overwrite_existing, error);
    }
  });
  //bool res = fs::copy_file(srcFolder + "/" + filename, dstFolder, error);
  //bool res = fs::copy_file(srcFolder + "/" + filename, dstFolder + "/" + filename, error);
  //printf("copy_file res=%d,error=%s\n", res, error.message().c_str());

  string cmd = "copy c:\\temp\\1\\f.txt c:\\temp\\2 /Y >nul"; //only support \\ format in system
  //int ret = system(cmd.c_str());
  TestTime("xcopy", [&]() {
    for (int i = 0; i < 10; i++) {
      system(cmd.c_str());
    }
  });
}

int main() {
  test_copy();
  return 0;
}