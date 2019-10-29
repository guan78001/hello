#include "LedLightCorrection.h"
#include <string>
#include <omp.h>
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
    printf("Usage: \n\tLEDCorrection.exe red.jpg green.jpg blue.jpg [nRectMaxSize=140 nRectMinSize=50] [uniformityThreshold=0.8]\n");
  } else {
    for (int i = 0; i < 3; i++) {
      filename[i] = argv[i + 1];
    }
  }
  LedLightCorrection led;
  LightInfo lightinfos[3];
  Mat mat[4];
  RectInfo max_rects[3];
  RectInfo min_rects[3];

  int nRectMaxSize = 140;
  int nRectMinSize = 50;

  if (argc >= 6) {
    nRectMaxSize = atoi(argv[4]);
    nRectMinSize = atoi(argv[5]);
  }
  printf("nRectMaxSize=[%d], nRectMinSize=[%d]\n", nRectMaxSize, nRectMinSize);
  float uniformityThreshold = 0.8;
  if (argc >= 7) {
    uniformityThreshold = atof(argv[6]);
  }
  printf("uniformityThreshold=%f\n", uniformityThreshold);
  char *name[3] = { "R", "G", "B" };
  cv::Scalar clr[3] = { cv::Scalar(0, 0, 255), cv::Scalar(0, 255, 0), cv::Scalar(255, 0, 0)};//BRG order
  double t0 = omp_get_wtime();
  for (int i = 0; i < 3; i++) {
    mat[i] = imread(filename[i]);
    max_rects[i].rectSize = nRectMaxSize;
    min_rects[i].rectSize = nRectMinSize;
    min_rects[i].isMax = false;
    led.GetMaxRect(mat[i], min_rects[i]);
    led.GetMaxRect(mat[i], max_rects[i]);
    led.GetLightInfo(mat[i], lightinfos[i]);
  }

  MergeImage(mat, mat[3]);

  int thickness = 2;
  for (int i = 0; i < 3; i++) {
    cv::rectangle(mat[i], cv::Point(max_rects[i].x, max_rects[i].y), cv::Point(max_rects[i].x + max_rects[i].rectSize, max_rects[i].y + max_rects[i].rectSize), clr[i], thickness);
    cv::rectangle(mat[i], cv::Point(min_rects[i].x, min_rects[i].y), cv::Point(min_rects[i].x + min_rects[i].rectSize, min_rects[i].y + min_rects[i].rectSize), clr[i], 1, 4);
    cv::rectangle(mat[3], cv::Point(max_rects[i].x, max_rects[i].y), cv::Point(max_rects[i].x + max_rects[i].rectSize, max_rects[i].y + max_rects[i].rectSize), clr[i], thickness);
    cv::rectangle(mat[3], cv::Point(min_rects[i].x, min_rects[i].y), cv::Point(min_rects[i].x + min_rects[i].rectSize, min_rects[i].y + min_rects[i].rectSize), clr[i], 1, 4);
    lightinfos[i].uniformity = min_rects[i].avg / lightinfos[i].avg;
    //char title[1024];
    //sprintf(title, "%s, mean=%.2f,stddev=%.2f, AvgtMinRect=%.2f,AvgMaxRect=%.2f, uniformity=%.3f",
    //        name[i], lightinfos[i].avg, lightinfos[i].stddev, min_rects[i].avg, max_rects[i].avg, lightinfos[i].uniformity);
    //imshow(title, mat[i]);
  }
  //imshow("RGB", mat[3]);


  Mat bigImage(mat[0].size() * 2, CV_8UC3, Scalar(0, 0, 0));
  int width = mat[0].cols;
  int height = mat[0].rows;
  mat[0].copyTo(bigImage(Rect(0, 0, width, height)));
  mat[1].copyTo(bigImage(Rect(width, 0, width, height)));
  mat[2].copyTo(bigImage(Rect(0, height, width, height)));
  mat[3].copyTo(bigImage(Rect(width, height, width, height)));

  printf("=============Result:=============\n");
  //printf("RectPos:(%d,%d),(%d,%d),(%d,%d)\n", max_rects[0].x, max_rects[0].y, max_rects[1].x, max_rects[1].y, max_rects[2].x, max_rects[2].y);
  //printf("mean: %f, %f, %f\n", lightinfos[0].avg, lightinfos[1].avg, lightinfos[2].avg);
  //printf("stddev: %f, %f, %f\n", lightinfos[0].stddev, lightinfos[1].stddev, lightinfos[2].stddev);
  //printf("uniformity: %f, %f, %f\n", lightinfos[0].uniformity, lightinfos[1].uniformity, lightinfos[2].uniformity);
  //printf("Channel	mean	stddev	avgMinRect	avgMaxRect	uniformity\n");
  bool pass[3] = { 1, 1, 1 };
  for (int i = 0; i < 3; i++) {
    char title[1024];
    pass[i] = lightinfos[i].uniformity > uniformityThreshold;
    sprintf(title, "%s, mean=%.2f,stddev=%.2f, AvgtMinRect=%.2f,AvgMaxRect=%.2f, uniformity=%.2f, Pass=%d",
            name[i], lightinfos[i].avg, lightinfos[i].stddev, min_rects[i].avg, max_rects[i].avg, lightinfos[i].uniformity, pass[i]);
    printf("%s\n", title);
  }
  bool isPass = pass[0] && pass[1] && pass[2];
  char *result = isPass ? "Light Uniformity Test Pass." : "Light Uniformity Test Failed.";
  char failed_str[256] = { 0 };
  char *channel_str[3] = { "Red", "Green", "Blue" };
  int count = 0;
  for (int i = 0; i < 3; i++) {
    if (!pass[i]) {
      count++;
      if (count > 1) {
        strcat(failed_str, ",");
      }
      strcat(failed_str, channel_str[i]);
    }
  }


  char title[256] = { 0 };
  if (isPass) {
    strcat(title, result);
  } else {
    sprintf(title, "%s Failed Channel:%s", result, failed_str);
  }
  printf("%s\n", title);
  double t1 = omp_get_wtime();
  printf("used time: %f s\n", t1 - t0);
  namedWindow(title, 0);
  cv::resizeWindow(title, width * 1.6, height * 1.6);
  imshow(title, bigImage);

  waitKey();
  return 0;
}