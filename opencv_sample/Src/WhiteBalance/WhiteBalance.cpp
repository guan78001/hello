#include <opencv2/opencv.hpp>
using namespace cv;
//https://blog.csdn.net/chenyuping333/article/details/82496591
void GrayWorld_WhiteBalance(char *imageFileName) {
  Mat imageSource = imread(imageFileName);
  //imshow("Origin", imageSource);
  vector<Mat> imageRGB;

  split(imageSource, imageRGB);

  double R, G, B;
  B = mean(imageRGB[0])[0];
  G = mean(imageRGB[1])[0];
  R = mean(imageRGB[2])[0];

  double KR, KG, KB;
  KB = (R + G + B) / (3 * B) / 0.93;
  KG = (R + G + B) / (3 * G) / 0.94;
  KR = (R + G + B) / (3 * R) / 1.16;
  printf("KB=%f,KG=%f,KR=%f\n", KB, KG, KR);
  imageRGB[0] = imageRGB[0] * KB;
  imageRGB[1] = imageRGB[1] * KG;
  imageRGB[2] = imageRGB[2] * KR;

  merge(imageRGB, imageSource);
  imshow("GrayWorld_WhiteBalance", imageSource);
}

void LineScale(char *imageFileName) {
  Mat imageSource = imread(imageFileName);
  //imshow("Origin", imageSource);
  vector<Mat> imageRGB;

  split(imageSource, imageRGB);

  imageRGB[0] = imageRGB[0] / 0.93;
  imageRGB[1] = imageRGB[1] / 0.94;
  imageRGB[2] = imageRGB[2] / 1.16;

  merge(imageRGB, imageSource);
  imshow("LineScale", imageSource);
}

void color_balance(IplImage *img) {
  int histo[256] = { 0 };//直方图统计每个像素值的数目
  int num_of_pixels = img->width * img->height;
  //统计每个像素值的数目
  for (int y = 0; y < img->height; ++y) {
    uchar *data = (uchar *)(img->imageData + y * img->widthStep); //定义的大小和图像尺寸一致
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
  while (histo[vmin + 1] <= cvRound(num_of_pixels * s / 2))
    vmin = vmin + 1;

  int vmax = 255 - 1;

  //统计像素点数目大于num_of_pixels*(1 - s / 2)的数目，s为控制比率
  while (histo[vmax - 1] > cvRound(num_of_pixels * (1 - s / 2))) {
    vmax = vmax - 1;
  }

  if (vmax < 255 - 1)
    vmax = vmax + 1;

  //处理图像中像素值大于vmin和小于vmax的像素，
  //即处理偏亮和偏暗的区域
  for (int y = 0; y < img->height; ++y) {

    uchar *data = (uchar *)(img->imageData + y * img->widthStep);
    for (int x = 0; x < img->width; ++x) {
      if (data[x] < vmin)
        data[x] = vmin;
      if (data[x] > vmax)
        data[x] = vmax;
    }
  }

  //对其他的像素进行处理（拉伸），其实可以合并到上一步，简化时间复杂度，这里分开只是为了让过程更清楚
  for (int y = 0; y < img->height; ++y) {

    uchar *data = (uchar *)(img->imageData + y * img->widthStep);
    for (int x = 0; x < img->width; ++x) {
      data[x] = cvRound((data[x] - vmin) * 255.0 / (vmax - vmin));
    }
  }
}

// Arbitrary rgb values 0..255
typedef struct rgb {
  unsigned char r, g, b;
  struct rgb() : r(0), g(0), b(0) {};
  struct rgb(unsigned char r1, unsigned char g1, unsigned char b1) : r(r1), g(g1), b(b1) {};
} Rgb;

//! Hue saturation value
typedef struct hsv {
  //! h in the range [0..360], s in the range [0..1], v in the range [0..1]
  float h, s, v;
  struct hsv() : h(0), s(0), v(0) {};
  struct hsv(float h1, float s1, float v1) : h(h1), s(s1), v(v1) {};
} Hsv;

void Convert(const Rgb &rgb, Hsv &hsv) {
  const float sc_inv255 = 1.0f / 255.0f;
  float r = rgb.r * sc_inv255;
  float g = rgb.g * sc_inv255;
  float b = rgb.b * sc_inv255;

  float high = (r > g) ? r : g;
  high = (high > b) ? high : b;
  float low = (r < g) ? r : g;
  low = (low < b) ? low : b;

  float h, s, v;
  v = high;

  float delta = high - low;
  if (delta < 0.00001f) {
    h = 0; // undefined
    s = 0;
  } else {
    s = delta / high;

    if (r == high) {
      h = (g - b) / delta; // yellow and magenta
    } else if (g == high) {
      h = 2.0f + (b - r) / delta; // cyan and yellow
    } else { // b==high
      h = 4.0f + (r - g) / delta; // magenta and cyan
    }
    h *= 60.0f; //degrees

    if (h < 0) {
      h += 360.0f;
    }
  }

  hsv.h = h;
  hsv.s = s;
  hsv.v = v;
}

void Convert(const Hsv &hsv, Rgb &rgb) {
  float h = hsv.h;
  float s = hsv.s;
  float v = hsv.v;

  const float sc_inv60 = 1.0f / 60.0f;
  if (h > 360.0f) h = 0.0f;
  h *= sc_inv60;

  int k = (int)(h);
  float f = h - k; // residual
  float p = v * (1.0f - s);
  float q = v * (1.0f - s * f);
  float t = v * (1.0f - s * (1 - f));

  float r, g, b;
  switch (k) {
    case 0:
      r = v; g = t; b = p; break;
    case 1:
      r = q; g = v; b = p; break;
    case 2:
      r = p; g = v; b = t; break;
    case 3:
      r = p; g = q; b = v; break;
    case 4:
      r = t; g = p; b = v; break;
    case 5:
    default:
      r = v; g = p; b = q; break;
  }

  rgb.r = (unsigned char)(r * 255 + 0.5f);
  rgb.g = (unsigned char)(g * 255 + 0.5f);
  rgb.b = (unsigned char)(b * 255 + 0.5f);
}

// Map selected range range [0..minVal..maxVal..1] to output range [0..outLow..outHigh..1]
class Normalizer {
 public:
  Normalizer(float inLow, float inHigh, float outLow, float outHigh);
  Normalizer(const Normalizer &) = default;
  float Get(float v)const;
 private:
  float m_inLow;
  float m_inHigh;
  float m_outLow;
  float m_outHigh;

  float k1, k2, k3;
};

Normalizer::Normalizer(float inLow, float inHigh, float outLow, float outHigh) {
  m_inLow = inLow;
  m_inHigh = inHigh;
  m_outLow = outLow;
  m_outHigh = outHigh;

  //Todo: make sure input condition:0<inLow<inHigh<1.
  k1 = outLow / inLow;
  k2 = (1 - outHigh) / (1 - inHigh);
  k3 = (outHigh - outLow) / (inHigh - inLow);
}

float Normalizer::Get(float v) const {
  if (v < m_inLow) {
    // Map [0..lowVal] to [0..outLow]
    v = v * k1;
  } else if (v > m_inHigh) {
    // Map [highVal..1] to [outHigh..1]
    v = m_outHigh + (v - m_inHigh) * k2;
  } else {
    // Map [lowVal..highVal] to [outLow..outHigh]
    v = m_outLow + (v - m_inLow) * k3;
  }
  return v;
}

Normalizer GetNormalizer(Mat &mat, float inLow, float inHigh, float outLow, float outHigh) {
  // Compute value histogram

  const int histSize = 256;
  int hist[histSize] = { 0 };

  for (int y = 0; y < mat.rows; ++y) {
    uchar *data = mat.ptr<uchar>(y);
    for (int x = 0; x < mat.cols; ++x) {
      hist[data[x]] += 1;
    }
  }

  int count = 0;
  for (int i = 0; i < histSize; ++i) {
    count += hist[i];
  }

  // Determine relevant min.max from histogram
  float low = inLow * count;
  float high = inHigh * count;

  int lowBin = 0;
  int i = 0;
  int sum = 0;
  while (i < histSize) {
    sum += hist[i];
    lowBin = i;
    if (sum > low) {
      break;
    }
    ++i;
  }
  float lowVal = lowBin / float(histSize); // in [0..1]

  int highBin = lowBin;
  while (i < histSize) {
    sum += hist[i];
    highBin = i;
    if (sum > high) {
      break;
    }
    ++i;
  }
  float highVal = highBin / float(histSize); // in [0..1]

  Normalizer normalizer(lowVal, highVal, outLow, outHigh);
  return normalizer;
}
Normalizer GetNormalizer_hsv(Mat &mat, float inLow, float inHigh, float outLow, float outHigh) {
  // Compute value histogram

  const int histSize = 256;
  int hist[histSize] = { 0 };

  for (int y = 0; y < mat.rows; ++y) {
    for (int x = 0; x < mat.cols; ++x) {
      Point3_<uchar> *p = mat.ptr<Point3_<uchar> >(y, x);
      Rgb rgb;
      Hsv hsv;
      rgb.b = p->x;
      rgb.g = p->y;
      rgb.r = p->z;
      Convert(rgb, hsv);
      int bin = (int)(hsv.v * histSize);
      if (bin >= 0 && bin < histSize) {
        hist[bin]++;
      }
    }
  }

  int count = 0;
  for (int i = 0; i < histSize; ++i) {
    count += hist[i];
  }

  // Determine relevant min.max from histogram
  float low = inLow * count;
  float high = inHigh * count;

  int lowBin = 0;
  int i = 0;
  int sum = 0;
  while (i < histSize) {
    sum += hist[i];
    lowBin = i;
    if (sum > low) {
      break;
    }
    ++i;
  }
  float lowVal = lowBin / float(histSize); // in [0..1]

  int highBin = lowBin;
  while (i < histSize) {
    sum += hist[i];
    highBin = i;
    if (sum > high) {
      break;
    }
    ++i;
  }
  float highVal = highBin / float(histSize); // in [0..1]

  Normalizer normalizer(lowVal, highVal, 40.0f / 255.0f, 200.0f / 255.0f);
  return normalizer;
}
void color_balance2(Mat &mat, float low = 40.0f / 255.0f, float high = 200.0f / 255.0f) {
  Normalizer normalizer = GetNormalizer(mat, 0.025f, 0.975f, low, high);
  for (int y = 0; y < mat.rows; ++y) {
    uchar *data = mat.ptr<uchar>(y);
    for (int x = 0; x < mat.cols; ++x) {
      data[x] = normalizer.Get(data[x] / 255.0f) * 255.0f;
    }
  }
}
void color_balance_hsv(Mat &mat) {
  Normalizer normalizer = GetNormalizer_hsv(mat, 0.025f, 0.975f, 40.0f / 255.0f, 200.0f / 255.0f);
  for (int y = 0; y < mat.rows; ++y) {
    for (int x = 0; x < mat.cols; ++x) {
      Point3_<uchar> *p = mat.ptr<Point3_<uchar> >(y, x);
      Rgb rgb;
      Hsv hsv;
      rgb.b = p->x;
      rgb.g = p->y;
      rgb.r = p->z;
      Convert(rgb, hsv);
      hsv.v = normalizer.Get(hsv.v);
      Convert(hsv, rgb);
      p->x = rgb.b;
      p->y = rgb.g;
      p->z = rgb.r;
    }
  }
}
void color_balance(Mat &mat) {
  int histo[256] = { 0 };
  int num_of_pixels = mat.size().area();

  //get histogram
  for (int y = 0; y < mat.rows; ++y) {
    uchar *data = mat.ptr<uchar>(y);
    for (int x = 0; x < mat.cols; ++x) {
      histo[data[x]] += 1;
    }
  }

  //accmulate histrogram
  for (int i = 1; i < 256; ++i)
    histo[i] = histo[i] + histo[i - 1];

  //get valid range of histogram
  double s = 0.0265;
  int vmin = 0;
  while (histo[vmin + 1] <= cvRound(num_of_pixels * s / 2))
    vmin = vmin + 1;

  int vmax = 255 - 1;
  while (histo[vmax - 1] > cvRound(num_of_pixels * (1 - s / 2))) {
    vmax = vmax - 1;
  }
  if (vmax < 255 - 1)
    vmax = vmax + 1;


  //scale histogram
  for (int y = 0; y < mat.rows; ++y) {
    uchar *data = mat.ptr<uchar>(y);
    for (int x = 0; x < mat.cols; ++x) {
      if (data[x] < vmin)
        data[x] = vmin;
      else if (data[x] > vmax)
        data[x] = vmax;
      data[x] = cvRound((data[x] - vmin) * 255.0 / (vmax - vmin));
    }
  }

}
int ReadImage(char *path, int id, int id2, Mat &mat) {
  vector<Mat> imageRGB(3);
  char filename[256];
  int count = 0;
  for (int i = 0; i < 3; i++) {
    sprintf(filename, "%s\\image_upper_%d_%d_0%d.jpg", path, id, id2, i + 6);
    printf("%s\n", filename);
    imageRGB[2 - i] = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
    if (imageRGB[2 - i].empty()) break;
    else {
      count++;
    }
  }
  if (count < 3) return 0;
  merge(imageRGB, mat);
  return 1;
}
void HisgramScale_RGB3(char *path, int id) {
  //imshow("Origin", imageSource);
  vector<Mat> imageRGB(3);
  char filename[256];
  for (int i = 0; i < 3; i++) {
    sprintf(filename, "%s\\image_upper_%d_0%d.jpg", id, i + 6);
    imageRGB[i] = imread(filename);
  }

  color_balance2(imageRGB[0]);
  color_balance2(imageRGB[1]);
  color_balance2(imageRGB[2]);
  Mat mat;
  merge(imageRGB, mat);
  imshow("HisgramScale_RGB3", mat);
}
void HisgramScale_RGB(Mat &mat) {
  //imshow("Origin", imageSource);
  vector<Mat> imageRGB;
  split(mat, imageRGB);
  color_balance2(imageRGB[0], 40.0f / 255.0f, 170.0f / 255.0f);
  color_balance2(imageRGB[1], 20.0f / 255.0f, 150.0f / 255.0f);
  color_balance2(imageRGB[2], 20.0f / 255.0f, 140.0f / 255.0f);
  merge(imageRGB, mat);
}
void HisgramScale_RGB(char *imageFileName) {
  Mat imageSource = imread(imageFileName);
  HisgramScale_RGB(imageSource);
  imshow("HisgramScaleRGB", imageSource);
}

void HisgramScale_Hsv(char *imageFileName) {
  Mat mat = imread(imageFileName);
  color_balance_hsv(mat);
  imshow("HisgramScale_Hsv", mat);
}
void HisgramScale_Hsv(Mat &mat) {
  color_balance_hsv(mat);
}

void GammaCorrection(float amplitude, float gamma, unsigned char &r, unsigned char &g, unsigned char &b) {
  Rgb rgb;
  rgb.r = r;
  rgb.g = g;
  rgb.b = b;

  // Compute the gamma correction
  Hsv hsv;
  Convert(rgb, hsv);
  // Add extra gamma correction to brighten the overall
  hsv.v = pow(hsv.v, gamma); // apply correction, brightens if less than 1
  hsv.v *= amplitude;

  // Adjust saturation (function provided by Zech)
  float L = 0.2f, center = 0.4f;
  float fac = L * 0.16f;
  float pi = 3.1415926f;
  float outputS = hsv.s <= 0 ? 0.0f : exp(log(hsv.s) / 0.7f);
  if (hsv.s <= center) {
    outputS += fac * sin(hsv.s / center * pi);
  } else {
    outputS += fac * sin((hsv.s - center) / (1.0f - center) * pi);
  }
  hsv.s = std::max(0.0f, std::min(outputS, 1.0f));
  Convert(hsv, rgb);

  r = rgb.r;
  g = rgb.g;
  b = rgb.b;
}

void GammaCorrection(cv::Mat &image, float gamma) {
  int col = image.cols;
  int row = image.rows;
  int channel = image.channels();
  unsigned char *imageData = image.data;
  #pragma omp parallel for
  for (int i = 0; i < col * row; i++) {
    unsigned char *color = imageData + i * channel;
    GammaCorrection(1.0, gamma, color[2], color[1], color[0]); // A = 1.0 by default
  }
}


int main() {
  //char *path = "D:\\DataSet\\shade_image\\StoneModel\\\jpeg";
  //int id1 = 4481, id2 = 13;

  //char *path = "D:\\DataSet\\shade_image\\Invivo3\\Models1";
  //int id1 = 3862, id2 = 35;

  char *path = "D:\\DataSet\\shade_image\\PlasticModel2\\Models";
  int id1 = 251, id2 = 22;

  //convert image
  char imageFileName[256];
  if (0) {
    int count = 1;
    for (int i = 0; i < 100; i++) {
      printf("i=%d\n", i);
      {
        Mat mat;
        ReadImage(path, id1 + i, id2 + i, mat);
        if (!mat.empty()) {
          sprintf(imageFileName, "%s\\image_%d.jpg", path, count++);
          imwrite(imageFileName, mat);
        }
      }
    }
  }


  for (int i = 0; i < 100; i++) {
    printf("i=%d\n", i);
    {
      sprintf(imageFileName, "%s\\image_%d.jpg", path, i++);
      Mat mat = imread(imageFileName);
      imshow("Origin", mat);
      GammaCorrection(mat, 0.6);//ACQ: 0.2, 0.4, 0.6
      imshow("GammaCorrection", mat);
      GrayWorld_WhiteBalance(imageFileName);
      HisgramScale_RGB(imageFileName);
      HisgramScale_Hsv(imageFileName);
      waitKey();
    }
    waitKey();
  }
  return 0;
}
