#include <iostream>
#include "C:\Work\code\lfs_d3dvideo_code\3rdParty\eigen3\include\Eigen\Eigen"
#include <cmath>
using namespace std;
const float pi = 3.141592f;
const float piinv = 1.0f / pi;

void test_atan2() {
#define PRINT_atan2(y,x) cout << "atan2("<<y<<","<<x<<")="<<atan2(y,x)<<", degree="<<atan2(y,x)/3.1415926*180 << endl;
  PRINT_atan2(0, 1);
  PRINT_atan2(1, 1);
  PRINT_atan2(1, 0);
  PRINT_atan2(1, -1);
  PRINT_atan2(0, -1);

  PRINT_atan2(-1, 1);
  PRINT_atan2(-1, 0);
  PRINT_atan2(-1, -1);
  PRINT_atan2(-1e-6, -1);
}
#define  PCL_DEBUG printf
bool pcl_computePairFeatures(const Eigen::Vector4f &p1, const Eigen::Vector4f &n1,
                             const Eigen::Vector4f &p2, const Eigen::Vector4f &n2,
                             float &f1, float &f2, float &f3, float &f4) {
  Eigen::Vector4f dp2p1 = p2 - p1;
  dp2p1[3] = 0.0f;
  f4 = dp2p1.norm();

  if (f4 == 0.0f) {
    PCL_DEBUG("[pcl::computePairFeatures] Euclidean distance between points is 0!\n");
    f1 = f2 = f3 = f4 = 0.0f;
    return (false);
  }

  Eigen::Vector4f n1_copy = n1,
                  n2_copy = n2;
  n1_copy[3] = n2_copy[3] = 0.0f;
  float angle1 = n1_copy.dot(dp2p1) / f4;

  // Make sure the same point is selected as 1 and 2 for each pair
  float angle2 = n2_copy.dot(dp2p1) / f4;
  if (std::acos(std::fabs(angle1)) > std::acos(std::fabs(angle2))) {
    // switch p1 and p2
    n1_copy = n2;
    n2_copy = n1;
    n1_copy[3] = n2_copy[3] = 0.0f;
    dp2p1 *= (-1);
    f3 = -angle2;
  } else
    f3 = angle1;

  // Create a Darboux frame coordinate system u-v-w
  // u = n1; v = (p_idx - q_idx) x u / || (p_idx - q_idx) x u ||; w = u x v
  Eigen::Vector4f v = dp2p1.cross3(n1_copy);
  v[3] = 0.0f;
  float v_norm = v.norm();
  if (v_norm == 0.0f) {
    PCL_DEBUG("[pcl::computePairFeatures] Norm of Delta x U is 0!\n");
    f1 = f2 = f3 = f4 = 0.0f;
    return (false);
  }
  // Normalize v
  v /= v_norm;

  Eigen::Vector4f w = n1_copy.cross3(v);
  // Do not have to normalize w - it is a unit vector by construction

  v[3] = 0.0f;
  f2 = v.dot(n2_copy);
  w[3] = 0.0f;
  // Compute f1 = arctan (w * n2, u * n2) i.e. angle of n2 in the x=u, y=w coordinate system
  f1 = std::atan2(w.dot(n2_copy), n1_copy.dot(n2_copy)); // @todo optimize this
  f1 *= piinv;
  return (true);
}
bool d3d_feature(const Eigen::Vector3f &ps, const Eigen::Vector3f &n1,
                 const Eigen::Vector3f &pt, const Eigen::Vector3f &n2,
                 float &f1, float &f2, float &f3, float &f4) {
  using namespace  Eigen;
  Eigen::Vector3f ptmps = pt - ps;

  Vector3f ns = n1, nt = n2;

  float d = ptmps.norm();
  f4 = d;

  if (d < 1E-6) return false;

  Vector3f ptmps_d = ptmps / d;
  float phi1 = ns.dot(ptmps_d); // in [-1..1]
  float phi2 = nt.dot(ptmps_d); // in [-1..1]
  float phi;

  //For each pair,let the point with small phi angle as start point, which means two points of the pair has same PFH.
  //Trick in PCL: Radu Bogdan Rusu, https://github.com/PointCloudLibrary/pcl/blob/master/features/src/pfh.cpp
  //Theoretical basis: Radu Bogdan Rusu,"Fast Point Feature Histograms(FPFH) for 3D Registration".

  //if (fabs(phi1) < fabs(phi2)) {
  //  // swap two points to compute PFH
  //  std::swap(ns, nt);
  //  ptmps_d *= -1.0f;

  //  phi = -phi2;
  //} else {
  //  phi = phi1;
  //}
  if (fabs(phi1) > fabs(phi2)) {
    phi = phi1;
  } else {
    // swap two points to compute PFH
    std::swap(ns, nt);
    ptmps_d *= -1.0f;

    phi = -phi2;
  }
  Vector3f u = ns;
  Vector3f v = ptmps_d.cross(u);// .CrossProduct(u);
  v.normalize();

  Vector3f w = u.cross(v);

  float alpha = v.dot(nt); // in [-1..1]
  //float phi = u.DotProduct(ptmps_d); // in [-1..1]
  float theta = atan2(w.dot(nt), u.dot(nt)) * piinv; // in [-1..1]

  f1 = theta;
  f2 = alpha;
  f3 = phi;

  return true;
}
void test_feature() {
  using namespace Eigen;
  Eigen::Vector4f p1, p2, n1, n2;
  float f1, f2, f3, f4;
  float eps = 1e-6f;
  //plane
  //p1 = { 1, 0, 0, 0 };
  //p2 = { 2, 0, 0, 0 };
  //n1 = { 0, 0, 1, 0 };
  //n2 = { 0, 0, 1, 0 };
  p1 = { 1, 0, 0, 0 };
  p2 = { 2, 0, 0, 0 };
  n1 = { 0, 0, 1, 0 };
  n2 = { 0, 0, 1, 0 };
  n1.normalize();
  n2.normalize();
  bool res = pcl_computePairFeatures(p1, n1, p2, n2, f1, f2, f3, f4);
  cout << "pcl_feature: " << f1 << " " << f2 << " " << f3 << " " << f4 << endl;

  Vector3f p1_3{ p1[0], p1[1], p1[2] };
  Vector3f p2_3{ p2[0], p2[1], p2[2] };
  Vector3f n1_3{ n1[0], n1[1], n1[2]};
  Vector3f n2_3{ n2[0], n2[1], n2[2] };

  f1 = f2 = f3 = f4 = 0;//clear;
  bool res2 = d3d_feature(p1_3, n1_3, p2_3, n2_3, f1, f2, f3, f4);
  cout << "d3d_feature: " << f1 << " " << f2 << " " << f3 << " " << f4 << endl;

}
int main() {
  test_feature();
  return 0;
}