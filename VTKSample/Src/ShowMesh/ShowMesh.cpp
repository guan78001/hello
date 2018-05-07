//Add below line if missing include(${VTK_USE_FILE}) in cmake files.
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL);
VTK_MODULE_INIT(vtkInteractionStyle);

#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkCellArray.h>
#include <vtkTriangle.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkDoubleArray.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkUnsignedIntArray.h>
#include <vtkFloatArray.h>
#include <vtkPLYReader.h>
// For compatibility with new VTK generic data arrays
#ifdef vtkGenericDataArray_h
#define InsertNextTupleValue InsertNextTypedTuple
#endif

struct Mesh {
  std::vector<float> verts;
  std::vector<float> normals;
  std::vector<unsigned char> colors;
  std::vector<int> faces;
};

vtkSmartPointer<vtkPoints> GetPoints(const float *verts, int nPoints) {
  vtkSmartPointer<vtkPoints> points =
    vtkSmartPointer<vtkPoints>::New();
  for (int i = 0; i < nPoints; ++i, verts += 3) {
    points->InsertNextPoint(verts[0], verts[1], verts[2]);
  }
  return points;
}
vtkSmartPointer<vtkUnsignedCharArray> GetColors(const unsigned char *colors, int nPoints) {
  vtkSmartPointer<vtkUnsignedCharArray> colorArray =
    vtkSmartPointer<vtkUnsignedCharArray>::New();
  colorArray->SetNumberOfComponents(3);
  colorArray->SetName("Colors");

  for (int i = 0; i < nPoints; ++i, colors += 3) {
    colorArray->InsertNextTupleValue(colors);
  }

  return colorArray;
}

vtkSmartPointer<vtkCellArray> GetTriangles(const int *ids, int nFaces) {
  vtkSmartPointer<vtkCellArray> triangles =
    vtkSmartPointer<vtkCellArray>::New();

  for (int i = 0; i < nFaces; ++i, ids += 3) {
    vtkSmartPointer<vtkTriangle> triangle =
      vtkSmartPointer<vtkTriangle>::New();
    triangle->GetPointIds()->SetId(0, ids[0]);
    triangle->GetPointIds()->SetId(1, ids[1]);
    triangle->GetPointIds()->SetId(2, ids[2]);

    triangles->InsertNextCell(triangle);
  }

  return triangles;
}

vtkSmartPointer<vtkDoubleArray> GetNormals(const float *normals, int nPoints) {
  vtkSmartPointer<vtkDoubleArray> pointNormalsArray =
    vtkSmartPointer<vtkDoubleArray>::New();
  pointNormalsArray->SetNumberOfComponents(3); //3d normals (ie x,y,z)
  pointNormalsArray->SetNumberOfTuples(nPoints);

  for (int i = 0; i < nPoints; ++i, normals += 3) {
    pointNormalsArray->SetTuple(i, normals);
  }

  return pointNormalsArray;
}

vtkSmartPointer<vtkPolyData> GetPolyData(const Mesh &mesh) {
  vtkSmartPointer<vtkPolyData> polydata =
    vtkSmartPointer<vtkPolyData>::New();
  int nPoints = (int)mesh.verts.size() / 3;
  int nFaces = (int)mesh.faces.size() / 3;
  polydata->SetPoints(GetPoints(&mesh.verts[0], nPoints));
  polydata->SetPolys(GetTriangles(&mesh.faces[0], nFaces));
  if (mesh.colors.size() > 0) {
    polydata->GetPointData()->SetScalars(GetColors(&mesh.colors[0], nPoints));
  }
  if (mesh.normals.size() > 0) {
    polydata->GetPointData()->SetNormals(GetNormals(&mesh.normals[0], nPoints));
  }

  return polydata;
}

Mesh GetSimpleMesh() {
  Mesh mesh;
  mesh.verts = { 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0 };
  mesh.colors = { 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 0};
  mesh.normals = {0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1};
  mesh.faces = {0, 2, 1, 2, 0, 3};
  return mesh;
}

Mesh GetMesh(const vtkSmartPointer<vtkPolyData> &polyData) {
  Mesh mesh;
  vtkPoints *pointsArray = polyData->GetPoints();
  int n = pointsArray->GetNumberOfPoints();
  mesh.verts.resize(n * 3);
  for (int i = 0; i < n; ++i) {
    double x[3];
    polyData->GetPoint(i, x);
    mesh.verts[3 * i] = x[0];
    mesh.verts[3 * i + 1] = x[1];
    mesh.verts[3 * i + 2] = x[2];
  }

  //face
  vtkCellArray *pFaces = polyData->GetPolys();
  vtkIdType nFace = pFaces->GetNumberOfCells();
  mesh.faces.resize(3 * nFace);
#if 0
  pFaces->InitTraversal();
  vtkSmartPointer<vtkIdList> idList = vtkSmartPointer<vtkIdList>::New();
  for (vtkIdType i = 0; i < nFace; ++i) {
    pFaces->GetNextCell(idList);
    mesh.faces[3 * i] = idList->GetId(0);
    mesh.faces[3 * i + 1] = idList->GetId(1);
    mesh.faces[3 * i + 2] = idList->GetId(2);
  }
#else
  vtkIdType cellLocation = 0; // the index into the cell array
  for (vtkIdType i = 0; i < nFace; ++i) {
    vtkIdType numIds;
    vtkIdType *pointIds;

    pFaces->GetCell(cellLocation, numIds, pointIds);
    cellLocation += 1 + numIds;

    mesh.faces[3 * i] = pointIds[0];
    mesh.faces[3 * i + 1] = pointIds[1];
    mesh.faces[3 * i + 2] = pointIds[2];
  }
#endif

  //color
  vtkDataArray *colors = polyData->GetPointData()->GetScalars();
  if (colors) {
    vtkIdType nColors = colors->GetNumberOfTuples();
    mesh.colors.resize(nColors * 3);
    for (vtkIdType i = 0; i < nColors; ++i) {
      for (int j = 0; j < 3; j++) {
        mesh.colors[3 * i + j] = colors->GetComponent(i, j);
      }
    }
  }

  //normal
  vtkDataArray *normals = polyData->GetPointData()->GetNormals();
  if (normals) {
    vtkIdType nNormals = normals->GetNumberOfTuples();
    mesh.normals.resize(nNormals);
    for (vtkIdType i = 0; i < nNormals; i++) {
      for (int j = 0; j < 3; j++) {
        mesh.normals[3 * i + j] = normals->GetComponent(i, j);
      }
    }
  }
  return mesh;
}
int main(int argc, char *argv[]) {
  vtkSmartPointer<vtkPolyData>  polydata = GetPolyData(GetSimpleMesh());

  if (argc > 1) {
    cout << argv[1] << endl;
    vtkSmartPointer<vtkPLYReader> reader =
      vtkSmartPointer<vtkPLYReader>::New();
    reader->SetFileName(argv[1]);
    reader->Update();
    polydata = vtkSmartPointer<vtkPolyData>(reader->GetOutput());

    Mesh mesh = GetMesh(polydata);
    polydata = GetPolyData(mesh);
  }
  // Visualize
  vtkSmartPointer<vtkPolyDataMapper> mapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();

  mapper->SetInputData(polydata);

  vtkSmartPointer<vtkActor> actor =
    vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);

  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  renderer->AddActor(actor);

  renderWindow->Render();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}