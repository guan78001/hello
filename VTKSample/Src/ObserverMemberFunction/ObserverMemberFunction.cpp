#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL);
VTK_MODULE_INIT(vtkInteractionStyle);

#include <vtkPolyDataMapper.h>
#include <vtkObjectFactory.h>
#include <vtkCommand.h>
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyle.h>
#include <vtkPolyData.h>
#include <vtkSphereSource.h>
#include <vtkInteractorStyleTrackballCamera.h>

// A class not derived from vtkObjectBase
class MyClass {
 public:
  void KeypressCallbackFunction(vtkObject *,
                                long unsigned int vtkNotUsed(eventId),
                                void *vtkNotUsed(callData)) {
    std::cout << "Caught event in MyClass" << std::endl;
  }

};

// A class that is derived from vtkObjectBase
class MyInteractorStyle : public vtkInteractorStyleTrackballCamera {
 public:
  static MyInteractorStyle *New();
  vtkTypeMacro(MyInteractorStyle, vtkInteractorStyleTrackballCamera);

  void KeypressCallbackFunction(vtkObject *,
                                long unsigned int vtkNotUsed(eventId),
                                void *vtkNotUsed(callData)) {
    std::cout << "Caught event in MyInteractorStyle" << std::endl;
  }

};
vtkStandardNewMacro(MyInteractorStyle);

int main(int, char *[]) {
  // Create a sphere
  vtkSmartPointer<vtkSphereSource> sphereSource =
    vtkSmartPointer<vtkSphereSource>::New();
  sphereSource->SetCenter(0.0, 0.0, 0.0);
  sphereSource->SetRadius(5.0);
  sphereSource->Update();

  vtkSmartPointer<vtkPolyDataMapper> mapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(sphereSource->GetOutputPort());

  // Create an actor
  vtkSmartPointer<vtkActor> actor =
    vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);

  // A renderer and render window
  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);

  // An interactor
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  MyClass myClass;
  renderWindowInteractor->AddObserver(vtkCommand::KeyPressEvent, &myClass, &MyClass::KeypressCallbackFunction);

  MyInteractorStyle *style = MyInteractorStyle::New();
  renderWindowInteractor->AddObserver(vtkCommand::KeyPressEvent, style, &MyInteractorStyle::KeypressCallbackFunction);

  vtkSmartPointer<MyInteractorStyle> style2 =
    vtkSmartPointer<MyInteractorStyle>::New();
  renderWindowInteractor->AddObserver(vtkCommand::KeyPressEvent, style2, &MyInteractorStyle::KeypressCallbackFunction);

  renderer->AddActor(actor);
  renderer->SetBackground(1, 1, 1); // Background color white
  renderWindow->Render();
  renderWindowInteractor->Start();

  style->Delete();

  return EXIT_SUCCESS;
}

//#include <vtkPolyDataMapper.h>
//#include <vtkObjectFactory.h>
//#include <vtkActor.h>
//#include <vtkSmartPointer.h>
//#include <vtkRenderWindow.h>
//#include <vtkRenderer.h>
//#include <vtkRenderWindowInteractor.h>
//#include <vtkPolyData.h>
//#include <vtkSphereSource.h>
//#include <vtkInteractorStyleTrackballCamera.h>
//
//vtkSmartPointer<vtkSphereSource> g_sphereSource;
//// Define interaction style
//class KeyPressInteractorStyle : public vtkInteractorStyleTrackballCamera {
// public:
//  static KeyPressInteractorStyle *New();
//  vtkTypeMacro(KeyPressInteractorStyle, vtkInteractorStyleTrackballCamera);
//
//  virtual void OnKeyPress() {
//    // Get the keypress
//    vtkRenderWindowInteractor *rwi = this->Interactor;
//    std::string key = rwi->GetKeySym();
//
//    // Output the key that was pressed
//    std::cout << "Pressed " << key << std::endl;
//
//    // Handle an arrow key
//    if (key == "Up") {
//      g_sphereSource->SetRadius(g_sphereSource->GetRadius() + 1);
//      g_sphereSource->Update();
//      std::cout << "The up arrow was pressed." << std::endl;
//      rwi->GetRenderWindow()->Render();
//    } else if (key == "Down") {
//      g_sphereSource->SetRadius(g_sphereSource->GetRadius() - 1);
//      g_sphereSource->Update();
//      std::cout << "The down arrow was pressed." << std::endl;
//      rwi->GetRenderWindow()->Render();
//    }
//    // Handle a "normal" key
//    if (key == "a") {
//      std::cout << "The a key was pressed." << std::endl;
//    }
//
//    // Forward events
//    vtkInteractorStyleTrackballCamera::OnKeyPress();
//  }
//
//};
//vtkStandardNewMacro(KeyPressInteractorStyle);
//
//int main(int, char *[]) {
//  vtkSmartPointer<vtkSphereSource> sphereSource =
//    vtkSmartPointer<vtkSphereSource>::New();
//  sphereSource->SetCenter(0.0, 0.0, 0.0);
//  sphereSource->SetRadius(5.0);
//  sphereSource->Update();
//  g_sphereSource = sphereSource;
//
//  vtkSmartPointer<vtkPolyDataMapper> mapper =
//    vtkSmartPointer<vtkPolyDataMapper>::New();
//  mapper->SetInputConnection(sphereSource->GetOutputPort());
//
//  // Create an actor
//  vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
//  actor->SetMapper(mapper);
//
//  // A renderer and render window
//  vtkSmartPointer<vtkRenderer> renderer =
//    vtkSmartPointer<vtkRenderer>::New();
//  vtkSmartPointer<vtkRenderWindow> renderWindow =
//    vtkSmartPointer<vtkRenderWindow>::New();
//  renderWindow->AddRenderer(renderer);
//
//  // An interactor
//  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
//    vtkSmartPointer<vtkRenderWindowInteractor>::New();
//  renderWindowInteractor->SetRenderWindow(renderWindow);
//
//  vtkSmartPointer<KeyPressInteractorStyle> style =
//    vtkSmartPointer<KeyPressInteractorStyle>::New();
//  renderWindowInteractor->SetInteractorStyle(style);
//  style->SetCurrentRenderer(renderer);
//
//  renderer->AddActor(actor);
//  renderer->SetBackground(.1, .2, .3); // Background color white
//
//  renderWindow->Render();
//
//  renderWindowInteractor->Start();
//
//  return EXIT_SUCCESS;
//}