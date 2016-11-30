#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
using namespace std;
using namespace cv;
int main() {
  printf("sizeof(void*)=%d\n", sizeof(void*));
  Mat M(3, 2, CV_8UC3, Scalar(0, 0, 255));
  cout << "M = " << endl << " " << M << endl << endl;

  M.create(4, 4, CV_8UC(2));
  cout << "M = " << endl << " " << M << endl << endl;

  Mat E = Mat::eye(4, 4, CV_64F);
  cout << "E = " << endl << " " << E << endl << endl;

  Mat O = Mat::ones(2, 2, CV_32F);
  cout << "O = " << endl << " " << O << endl << endl;

  Mat Z = Mat::zeros(3, 3, CV_8UC1);
  cout << "Z = " << endl << " " << Z << endl << endl;

  Mat C = (Mat_<double>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
  cout << "C = " << endl << " " << C << endl << endl;
  //int sz[3] = { 2, 2, 2 };
  //Mat L(3, sz, CV_8UC(1), Scalar::all(0));
  //cout << L << endl;
}
