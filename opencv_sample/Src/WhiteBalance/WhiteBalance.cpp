#include <opencv2/opencv.hpp>
using namespace cv;
//https://blog.csdn.net/chenyuping333/article/details/82496591
void GrayWorld_WhiteBalance(char * imageFileName) {
  Mat imageSource = imread(imageFileName);
  imshow("ԭʼͼ��", imageSource);
  vector<Mat> imageRGB;

  //RGB��ͨ������
  split(imageSource, imageRGB);

  //��ԭʼͼ���RGB�����ľ�ֵ
  double R, G, B;
  B = mean(imageRGB[0])[0];
  G = mean(imageRGB[1])[0];
  R = mean(imageRGB[2])[0];

  //��Ҫ������RGB����������
  double KR, KG, KB;
  KB = (R + G + B) / (3 * B);
  KG = (R + G + B) / (3 * G);
  KR = (R + G + B) / (3 * R);

  //����RGB����ͨ�����Ե�ֵ
  imageRGB[0] = imageRGB[0] * KB;
  imageRGB[1] = imageRGB[1] * KG;
  imageRGB[2] = imageRGB[2] * KR;

  //RGB��ͨ��ͼ��ϲ�
  merge(imageRGB, imageSource);
  imshow("��ƽ�������", imageSource);
}

void color_balance(IplImage *img) {
  int histo[256] = { 0 };//ֱ��ͼͳ��ÿ������ֵ����Ŀ
  int num_of_pixels = img->width*img->height;
  //ͳ��ÿ������ֵ����Ŀ
  for (int y = 0; y < img->height; ++y) {
    uchar *data = (uchar*)(img->imageData + y*img->widthStep);//����Ĵ�С��ͼ��ߴ�һ��
    for (int x = 0; x < img->width; ++x) {
      histo[data[x]] += 1;
    }
  }

  //ͳ�Ƶ�ǰ����ֵ��֮ǰ����ֵ������
  for (int i = 1; i < 256; ++i)
    histo[i] = histo[i] + histo[i - 1];

  double s = 0.0265;//�˲������Ե����������0.1����(0=<s<=1)

  int vmin = 0;

  //ͳ�����ص���ĿС��num_of_pixels*s / 2����Ŀ��sΪ���Ʊ���
  while (histo[vmin + 1] <= cvRound(num_of_pixels*s / 2))
    vmin = vmin + 1;

  int vmax = 255 - 1;

  //ͳ�����ص���Ŀ����num_of_pixels*(1 - s / 2)����Ŀ��sΪ���Ʊ���
  while (histo[vmax - 1] > cvRound(num_of_pixels*(1 - s / 2))) {
    vmax = vmax - 1;
  }

  if (vmax < 255 - 1)
    vmax = vmax + 1;

  //����ͼ��������ֵ����vmin��С��vmax�����أ�
  //������ƫ����ƫ��������
  for (int y = 0; y < img->height; ++y) {

    uchar *data = (uchar*)(img->imageData + y*img->widthStep);
    for (int x = 0; x < img->width; ++x) {
      if (data[x] < vmin)
        data[x] = vmin;
      if (data[x] > vmax)
        data[x] = vmax;
    }
  }

  //�����������ؽ��д������죩����ʵ���Ժϲ�����һ������ʱ�临�Ӷȣ�����ֿ�ֻ��Ϊ���ù��̸����
  for (int y = 0; y < img->height; ++y) {

    uchar *data = (uchar*)(img->imageData + y*img->widthStep);
    for (int x = 0; x < img->width; ++x) {
      data[x] = cvRound((data[x] - vmin)*255.0 / (vmax - vmin));
    }
  }
}

void HisgramScale(char * imageFileName) {
  IplImage *srcImg = cvLoadImage(imageFileName);//��ȡͼƬ
  IplImage *dstImg = cvCreateImage(cvGetSize(srcImg), 8, 3);
  IplImage *redCh = cvCreateImage(cvGetSize(srcImg), 8, 1);//Rͨ��
  IplImage *greenCh = cvCreateImage(cvGetSize(srcImg), 8, 1);//Gͨ��
  IplImage *blueCh = cvCreateImage(cvGetSize(srcImg), 8, 1);//Bͨ��
  cvSplit(srcImg, blueCh, greenCh, redCh, NULL);//��ԭͼ���RGBͨ��
  color_balance(redCh);//��Rͨ������ɫ��ƽ��
  color_balance(greenCh);//��Gͨ������ɫ��ƽ��
  color_balance(blueCh);//��Bͨ������ɫ��ƽ��
  cvMerge(blueCh, greenCh, redCh, NULL, dstImg);//�ϲ��������ͨ����Ϊ���ս��

  //��ʾ����
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