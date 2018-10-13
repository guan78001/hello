#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL);
VTK_MODULE_INIT(vtkInteractionStyle);

#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCamera.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkCubeSource.h>
#include <vtkConeSource.h>
VTK_MODULE_INIT(vtkRenderingFreeType);

#include <vtkMatrix4x4.h>

void PrintMatrix(double m[16], char *matrix = "") {
  cout << matrix << " matrix:\n";
  for (int i = 0; i < 16; i++) {
    if (i % 4 == 0) cout << endl;
    cout << " " << m[i];
  }
  cout << endl;
}
vtkSmartPointer<vtkMatrix4x4> GetMatrix(double m[16]) {
  vtkSmartPointer<vtkMatrix4x4> mat = vtkSmartPointer<vtkMatrix4x4>::New();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      mat->Element[i][j] = m[4 * i + j];
    }
  }
  return mat;
}

vtkSmartPointer<vtkActor> GetCube() {
  //create a cube
  vtkSmartPointer<vtkCubeSource> cubeSource = vtkSmartPointer<vtkCubeSource>::New();
  cubeSource->SetXLength(2);
  cubeSource->SetYLength(3);
  cubeSource->SetZLength(4);
  cubeSource->Update();

  // Create a mapper and actor
  vtkSmartPointer<vtkPolyDataMapper> mapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(cubeSource->GetOutputPort());

  vtkSmartPointer<vtkActor> actor =
    vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);

  return actor;
}

vtkSmartPointer<vtkActor> GetCone() {
  vtkSmartPointer<vtkConeSource> source = vtkSmartPointer<vtkConeSource>::New();
  source->Update();

  // Create a mapper and actor
  vtkSmartPointer<vtkPolyDataMapper> mapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(source->GetOutputPort());

  vtkSmartPointer<vtkActor> actor =
    vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);

  return actor;
}

int main(int, char *[]) {
  //create a cube
  auto cube = GetCube();
  auto cone = GetCone();

  vtkSmartPointer<vtkActor> actor0 =
    vtkSmartPointer<vtkActor>::New();
  double m[16];
  actor0->GetMatrix(m);
  PrintMatrix(m, "default");
  actor0->RotateX(30);

  actor0->GetMatrix(m);
  PrintMatrix(m, "rotatex-30");

  auto m2 = GetMatrix(m);
  vtkSmartPointer<vtkMatrix4x4> mat = vtkSmartPointer<vtkMatrix4x4>::New();
  //actor->RotateY(10);

  // Create a renderer, render window, and interactor
  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();

  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  // Add the actor to the scene
  renderer->AddActor(cone);
  renderer->AddActor(cube);
  renderer->SetBackground(1, 1, 1); // Background color white

  //Axes
  vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();
  vtkSmartPointer<vtkOrientationMarkerWidget> widget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
  widget->SetOutlineColor(0.9300, 0.5700, 0.1300);
  widget->SetOrientationMarker(axes);
  widget->SetInteractor(renderWindowInteractor);
  widget->SetViewport(0, 0, 0.4, 0.4);
  widget->SetEnabled(1);
  //widget->InteractiveOn();//moveable
  widget->InteractiveOff();

  renderer->SetBackground(0.1804, 0.5451, 0.3412); // Sea green
  renderer->ResetCamera();
  //renderer->ResetCamera(
  //  -2, 2,
  //  -2, 2,
  //  -2, 2);
  // Render and interact
  renderWindow->Render();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}