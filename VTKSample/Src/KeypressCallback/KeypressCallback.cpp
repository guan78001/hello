#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL);
VTK_MODULE_INIT(vtkInteractionStyle);

#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyData.h>
#include <vtkSphereSource.h>
#include <vtkCallbackCommand.h>
#include <vtkCommand.h>
#include <omp.h>
static void KeypressCallbackFunction ( vtkObject *caller, long unsigned int eventId,
                                       void *clientData, void *callData );
vtkSmartPointer<vtkRenderWindow> g_render;
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
  g_render = renderWindow;

  renderWindow->AddRenderer(renderer);

  // An interactor
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  vtkSmartPointer<vtkCallbackCommand> keypressCallback =
    vtkSmartPointer<vtkCallbackCommand>::New();

  // Allow the observer to access the sphereSource
  keypressCallback->SetClientData(sphereSource);
  keypressCallback->SetCallback(KeypressCallbackFunction );
  renderWindowInteractor->AddObserver(vtkCommand::KeyPressEvent, keypressCallback);

  renderer->AddActor(actor);
  renderer->SetBackground(1, 1, 1); // Background color white
  renderWindow->Render();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}

void KeypressCallbackFunction(vtkObject *, long unsigned int (eventId), void *clientData, void *vtkNotUsed(callData) ) {
  // Prove that we can access the sphere source
  vtkSphereSource *sphereSource =
    static_cast<vtkSphereSource *>(clientData);
  #pragma omp critical
  {
    std::cout << eventId << endl;
    std::cout << "Radius is " << sphereSource->GetRadius() << std::endl;
  }

  sphereSource->SetRadius(sphereSource->GetRadius() + 1);
  g_render->Render();
}