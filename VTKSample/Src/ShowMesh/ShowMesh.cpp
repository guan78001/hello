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

// For compatibility with new VTK generic data arrays
#ifdef vtkGenericDataArray_h
#define InsertNextTupleValue InsertNextTypedTuple
#endif

int main(int, char *[]) {
  // Setup points
  vtkSmartPointer<vtkPoints> points =
    vtkSmartPointer<vtkPoints>::New();
  points->InsertNextPoint(1.0, 0.0, 0.0);
  points->InsertNextPoint(0.0, 0.0, 0.0);
  points->InsertNextPoint(0.0, 1.0, 0.0);
  points->InsertNextPoint(1.0, 1.0, 0.0);

  // Define some colors
  unsigned char red[3] = { 255, 0, 0 };
  unsigned char green[3] = { 0, 255, 0 };
  unsigned char blue[3] = { 0, 0, 255 };
  unsigned char yellow[3] = { 255, 255, 0 };

  // Setup the colors array
  vtkSmartPointer<vtkUnsignedCharArray> colors =
    vtkSmartPointer<vtkUnsignedCharArray>::New();
  colors->SetNumberOfComponents(3);
  //colors->SetName("Colors");

  // Add the three colors we have created to the array
  colors->InsertNextTupleValue(red);
  colors->InsertNextTupleValue(green);
  colors->InsertNextTupleValue(blue);
  colors->InsertNextTupleValue(yellow);

  // Create a triangle
  vtkSmartPointer<vtkCellArray> triangles =
    vtkSmartPointer<vtkCellArray>::New();
  vtkSmartPointer<vtkTriangle> triangle =
    vtkSmartPointer<vtkTriangle>::New();
  triangle->GetPointIds()->SetId(0, 0);
  triangle->GetPointIds()->SetId(1, 2);
  triangle->GetPointIds()->SetId(2, 1);
  triangles->InsertNextCell(triangle);

  {
    vtkSmartPointer<vtkTriangle> triangle =
      vtkSmartPointer<vtkTriangle>::New();
    triangle->GetPointIds()->SetId(0, 2);
    triangle->GetPointIds()->SetId(1, 0);
    triangle->GetPointIds()->SetId(2, 3);
    triangles->InsertNextCell(triangle);
  }

  // Create a polydata object and add everything to it
  vtkSmartPointer<vtkPolyData> polydata =
    vtkSmartPointer<vtkPolyData>::New();
  polydata->SetPoints(points);
  polydata->SetPolys(triangles);
  polydata->GetPointData()->SetScalars(colors);

  // Set point normals
  vtkSmartPointer<vtkDoubleArray> pointNormalsArray =
    vtkSmartPointer<vtkDoubleArray>::New();
  pointNormalsArray->SetNumberOfComponents(3); //3d normals (ie x,y,z)
  pointNormalsArray->SetNumberOfTuples(polydata->GetNumberOfPoints());

  // Construct the normal vectors
  double pN1[3] = { 1.0, 0.0, 0.0 };
  double pN2[3] = { 0.0, 1.0, 0.0 };
  double pN3[3] = { 0.0, 0.0, 1.0 };
  double pN4[3] = { 1.0, 0.0, 0.0 };

  // Add the data to the normals array
  pointNormalsArray->SetTuple(0, pN1);
  pointNormalsArray->SetTuple(1, pN2);
  pointNormalsArray->SetTuple(2, pN3);
  pointNormalsArray->SetTuple(3, pN4);

  // Add the normals to the points in the polydata
  polydata->GetPointData()->SetNormals(pointNormalsArray);


  // Visualize
  vtkSmartPointer<vtkPolyDataMapper> mapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
#if VTK_MAJOR_VERSION <= 5
  mapper->SetInputConnection(polydata->GetProducerPort());
#else
  mapper->SetInputData(polydata);
#endif

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