#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL);
VTK_MODULE_INIT(vtkInteractionStyle);

#include <vtkSphereSource.h>
#include <vtkCallbackCommand.h>
#include <vtkCommand.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

#include "vtkTestFilter.h"
#include <thread>
static void CallbackFunction(vtkObject *caller,
                             long unsigned int eventId,
                             void *clientData,
                             void *callData );

static void Hello(vtkObject *caller,
                  long unsigned int eventId,
                  void *clientData,
                  void *callData) {
  cout << "Hello" << endl;
}
int main(int, char *[]) {
  // Create a renderer, render window, and interactor
  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  vtkSmartPointer<vtkTestFilter> filter =
    vtkSmartPointer<vtkTestFilter>::New();

  vtkSmartPointer<vtkCallbackCommand> callback =
    vtkSmartPointer<vtkCallbackCommand>::New();
  callback->SetCallback(CallbackFunction );
  filter->AddObserver(filter->RefreshEvent, callback);

  filter->Update();

  std::thread th(
  [&]() {
    //
    auto cmd = vtkSmartPointer<vtkCallbackCommand>::New();
    cmd->SetCallback(Hello);
    renderWindowInteractor->AddObserver("hello", cmd);
    for (int i = 0; i < 10; i++) {
      renderWindowInteractor->InvokeEvent("hello");
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
  }
  );

  //trigger user event.
  filter->InvokeEvent(filter->RefreshEvent, NULL);
  renderWindow->Render();
  renderWindowInteractor->Start();
  th.join();
  return EXIT_SUCCESS;
}

void CallbackFunction(vtkObject *caller,
                      long unsigned int eventId,
                      void *clientData, void *callData ) {
  std::cout << "CallbackFunction called." << std::endl;
}