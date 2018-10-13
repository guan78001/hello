#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL);
VTK_MODULE_INIT(vtkInteractionStyle);

#include <vtkSmartPointer.h>
#include <vtkConeSource.h>
#include <vtkSphereSource.h>
#include <vtkCubeSource.h>
#include <vtkTransform.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkActorCollection.h>
#include <vtkInteractorStyle.h>
#include <vtkCallbackCommand.h>
#include <thread>
#include <vtkInteractorStyleTrackballCamera.h>
#include "vtkTestFilter.h"
#include <windows.h>
#include <vtkWin32OpenGLRenderWindow.h>
void wait(int ms) {
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
class My {
 public:
  static void Hello(vtkObject *caller,
                    long unsigned int eventId,
                    void *clientData,
                    void *callData) {
    My *my = (My *)clientData;
    my->Hello();
  }

  void AddActors() {
    actorCollection->InitTraversal();

    for (vtkIdType i = 0; i < actorCollection->GetNumberOfItems(); i++) {
      vtkActor *actor = actorCollection->GetNextActor();
      actor->SetUserTransform(transform);
      renderer->AddActor(actor);
    }
    // Render and interact
    renderWindow->Render();
  }

  void RemoveActors() {
    actorCollection->InitTraversal();
    for (vtkIdType i = 0; i < actorCollection->GetNumberOfItems(); i++) {
      vtkActor *actor = actorCollection->GetNextActor();
      actor->SetUserTransform(transform);
      renderer->RemoveActor(actor);
    }
    // Render and interact
    renderWindow->Render();
  }
  void Hello() {
    //return;
    for (int i = 0; i < 1; i++) {
      cout << std::this_thread::get_id() << "  " << "My Hello." << endl;
      AddActors();
      wait(100);
      RemoveActors();
      wait(100);
    }

  }
  vtkSmartPointer<vtkActorCollection> actorCollection;
  vtkSmartPointer<vtkRenderWindow> renderWindow;
  vtkSmartPointer<vtkTransform> transform;
  vtkSmartPointer<vtkRenderer> renderer;
};

class KeyPressInteractorStyle : public vtkInteractorStyleTrackballCamera {
 public:
  static KeyPressInteractorStyle *New();
  vtkTypeMacro(KeyPressInteractorStyle, vtkInteractorStyleTrackballCamera);

  virtual void OnKeyPress() {
    // Get the keypress
    vtkRenderWindowInteractor *rwi = this->Interactor;
    std::string key = rwi->GetKeySym();
    std::cout << "OnKeyPress: " << std::this_thread::get_id() << endl;
    // Output the key that was pressed
    std::cout << "Pressed " << key << std::endl;

    // Handle an arrow key
    if (key == "Up") {
      std::cout << GetCurrentThreadId() << ". The up arrow was pressed." << std::endl;
      if (p) {
        p->AddActors();
      }
      rwi->GetRenderWindow()->Render();
    } else if (key == "Down") {
      std::cout << GetCurrentThreadId() << ". The down arrow was pressed." << std::endl;
      if (p) {
        p->RemoveActors();
      }
      rwi->GetRenderWindow()->Render();
    }
    // Handle a "normal" key
    if (key == "a") {
      std::cout << "The a key was pressed." << std::endl;
    }

    // Forward events
    vtkInteractorStyleTrackballCamera::OnKeyPress();
  }
  My *p = NULL;
};


vtkStandardNewMacro(KeyPressInteractorStyle);

void TimerCallbackFunction(vtkObject *caller, long unsigned int vtkNotUsed(eventId), void *clientData, void *vtkNotUsed(callData));
int main(int, char *[]) {
  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();

  // Create a renderer, render window, and interactor

  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  // Add the actor to the scene
  renderer->SetBackground(.2, .3, .4);

  vtkSmartPointer<KeyPressInteractorStyle> style =
    vtkSmartPointer<KeyPressInteractorStyle>::New();
  renderWindowInteractor->SetInteractorStyle(style);
  style->SetCurrentRenderer(renderer);

  // Create a cone
  vtkSmartPointer<vtkConeSource> coneSource =
    vtkSmartPointer<vtkConeSource>::New();
  coneSource->SetHeight(3);

  vtkSmartPointer<vtkPolyDataMapper> coneMapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  coneMapper->SetInputConnection(coneSource->GetOutputPort());
  vtkSmartPointer<vtkActor> coneActor =
    vtkSmartPointer<vtkActor>::New();
  coneActor->SetMapper(coneMapper);

  // Create a cube
  vtkSmartPointer<vtkCubeSource> cubeSource =
    vtkSmartPointer<vtkCubeSource>::New();

  vtkSmartPointer<vtkPolyDataMapper> cubeMapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  cubeMapper->SetInputConnection(cubeSource->GetOutputPort());
  vtkSmartPointer<vtkActor> cubeActor =
    vtkSmartPointer<vtkActor>::New();
  cubeActor->SetMapper(cubeMapper);

  // Create a sphere
  vtkSmartPointer<vtkSphereSource> sphereSource =
    vtkSmartPointer<vtkSphereSource>::New();

  vtkSmartPointer<vtkPolyDataMapper> sphereMapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  sphereMapper->SetInputConnection(sphereSource->GetOutputPort());
  vtkSmartPointer<vtkActor> sphereActor =
    vtkSmartPointer<vtkActor>::New();
  sphereActor->SetMapper(sphereMapper);
  renderer->AddActor(sphereActor);

  vtkSmartPointer<vtkActorCollection> actorCollection =
    vtkSmartPointer<vtkActorCollection>::New();
  actorCollection->AddItem(cubeActor);
  actorCollection->AddItem(coneActor);
  actorCollection->InitTraversal();

  vtkSmartPointer<vtkTransform> transform =
    vtkSmartPointer<vtkTransform>::New();
  transform->PostMultiply(); //this is the key line
  transform->Translate(5.0, 0, 0);

  cout << "Main thread: " << std::this_thread::get_id() << endl;

  My my;
  my.actorCollection = actorCollection;
  my.transform = transform;
  my.renderWindow = renderWindow;
  my.renderer = renderer;
  style->p = &my;
  my.AddActors();

  //vtkSmartPointer<vtkTestFilter> filter =
  //  vtkSmartPointer<vtkTestFilter>::New();

  //vtkSmartPointer<vtkCallbackCommand> callback =
  //  vtkSmartPointer<vtkCallbackCommand>::New();

  //callback->SetCallback(My::Hello);
  //callback->SetClientData(&my);
  //filter->AddObserver(filter->RefreshEvent, callback);
  //filter->Update();


  vtkSmartPointer<vtkCallbackCommand> timerCallback = vtkSmartPointer<vtkCallbackCommand>::New();
  timerCallback->SetCallback(TimerCallbackFunction);
  timerCallback->SetClientData(&my);

  renderWindowInteractor->Initialize();
  renderWindowInteractor->CreateRepeatingTimer(200);
  renderWindowInteractor->AddObserver(vtkCommand::TimerEvent, timerCallback);

  // Render and interact
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}

void TimerCallbackFunction(vtkObject *caller, long unsigned int vtkNotUsed(eventId), void *clientData, void *vtkNotUsed(callData)) {
  static int counter = 0;
  cout << GetCurrentThreadId() << " , timer callback: " << ++counter << endl;

  My *my = (My *)clientData;
  my->Hello();
  //// To avoid globals we can implement this later...
  ////  vtkSmartPointer<vtkProgrammableDataObjectSource> pDOS =
  ////      static_cast<vtkProgrammableDataObjectSource*>(clientData);

  //vtkRenderWindowInteractor *rwi =
  //	static_cast<vtkRenderWindowInteractor*>(caller);

  //NextPoint();

  //pDOS->Modified();
  //rwi->Render();
  //renderer->ResetCamera(); // Optional: Reposition Camera, so it displays the whole object

}