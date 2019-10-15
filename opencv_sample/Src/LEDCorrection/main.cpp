#include "LedLightCorrection.h"
#include <string>

#include <opencv2/opencv.hpp>
using namespace cv;

char *filename[3] = {
  R"(C:\csd\matlab_script\ColorCorrection\DebugImages\View16_0.jpg)",
  R"(C:\csd\matlab_script\ColorCorrection\DebugImages\View16_1.jpg)",
  R"(C:\csd\matlab_script\ColorCorrection\DebugImages\View9_2.jpg)"
};

void MergeImage(const Mat mat[3], Mat &RGB) {
  vector<Mat> mat3(3);
  for (int i = 0; i < 3; i++ ) {
    vector<Mat> imageRGB;
    split(mat[i], imageRGB);
    mat3[2 - i] = imageRGB[i];
  }
  merge(mat3, RGB);
}

int main(int argc, char *argv[]) {
  if (argc < 4) {
    printf("Usage: \n\tLEDCorrection.exe red.jpg green.jpg blue.jpg\n");
  } else {
    for (int i = 0; i < 3; i++) {
      filename[i] = argv[i + 1];
    }
  }
  LedLightCorrection led;
  LightInfo lightinfos[3];
  Mat mat[4];
  RectInfo rects[3];
  char *name[3] = { "R", "G", "B" };
  cv::Scalar clr[3] = { cv::Scalar(0, 0, 255), cv::Scalar(0, 255, 0), cv::Scalar(255, 0, 0)};//BRG order
  for (int i = 0; i < 3; i++) {
    mat[i] = imread(filename[i]);
    rects[i].rectSize = 140;
    led.GetMaxRect(mat[i], rects[i]);
    led.GetLightInfo(mat[i], lightinfos[i]);
  }
  MergeImage(mat, mat[3]);
  int thickness = 2;
  for (int i = 0; i < 3; i++) {
    cv::rectangle(mat[i], cv::Point(rects[i].x, rects[i].y), cv::Point(rects[i].x + rects[i].rectSize, rects[i].y + rects[i].rectSize), clr[i], thickness);
    cv::rectangle(mat[3], cv::Point(rects[i].x, rects[i].y), cv::Point(rects[i].x + rects[i].rectSize, rects[i].y + rects[i].rectSize), clr[i], thickness);
    //imshow(name[i], mat[i]);
  }
  imshow("RGB", mat[3]);

  printf("=============Result:=============\n");
  printf("RectPos:(%d,%d),(%d,%d),(%d,%d)\n", rects[0].x, rects[0].y, rects[1].x, rects[1].y, rects[2].x, rects[2].y);
  printf("mean: %f, %f, %f\n", lightinfos[0].avg, lightinfos[1].avg, lightinfos[2].avg);
  printf("stddev: %f, %f, %f\n", lightinfos[0].stddev, lightinfos[1].stddev, lightinfos[2].stddev);
  printf("uniformity: %f, %f, %f\n", lightinfos[0].uniformity, lightinfos[1].uniformity, lightinfos[2].uniformity);

  waitKey();
  return 0;
}