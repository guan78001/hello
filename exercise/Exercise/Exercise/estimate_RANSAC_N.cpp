#include <iostream>
#include <algorithm>
using namespace std;

int EstimateK(float inlier_ratio, float expetect_confidence, int ransac_n = 3) {
  /*
  in liner ratio:r
  a good transform ratio: r^3
  # 1 fail transform ratio:1-r^3
  # N fail transform ratio: (1-r^3)^N
  # expect fail rate 1-expetect_p
  # expect N:
  */
  double est_d =
    std::log(1.0 - expetect_confidence) /
    std::log(1.0 - std::pow(inlier_ratio, ransac_n));

  int est_i = static_cast<int>(std::ceil(est_d));
  return est_i;
}
int EstimateIterations(float inlier_ratio, float expetect_confidence, int MaxIterations, int ransac_n = 3) {
  return std::min(EstimateK(inlier_ratio, expetect_confidence, ransac_n), MaxIterations);
}

int main() {
  cout << EstimateK(0.356322, 0.999000) << endl;
  return 0;
}