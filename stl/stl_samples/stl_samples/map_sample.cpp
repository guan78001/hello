#define _CRT_SECURE_NO_WARNINGS
#include <map>
#include <vector>
#include <queue>
#include <set>
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
  Point3(const Point3 &rhs) {
    p[0] = rhs.p[0];
    p[1] = rhs.p[1];
    p[2] = rhs.p[2];
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

  Point3  operator + (const Point3 &rhs) {
    Point3 obj(*this);
    for (int i = 0; i < 3; i++) {
      obj.p[i] += rhs[i];
    }
    return obj;
  }
  Point3  operator - (const Point3 &rhs) {
    Point3 obj(*this);
    for (int i = 0; i < 3; i++) {
      obj.p[i] -= rhs[i];
    }
    return obj;
  }
  Point3 operator *(float ratio) {
    Point3 obj(*this);
    for (int i = 0; i < 3; i++) {
      obj.p[i] *= ratio;
    }
    return obj;
  }
  float NormL2() {
    return sqrt(p[0] * p[0] + p[1] * p[1] + p[2] * p[2]);
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
typedef Point3< int > Face;
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
struct EdgeInfo {
  int v0, v1;
  int t0, t1;
  float length;
  EdgeInfo(int _v0, int _v1, float _len) {
    v0 = _v0;
    v1 = _v1;
    length = _len;
  }
  EdgeInfo() {
    v0 = v1 = -1;
    length = 0;
  }

  bool operator<(const EdgeInfo &rhs) const {
    return length < rhs.length;
  }
};
int index(int v0, const Face &t) {
  for (int i = 0; i < 3; i++) {
    if (t[i] == v0) return i;
  }
  printf("[Bug] index(%d,T(%d,%d,%d)\n",v0,t[0],t[1],t[2]);
  return -1;
}
int NextVertex(int v0, const Face &t) {
  int id = index(v0, t);
  return t[(id + 1) % 3];
}
float length(Point3f v0, Point3f v1) {
  Point3f v = v0 - v1;
  return v.NormL2();
}
struct T0T1 {
  int t0, t1;
  T0T1() {}
  T0T1(int _t0, int _t1) {
    t0 = _t0;
    t1 = _t1;
  }
};

void DenseMesh(vector<Point3f>&verts, vector<Face> &faces, priority_queue<EdgeInfo> &edges, map<Edge, float> &edgeTris, float minLength) {
  while (!edges.empty()) {
    EdgeInfo &e = edges.top();
    edges.pop();
    int v0 = e.v0;
    int v1 = e.v1;
    int t0 = edgeTris[Edge(v0, v1)];
    int t1 = edgeTris[Edge(v1, v0)];

    //add mid point
    int v4 = verts.size();
    Point3f pv4 = (verts[v0] - verts[v1])*0.5f;
    verts.push_back(pv4);

    int v2 = NextVertex(v1, faces[t0]);
    int v3 = NextVertex(v0,faces[t1]);

    //new faces
    int t2 = faces.size();
    faces.push_back(Face(v1, v2, v4));
    int t3 = faces.size();
    faces.push_back(Face(v3, v1, v4));

    //change original faces vertex id
    faces[t1][index(v1, faces[t1])] = v4;
    faces[t0][index(v1, faces[t0])] = v4;

    //add edges
    edgeTris[Edge(v0, v4)] = t0;
    edgeTris[Edge(v4, v0)] = t1;

    edgeTris[Edge(v3, v4)] = t1;
    edgeTris[Edge(v4, v3)] = t3;

    edgeTris[Edge(v4, v1)] = t2;
    edgeTris[Edge(v1, v4)] = t3;

    edgeTris[Edge(v4, v2)] = t0;
    edgeTris[Edge(v2, v4)] = t2;

    float len1 = length(verts[v0], verts[v4]);
    if (len1>minLength) {
      edges.push(EdgeInfo(v0, v4, len1));
      edges.push(EdgeInfo(v1, v4, len1));
    }
    float len2 = length(verts[v3], verts[v4]);
    if (len2>minLength) {
      edges.push(EdgeInfo(v3, v4, len2));
    }
    float len3 = length(verts[v2], verts[v4]);
    edges.push(EdgeInfo(v2, v4, len3));
  }
}
void ConstructTables(vector<Triangle>&faces, vector<Point3f>& points, float minLength, priority_queue<EdgeInfo>& edges, map<Edge, float>& edgeTris) {
  for (int fId = 0; fId < faces.size(); fId++) {
    Face &f = faces[fId];
    const int id[4] = { 0, 1, 2, 0 };
    for (int i = 0; i < 3; i++) {
      edgeTris[Edge(f[id[i]], f[id[i + 1]])] = fId;
      if (edgeTris.find(Edge(f[id[i+1]],f[id[i]]))==edgeTris.end()) {
        float len = length(points[f[id[i]]], points[f[id[i + 1]]]);
        if (len > minLength) {
          edges.push(EdgeInfo(f[id[i]], f[id[i + 1]], len));
        }
      }
    }
  }
}
void test_DenseMesh() {
  vector<Triangle>faces{
    { 0, 1, 2 },
    { 0, 2, 3 }
  };
  vector<Point3f> points = {
    { 0, 1, 0 },
    { 0, 0, 0 },
    { 1, 0, 0 },
    { 1.0, 1.0, 0 },
  };
  float minLength = 0.5f;
  //Construct
  priority_queue<EdgeInfo> edges;
  map<Edge, float> edgeTris;
  //ConstructTables(faces, points, minLength, edges, edgeTris);
  edgeTris[Edge(0,1)] = 0;
  edgeTris[Edge(1,2)] = 0;
  edgeTris[Edge(2,0)] = 0;

  edgeTris[Edge(0,3)] = 1;
  edgeTris[Edge(3,1)] = 1;
  edgeTris[Edge(1,0)] = 1;

  edges.push(EdgeInfo(0, 1, length(points[0], points[1])));
  DenseMesh(points, faces, edges, edgeTris, minLength);
  Mesh m(faces, points);
  m.Write("d:\\temp\\mesh\\dense.ply");
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
  test_DenseMesh();
  return 0;
  test1();
  test2();
  return 0;
}