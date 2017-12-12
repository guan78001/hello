#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
using namespace std;

std::string path = "d:\\temp\\CarveTextOnMesh\\";
const std::string FullName(const std::string filename) {
  return path + filename;
}

struct Mesh {
  Mesh() {}
  Mesh(const std::vector<int> &_triangles, const std::vector<float> &_points) {
    triangles = _triangles;
    points = _points;
  }
  std::vector<int> triangles;
  std::vector<float> points;
  void Write(const std::string &filename, bool isAsciiMode = true) {
    int nPoints = points.size() / 3;
    int nFaces = triangles.size() / 3;
    if (isAsciiMode) {
      FILE *fp = fopen(filename.c_str(), "w");
      fprintf(fp, "ply\n");
      fprintf(fp, "format ascii 1.0\n");
      fprintf(fp, "comment VCGLIB generated\n");
      fprintf(fp, "element vertex %d\n", points.size() / 3);
      fprintf(fp, "property float x\n");
      fprintf(fp, "property float y\n");
      fprintf(fp, "property float z\n");
      fprintf(fp, "element face %d\n", triangles.size() / 3);
      fprintf(fp, "property list uchar int vertex_indices\n");
      fprintf(fp, "end_header\n");

      for (int i = 0; i < nPoints; i++) {
        fprintf(fp, "%f %f %f\n", points[3 * i], points[3 * i + 1], points[3 * i + 2]);
      }
      for (int i = 0; i < nFaces; i++) {
        fprintf(fp, "3 %d %d %d\n", triangles[3 * i + 0], triangles[3 * i + 1], triangles[3 * i + 2]);
      }
      fclose(fp);
    } else {
      FILE *fp = fopen(filename.c_str(), "wb");
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

      if (nPoints > 0) {
        fwrite(&points[0], sizeof(float), nPoints, fp);
      }

      for (int i = 0; i < nFaces; i++) {
        unsigned char attribute = 3;
        fwrite(&attribute, sizeof(attribute), 1, fp);
        fwrite(&triangles[3 * i], 3 * sizeof(int), 1, fp);
      }

      fclose(fp);
    }
  }
  void Read(const std::string &filename) {

  }
};

struct BMP {
  int width = 0;
  int height = 0;
  int channel = 3;
  unsigned char *data = nullptr;
  void Read(const char *filename) {
    FILE *f = fopen(filename, "rb");
    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

    // extract image height and width from header
    width = *(int *)&info[18];
    height = *(int *)&info[22];
    channel = 3;

    int size = channel * width * height;

    delete[] data;
    data = new unsigned char[size]; // allocate 3 bytes per pixel
    fread(data, sizeof(unsigned char), size, f); // read the rest of the data at once
    fclose(f);
  }
  ~BMP() {
    delete[] data;
  }
};

void GeneratePlaneMesh(const BMP &bmp, Mesh &mesh) {
  std::vector<float> &vPoints = mesh.points;
  std::vector<int> &tris = mesh.triangles;

  int height = bmp.height;
  int width = bmp.width;
  int cn = bmp.channel;
  unsigned char *data = bmp.data;

  float pixelLength = 0.1f;
  float zDepth = -1.0f;

  //fill points and carve.
  for (int h = 0; h <= height; h++) {
    for (int w = 0; w <= width; w++) {
      float z = zDepth;
      if (h == height || w == width || data[h * width * cn + w * cn + 1] > 128) {
        z = 0;
      }
      float x = w * pixelLength;
      float y = h * pixelLength;
      vPoints.push_back(x);
      vPoints.push_back(y);
      vPoints.push_back(z);
    }
  }

  //fill triangles
  for (int h = 0; h < height; h++) {
    for (int w = 0; w < width; w++) {
      int tri1[3];
      int tri2[3];
      //Note: Image coordinate x->, y: up to down, mesh coordiante: x->, y: down to up.
      //inverse triangle here to get front normal.
      tri1[2] = h * (width + 1) + w;
      tri1[1] = (h + 1) * (width + 1) + w;
      tri1[0] = (h + 1) * (width + 1) + w + 1;

      tri2[0] = tri1[0];
      tri2[1] = tri1[2];
      tri2[2] = h * (width + 1) + w + 1;

      for (int i = 0; i < 3; i++) {
        tris.push_back(tri1[i]);
      }

      for (int i = 0; i < 3; i++) {
        tris.push_back(tri2[i]);
      }
    }
  }
}

int main() {
  //1.Read BMP
  BMP bmp;
  bmp.Read(FullName("1.bmp").c_str());

  //2.Generate mesh
  Mesh mesh;
  GeneratePlaneMesh(bmp, mesh);

  mesh.Write(FullName("CarvePlaneMesh2.ply"));

  return 0;
}