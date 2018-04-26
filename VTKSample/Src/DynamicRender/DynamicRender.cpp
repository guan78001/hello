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

  auto renderStyle =
    vtkSmartPointer<vtkInteractorStyle>::New();

  //renderWindowInteractor->SetInteractorStyle(renderStyle);

  //vtkSmartPointer<vtkCallbackCommand> pickerCommand = vtkSmartPointer<vtkCallbackCommand>::New();
  //p
  //pickerCommand->SetClientData(renderStyle);
  //pickerCommand->SetCallback(PickerInteractionCallback);

  //renderStyle->AddObserver(vtkCommand::KeyPressEvent, pickerCommand);

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

  //actorCollection->SetUserTransform(transform);


  /*while (true)*/ {
    actorCollection->InitTraversal();

    for (vtkIdType i = 0; i < actorCollection->GetNumberOfItems(); i++) {
      vtkActor *actor = actorCollection->GetNextActor();
      actor->SetUserTransform(transform);
      renderer->AddActor(actor);
    }
    // Render and interact
    renderWindow->Render();
    //renderWindowInteractor->Start();

    int n;
    cin >> n;

    actorCollection->InitTraversal();
    for (vtkIdType i = 0; i < actorCollection->GetNumberOfItems(); i++) {
      vtkActor *actor = actorCollection->GetNextActor();
      actor->SetUserTransform(transform);
      renderer->RemoveActor(actor);
    }

    // Render and interact
    renderWindow->Render();
  }

  // Render and interact
  renderWindowInteractor->Start();




  return EXIT_SUCCESS;
}