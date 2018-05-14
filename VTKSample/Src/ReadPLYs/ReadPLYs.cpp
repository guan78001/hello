#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);

#include <vtkPolyData.h>
#include <vtkPLYReader.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTerrain.h>
#include <vtkTransform.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkCamera.h>
#include <sstream>
vtkSmartPointer<vtkActorCollection> GetActors(const std::vector<std::string> &filenames) {

  vtkSmartPointer<vtkActorCollection> actorCollection =
    vtkSmartPointer<vtkActorCollection>::New();

  for (const std::string &inputFilename : filenames) {
    vtkSmartPointer<vtkPLYReader> reader =
      vtkSmartPointer<vtkPLYReader>::New();
    reader->SetFileName(inputFilename.c_str());

    // Visualize
    vtkSmartPointer<vtkPolyDataMapper> mapper =
      vtkSmartPointer<vtkPolyDataMapper>::New();

    mapper->SetInputConnection(reader->GetOutputPort());

    vtkSmartPointer<vtkActor> actor =
      vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    actorCollection->AddItem(actor);
  }

  return actorCollection;
}

std::vector<vtkSmartPointer<vtkActor>> GetActors2(const std::vector<std::string> &filenames) {

  std::vector<vtkSmartPointer<vtkActor>> vec;

  for (const std::string &inputFilename : filenames) {
    vtkSmartPointer<vtkPLYReader> reader =
      vtkSmartPointer<vtkPLYReader>::New();
    reader->SetFileName(inputFilename.c_str());

    // Visualize
    vtkSmartPointer<vtkPolyDataMapper> mapper =
      vtkSmartPointer<vtkPolyDataMapper>::New();

    mapper->SetInputConnection(reader->GetOutputPort());

    vtkSmartPointer<vtkActor> actor =
      vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    vec.push_back(actor);
  }

  return vec;
}

std::vector<std::string> GetFileNames(const char *path, int n) {
  std::vector<std::string> filenames;
  std::stringstream ss;
  for (int i = 1; i <= n; ++i) {
    //ss.str("");
    //ss.clear();
    ss = std::stringstream();
    ss << path << "\\" << i << ".ply";
    filenames.push_back(ss.str());
  }

  //test
  for (auto item : filenames) {
    cout << item << endl;
  }
  return filenames;
}
int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << "  Filename(.ply)" << std::endl;
    return EXIT_FAILURE;
  }
  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  std::vector<std::string> files = GetFileNames(argv[1], 3);
  //transform->PostMultiply(); //this is the key line

  vtkSmartPointer<vtkActorCollection> actors = GetActors(files);

  double origin_normal[3] = { 0, 0, 1 };
  double position[3][4];
  double center_point[3][4];

  actors->InitTraversal();
  for (vtkIdType i = 0; i < actors->GetNumberOfItems(); i++) {
    vtkSmartPointer<vtkTransform> dir = vtkSmartPointer<vtkTransform>::New();
    dir->Translate(origin_normal);

    vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
    vtkActor *actor = actors->GetNextActor();
    //transform->RotateY(i * 10);
    transform->Translate(i * 5, 0, 0);
    actor->SetUserTransform(transform);

    double *bounds = actor->GetBounds();
    printf("actor_%d, [%f,%f ; %f,%f ;%f,%f]\n", i, bounds[0], bounds[1], bounds[2], bounds[3], bounds[4], bounds[5]);
    double *center = center_point[i];
    for (int t = 0; t < 3; ++t) {
      center[t] = (bounds[2 * t] + bounds[2 * t + 1]) * 0.5;
    }
    printf("center: [%f,%f,%f]\n", center[0], center[1], center[2]);

    dir->TransformPoint(center_point[i], position[i]);
    printf("position: [%f,%f,%f]\n", position[i][0], position[i][1], position[i][2]);

    renderer->AddActor(actor);
  }

  vtkSmartPointer<vtkInteractorStyleTerrain> style =
    vtkSmartPointer<vtkInteractorStyleTerrain>::New();

  renderWindowInteractor->SetInteractorStyle(style);

  vtkSmartPointer<vtkAxesActor> axes =
    vtkSmartPointer<vtkAxesActor>::New();

  vtkSmartPointer<vtkOrientationMarkerWidget> widget =
    vtkSmartPointer<vtkOrientationMarkerWidget>::New();
  widget->SetOutlineColor(0.9300, 0.5700, 0.1300);
  widget->SetOrientationMarker(axes);
  widget->SetInteractor(renderWindowInteractor);
  widget->SetViewport(0.0, 0.0, 0.3, 0.3);
  widget->SetEnabled(1);
  widget->InteractiveOn();//moveable
  widget->InteractiveOff();

  vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();
  int t = 0;
  //camera->SetPosition(45, 15, 10);
  //camera->SetFocalPoint(focal_point[t]);

  camera->SetPosition(position[t]);
  camera->SetFocalPoint(center_point[t]);

  renderer->SetActiveCamera(camera);
  renderer->ResetCamera();

  renderer->SetBackground(0.1804, 0.5451, 0.3412); // Sea green
  renderWindow->SetSize(600, 600);
  renderWindow->Render();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}