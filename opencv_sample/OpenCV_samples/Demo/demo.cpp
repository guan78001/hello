#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
using namespace std;
using namespace cv;
int main() {
  printf("sizeof(void*)=%d\n", sizeof(void*));
  Mat M(3, 2, CV_8UC3, Scalar(0, 0, 255));
  cout << "M = " << endl << " " << M << endl << endl;
}
