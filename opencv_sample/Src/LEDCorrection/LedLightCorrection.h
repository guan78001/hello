#pragma  once
//#include <opencv2/opencv.hpp>
//using namespace cv;

struct RectInfo {
  int x, y;//left top position
  int rectSize;//edge-length of rect(square).
  float avg = 0;
  bool isMax = true;
};
struct LightInfo {
  float avg;
  float stddev;//standard deviation
  float uniformity;//Uniformity of light. lowest/avg
};

namespace cv {
class Mat;
}
class LedLightCorrection {
 public:
  int LedLightCorrection::GetLightInfo(char *filename, LightInfo &lightinfo);
  int GetLightInfo(char *filename[3], LightInfo lightinfo[3]);

  void GetLightInfo(cv::Mat image, LightInfo &lightinfo);
  int GetMaxRect(cv::Mat image, RectInfo &rect);
  int GetMaxRect(unsigned char *data, int height, int width, int channels, RectInfo &rect);
};