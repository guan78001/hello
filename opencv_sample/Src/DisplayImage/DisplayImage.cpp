#include <stdio.h>
#include <opencv2/opencv.hpp>
//#include "vld.h"
#include <iostream>
using namespace std;
using namespace cv;
int main(int argc, char **argv) {
  //int *p = new int[4];
  CvMat *mat = cvCreateMat(4, 4, 0);
  cvReleaseMat(&mat);

  cout << "OpenCV version : " << CV_VERSION << endl;
  cout << "Major version : " << CV_MAJOR_VERSION << endl;
  cout << "Minor version : " << CV_MINOR_VERSION << endl;
  cout << "Subminor version : " << CV_SUBMINOR_VERSION << endl;


  //cv::Mat *p = new cv::Mat();
  //CvMat *p2 = new CvMat;
  if (argc != 2) {
    printf("usage: DisplayImage.out <Image_Path>\n");
    return -1;
  }
  Mat image;
  image = imread(argv[1], 1);
  if (!image.data) {
    printf("No image data \n");
    return -1;
  }
  namedWindow("Display Image", WINDOW_AUTOSIZE);
  imshow("Display Image", image);
  waitKey(0);
  return 0;
}