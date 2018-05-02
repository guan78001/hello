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
  vtkSmartPointer<vtkTransform> transform =
    vtkSmartPointer<vtkTransform>::New();
  //transform->PostMultiply(); //this is the key line

#if 1
  vtkSmartPointer<vtkActorCollection> actors = GetActors(files);
  actors->InitTraversal();
  double x = 1.0f;
  transform->Identity();
  for (vtkIdType i = 0; i < actors->GetNumberOfItems(); i++) {
    vtkActor *actor = actors->GetNextActor();
    //renderer->AddActor(actor);
    //transform->Identity();
    if (i > 0) {
      transform->Translate(0, i * i * 10, 0);
      //transform->RotateX(i * 40);
      actor->SetUserTransform(transform);
    }

    renderer->AddActor(actor);
  }
#else
  auto acotrs = GetActors2(files);
  double x = 1.0f;
  for (auto &actor : acotrs) {
    x += 20.0f;
    transform->Translate(x, 0, 0);
    actor->SetUserTransform(transform);
    renderer->AddActor(actor);
  }
#endif

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
  widget->SetViewport(0.0, 0.0, 0.4, 0.4);
  widget->SetEnabled(1);
  widget->InteractiveOn();//moveable
  widget->InteractiveOff();

  renderer->ResetCamera();

  renderer->SetBackground(0.1804, 0.5451, 0.3412); // Sea green
  renderWindow->SetSize(600, 600);
  renderWindow->Render();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}