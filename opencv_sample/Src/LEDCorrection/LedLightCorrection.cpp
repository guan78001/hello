#include "LedLightCorrection.h"
#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

int LedLightCorrection::GetLightInfo(char *filename, LightInfo &lightinfo) {
  Mat image = imread(filename);
  GetLightInfo(image, lightinfo);


  RectInfo ri;
  //max rect
  ri.rectSize = 140;
  GetMaxRect(image, ri);

  //min rect
  ri.isMax = false;
  ri.rectSize = 20;
  GetMaxRect(image, ri);
  return 1;
}

int LedLightCorrection::GetLightInfo(char *filename[3], LightInfo lightinfo[3]) {
  for (int i = 0; i < 3; i++) {
    GetLightInfo(filename[i], lightinfo[i]);
  }
  printf("mean: %f, %f, %f\n", lightinfo[0].avg, lightinfo[1].avg, lightinfo[2].avg);
  printf("stddev: %f, %f, %f\n", lightinfo[0].stddev, lightinfo[1].stddev, lightinfo[2].stddev);
  printf("uniformity: %f, %f, %f\n", lightinfo[0].uniformity, lightinfo[1].uniformity, lightinfo[2].uniformity);
  return 1;
}

void LedLightCorrection::GetLightInfo(Mat image, LightInfo &lightinfo) {
  cv::Scalar mean, stddev; //0:1st channel, 1:2nd channel and 2:3rd channel
  meanStdDev(image, mean, stddev, cv::Mat());
  //cout << "mean: " << mean[0] << ", stddev=" << stddev[0] << endl;
  double min, max;
  //cv::Point min_loc, max_loc;
  //cv::minMaxLoc(image, &min, &max, &min_loc, &max_loc);
  cv::minMaxLoc(image, &min, &max);
  double uniformity = min / mean[2];
  //cout << "uniformity=" << uniformity << endl;

  lightinfo.avg = mean[0];
  lightinfo.stddev = stddev[0];
  lightinfo.uniformity = uniformity;
}

int LedLightCorrection::GetMaxRect(unsigned char *data, int height, int width, int channels, RectInfo &rect) {
  vector<double> colB((height + 1) * (width + 1));
#define COLB(y,x) colB[(y)*(width+1)+(x)]
  int bestx = 0, besty = 0;
  double best = rect.isMax ? 0 : 255.0f * rect.rectSize * rect.rectSize;
  int x, y;
  for (y = 1; y <= height; y++) {
    double *row = &COLB(y, 0);
    uchar *pData = data + channels * (y - 1) * width;
    for (x = 1; x <= width; x++) {
      row[x] = row[x - 1] + pData[channels * (x - 1)];
    }
  }
  int r = rect.rectSize;
  for (x = rect.rectSize; x <= width; x++) {
    double area = 0;
    for (y = rect.rectSize; y <= height; y++) {
      if (y == rect.rectSize) {
        for (int j = 1; j <= y; j++) {
          area += COLB(j, x) - COLB(j, x - r);
        }
      } else {
        area += COLB(y, x) - COLB(y, x - r);
        area -= COLB(y - r, x) - COLB(y - r, x - r);
      }
      if (area > best == rect.isMax) {
        best = area;
        bestx = x;
        besty = y;
      }
    }
  }

  rect.x = bestx - r + 1;
  rect.y = besty - r + 1;
  //cout << "bestx=" << rect.x << ", besty=" << rect.y << ", best=" << best << endl;
  return 1;
}

int LedLightCorrection::GetMaxRect(Mat image, RectInfo &rect) {
  int height = image.rows;
  int width = image.cols;
  int nChanels = image.channels();
  uchar *data = image.data;

  return GetMaxRect(data, height, width, nChanels, rect);
}

