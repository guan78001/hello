#define _CRT_SECURE_NO_WARNINGS
#include <map>
#include <vector>
using namespace std;


pair<int, int> Pair(int a, int b) {
  return std::make_pair(a, b);
}
void test1() {
  printf("test1:\n");
  map<pair<int, int>, int> m;

  const int nPairs = 4*3;
  vector<pair<int, int>> Pairs(nPairs);
  Pairs[0] = Pair(0, 1);
  Pairs[1] = Pair(1, 2);
  Pairs[2] = Pair(2, 0);

  Pairs[3] = Pair(0, 2);
  Pairs[4] = Pair(2, 3);
  Pairs[5] = Pair(3, 0);

  Pairs[6] = Pair(1, 4);
  Pairs[7] = Pair(4, 2);
  Pairs[8] = Pair(2, 1);

  Pairs[9] = Pair(2, 4);
  Pairs[10] = Pair(4, 3);
  Pairs[11] = Pair(3, 2);

  for (int i = 0; i < nPairs; i++) {
    int v0 = Pairs[i].first;
    int v1 = Pairs[i].second;
#if 1
    m[Pair(v0, v1)] += 1;
    m[Pair(v1, v0)] -= 1;
#else
    if (v0 < v1) {
      m[Pair(v0, v1)] += 1;
      m[Pair(v1, v0)] -= 1;
    } else {
      m[Pair(v0, v1)] -= 1;
      m[Pair(v1, v0)] += 1;
    }
#endif
  }
  for (auto item : m) {
    if (item.second == 1) {
      printf("(%d,%d)=%d\n", item.first.first, item.first.second, item.second);
    }
  }
}

//version 2
template <class T>
struct Point3 {
  union {
    struct {
      T x, y, z;
    };
    T p[3];
  };
  Point3(T _x, T _y, T _z) {
    x = _x;
    y = _y;
    z = _z;
  }
  Point3(T _p[3]) {
    p[0] = _p[0];
    p[1] = _p[1];
    p[2] = _p[2];
  }

  T * operator()() {
    return p;
  }
  T * operator()() const {
    return p;
  }
  T & operator[](int idx) {
    return p[idx];
  }
  const T  operator[](int idx) const {
    return p[idx];
  }
  bool operator< (const Point3 &rhs)const {
    return (x < rhs.x ||
            (/*!(x < rhs.x) &&*/ y < rhs.y)||
            z<rhs.z);
  }
};
template <class T>
struct Point2 {
  union {
    struct {
      T x, y;
    };
    T p[2];
  };
  Point2(T _x, T _y) {
    x = _x;
    y = _y;
  }
  Point2(T _p[2]) {
    p[0] = _p[0];
    p[1] = _p[1];
  }

  T * operator()() {
    return p;
  }
  T * operator()() const {
    return p;
  }
  T & operator[](int idx) {
    return p[index];
  }
  const T  operator[](int idx) const {
    return p[idx];
  }
  bool operator< (const Point2 &rhs)const {
    //return make_pair(x, y) < make_pair(rhs.x, rhs.y);
    if (x < rhs.x) return true;
    if (x == rhs.x & y<rhs.y) return true;
    return false;

    return (x < rhs.x ||
            (/*!(x < rhs.x) &&*/ y<rhs.y));
  }
};

typedef Point3< float > Point3f;
typedef Point3< int > Triangle;
typedef Point2< int > Edge;

struct Mesh {
  Mesh() {}
  Mesh(const std::vector<Triangle> &_triangles, const std::vector<Point3f> &_points) {
    triangles = _triangles;
    points = _points;
  }
  std::vector<Triangle> triangles;
  std::vector<Point3f> points;
  void Write(const std::string &filename,bool isAsciiMode=true) {
    if (isAsciiMode) {
      FILE* fp = fopen(filename.c_str(), "w");
      fprintf(fp, "ply\n");
      fprintf(fp, "format ascii 1.0\n");
      fprintf(fp, "comment VCGLIB generated\n");
      fprintf(fp, "element vertex %d\n",points.size());
      fprintf(fp, "property float x\n");
      fprintf(fp, "property float y\n");
      fprintf(fp, "property float z\n");
      fprintf(fp, "element face %d\n", triangles.size());
      fprintf(fp, "property list uchar int vertex_indices\n");
      fprintf(fp, "end_header\n");

      for (int i = 0; i < points.size(); i++) {
        fprintf(fp,"%f %f %f\n", points[i][0], points[i][1], points[i][2]);
      }
      for (int i = 0; i < triangles.size(); i++) {
        fprintf(fp, "3 %d %d %d\n", triangles[i][0], triangles[i][1], triangles[i][2]);
      }
      fclose(fp);
    } else {
      FILE* fp = fopen(filename.c_str(), "wb");
      fprintf(fp, "ply\n");
      fprintf(fp, "format binary_little_endian 1.0\n");
      fprintf(fp, "comment VCGLIB generated\n");
      fprintf(fp, "element vertex %d\n", points.size());
      fprintf(fp, "property float x\n");
      fprintf(fp, "property float y\n");
      fprintf(fp, "property float z\n");
      fprintf(fp, "element face %d\n", triangles.size());
      fprintf(fp, "property list uchar int vertex_indices\n");
      fprintf(fp, "end_header\n");

      int nPoints = points.size();
      if (nPoints>0) {
        fwrite(&points[0], 3 * sizeof(float), nPoints, fp);
      }

      for (int i = 0; i < triangles.size(); i++) {
        unsigned char attribute = 3;
        fwrite(&attribute, sizeof(attribute), 1, fp);
        fwrite(&triangles[i], 3 * sizeof(int), 1, fp);
      }

      fclose(fp);
    }
  }
  void Read(const std::string &filename) {

  }
};
Edge GetEdge(const Triangle &tri, int index) {
  return Edge(tri[index], tri[(index + 1)%3]);
}
Edge GetReverseEdge(const Edge& edge) {
  return Edge(edge.y,edge.x);
}
void test2() {
  printf("test2:\n");
  vector<Triangle>tris {
    { 0, 1, 2 },
    { 0, 2, 3 },
    { 1, 4, 2 },
    { 2, 4, 3 },
  };
  vector<Point3f> points = {
    {0,1,0},
    {0,0,0},
    {0.5,0.5,0},
    {1.0,1.0,0},
    {1,0,0}
  };
  Mesh mesh(tris, points);
  mesh.Write("d:\\temp\\mesh\\sample.ply");
  mesh.Write("d:\\temp\\mesh\\sample_bin.ply",false);
  map<Edge, int> edges;
  for (auto tri:tris) {
    for (int j = 0; j < 3; j++) {
      Edge e = GetEdge(tri, j);
      edges[e] += 1;
      edges[GetReverseEdge(e)] -= 1;
    }
  }
  for (auto item: edges) {
    if (item.second==1) {
      printf("(%d,%d)=%d\n", item.first[0], item.first[1], item.second);
    }
  }
};

int main() {
  test1();

  test2();
  return 0;
}