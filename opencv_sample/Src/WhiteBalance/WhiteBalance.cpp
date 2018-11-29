#include <opencv2/opencv.hpp>
using namespace cv;
//https://blog.csdn.net/chenyuping333/article/details/82496591
void GrayWorld_WhiteBalance(char * imageFileName) {
  Mat imageSource = imread(imageFileName);
  imshow("原始图像", imageSource);
  vector<Mat> imageRGB;

  //RGB三通道分离
  split(imageSource, imageRGB);

  //求原始图像的RGB分量的均值
  double R, G, B;
  B = mean(imageRGB[0])[0];
  G = mean(imageRGB[1])[0];
  R = mean(imageRGB[2])[0];

  //需要调整的RGB分量的增益
  double KR, KG, KB;
  KB = (R + G + B) / (3 * B);
  KG = (R + G + B) / (3 * G);
  KR = (R + G + B) / (3 * R);

  //调整RGB三个通道各自的值
  imageRGB[0] = imageRGB[0] * KB;
  imageRGB[1] = imageRGB[1] * KG;
  imageRGB[2] = imageRGB[2] * KR;

  //RGB三通道图像合并
  merge(imageRGB, imageSource);
  imshow("白平衡调整后", imageSource);
}

void color_balance(IplImage *img) {
  int histo[256] = { 0 };//直方图统计每个像素值的数目
  int num_of_pixels = img->width*img->height;
  //统计每个像素值的数目
  for (int y = 0; y < img->height; ++y) {
    uchar *data = (uchar*)(img->imageData + y*img->widthStep);//定义的大小和图像尺寸一致
    for (int x = 0; x < img->width; ++x) {
      histo[data[x]] += 1;
    }
  }

  //统计当前像素值和之前像素值的总数
  for (int i = 1; i < 256; ++i)
    histo[i] = histo[i] + histo[i - 1];

  double s = 0.0265;//此参数可以调整，最好在0.1以下(0=<s<=1)

  int vmin = 0;

  //统计像素点数目小于num_of_pixels*s / 2的数目，s为控制比率
  while (histo[vmin + 1] <= cvRound(num_of_pixels*s / 2))
    vmin = vmin + 1;

  int vmax = 255 - 1;

  //统计像素点数目大于num_of_pixels*(1 - s / 2)的数目，s为控制比率
  while (histo[vmax - 1] > cvRound(num_of_pixels*(1 - s / 2))) {
    vmax = vmax - 1;
  }

  if (vmax < 255 - 1)
    vmax = vmax + 1;

  //处理图像中像素值大于vmin和小于vmax的像素，
  //即处理偏亮和偏暗的区域
  for (int y = 0; y < img->height; ++y) {

    uchar *data = (uchar*)(img->imageData + y*img->widthStep);
    for (int x = 0; x < img->width; ++x) {
      if (data[x] < vmin)
        data[x] = vmin;
      if (data[x] > vmax)
        data[x] = vmax;
    }
  }

  //对其他的像素进行处理（拉伸），其实可以合并到上一步，简化时间复杂度，这里分开只是为了让过程更清楚
  for (int y = 0; y < img->height; ++y) {

    uchar *data = (uchar*)(img->imageData + y*img->widthStep);
    for (int x = 0; x < img->width; ++x) {
      data[x] = cvRound((data[x] - vmin)*255.0 / (vmax - vmin));
    }
  }
}

void HisgramScale(char * imageFileName) {
  IplImage *srcImg = cvLoadImage(imageFileName);//读取图片
  IplImage *dstImg = cvCreateImage(cvGetSize(srcImg), 8, 3);
  IplImage *redCh = cvCreateImage(cvGetSize(srcImg), 8, 1);//R通道
  IplImage *greenCh = cvCreateImage(cvGetSize(srcImg), 8, 1);//G通道
  IplImage *blueCh = cvCreateImage(cvGetSize(srcImg), 8, 1);//B通道
  cvSplit(srcImg, blueCh, greenCh, redCh, NULL);//把原图拆分RGB通道
  color_balance(redCh);//对R通道进行色彩平衡
  color_balance(greenCh);//对G通道进行色彩平衡
  color_balance(blueCh);//对B通道进行色彩平衡
  cvMerge(blueCh, greenCh, redCh, NULL, dstImg);//合并操作后的通道，为最终结果

  //显示操作
  cvNamedWindow("src", CV_WINDOW_AUTOSIZE);
  cvShowImage("src", srcImg);

  cvNamedWindow("dst_hist", CV_WINDOW_AUTOSIZE);
  cvShowImage("dst_hist", dstImg);
}


int main() {
  char *imageFileName = "D:\\git_clone\\github\\hello\\opencv_sample\\data\\fog.png";
  GrayWorld_WhiteBalance(imageFileName);
  HisgramScale(imageFileName);
  waitKey();
  return 0;
}