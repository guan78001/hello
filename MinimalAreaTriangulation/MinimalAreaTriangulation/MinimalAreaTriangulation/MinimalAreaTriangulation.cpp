#include "MAT.h"

#include <vector>
#include <iostream>
void print(std::vector<TriangleIndex> &triangles) {
  for (int i = 0; i < triangles.size(); i++) {
    std::cout << triangles[i].idx[0] << " " << triangles[i].idx[1] << " " << triangles[i].idx[2] << std::endl;
  }
}
int main() {
  typedef float T;
  MinimalAreaTriangulation<T> m;
  typedef Point3D<T> PointType;

  //Point points[] = { { 0, 0, 0 }, { 0, 0, 1 }, { 1, 0, 0 }, { 1, 1, 0 }, { 0, 1, 0 }};
  std::vector<PointType> vertices(5);
  vertices[0] = PointType(0.0, 0.0, 0.0);
  vertices[1] = PointType(0.0, 0.0, 1.0);
  vertices[2] = PointType(1.0, 0.0, 0.0);
  vertices[3] = PointType(1.0, 1.0, 0.0);
  vertices[4] = PointType(0.0, 1.0, 0.0);


  std::vector<TriangleIndex> triangles;
  m.GetTriangulation(vertices, triangles);
  print(triangles);
  return 0;
}