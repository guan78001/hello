// Recursive implementation for minimum cost convex polygon triangulation
#include <iostream>
#include <map>
#include <cmath>
#define MAX 1000000.0
using namespace std;

// Structure of a point in 2D plane
struct Point {
  int x, y, z;
};

// Utility function to find minimum of two double values
double min(double x, double y) {
  return (x <= y) ? x : y;
}

// A utility function to find distance between two points in a plane
double dist(Point p1, Point p2) {
  double len = sqrt((p1.x - p2.x) * (p1.x - p2.x) +
                    (p1.y - p2.y) * (p1.y - p2.y) +
                    (p1.z - p2.z) * (p1.z - p2.z) );
  return len;
}

// A utility function to find cost of a triangle. The cost is considered
// as perimeter (sum of lengths of all edges) of the triangle
double cost(Point points[], int i, int j, int k) {
  Point p1 = points[i], p2 = points[j], p3 = points[k];
  return dist(p1, p2) + dist(p2, p3) + dist(p3, p1);
}

// A recursive function to find minimum cost of polygon triangulation
// The polygon is represented by points[i..j].
typedef std::map<std::pair<int, int>, int> MidPoints;
double mTC(Point points[], int i, int j, MidPoints &midpoints) {
  // There must be at least three points between i and j
  // (including i and j)
  if (j < i + 2)
    return 0;

  // Initialize result as infinite
  double res = MAX;

  // Find minimum triangulation by considering all
  for (int k = i + 1; k < j; k++) {
    //Todo: record intermediate result to reduce computation.
    double temp = (mTC(points, i, k, midpoints) + mTC(points, k, j, midpoints) + cost(points, i, k, j));
    if (temp < res ) {
      res = temp;
      midpoints[std::make_pair(i, j)] = k;
    }
  }

  return  res;
}
void print(MidPoints &midpoints, int i, int j) {
  if (i + 2 > j) return;
  int k = midpoints[std::make_pair(i, j)];
  printf("\n%d %d %d", i, k, j);
  print(midpoints, i, k);
  print(midpoints, k, j);
}
// Driver program to test above functions
int main() {
  //Point points[] = { { 0, 0, 0 }, { 0, 0, 1 }, { 1, 0, 0 }, { 1, 1, 0 }, { 0, 1, 0 }};
  Point points[] = { { 0, 0, 0 }, { 100, 0, 0 }, { 3, 2, 0 }, { 0, 100, 0 }, { 2, 3, 0 }, };
  int n = sizeof(points) / sizeof(points[0]);
  n = 5;
  MidPoints midpoints;
  cout << mTC(points, 0, n - 1, midpoints);
  print(midpoints, 0, n - 1);
  printf("\n");
  return 0;
}