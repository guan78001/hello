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
#include <vtkLight.h>
#include <vtkProperty.h>

#include <vtkSliderWidget.h>
#include <vtkSliderRepresentation3D.h>
#include <vtkSliderRepresentation2D.h>
#include <vtkProperty2D.h>
#include <vtkWidgetEvent.h>
#include <vtkCallbackCommand.h>
#include <vtkSphereSource.h>
#include <vtkPointData.h>
#include <vtkDataSetAttributes.h>

#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProgrammableFilter.h>
#include <vtkCallbackCommand.h>
#include <vtkInteractorStyleTrackballCamera.h>
class vtkSliderCallback : public vtkCommand {
 public:
  static vtkSliderCallback *New() {
    return new vtkSliderCallback;
  }
  virtual void Execute(vtkObject *caller, unsigned long, void *) {
    vtkSliderWidget *sliderWidget =
      reinterpret_cast<vtkSliderWidget *>(caller);
    double val = static_cast<vtkSliderRepresentation *>(sliderWidget->GetRepresentation())->GetValue();
    if (actor) {
      auto prop = actor->GetProperty();
      prop->SetAmbient(val);
    }

  }
  vtkActor *actor = nullptr;
};

class vtkSliderRGBCallback : public vtkCommand {
 public:
  static vtkSliderRGBCallback *New() {
    return new vtkSliderRGBCallback;
  }
  virtual void Execute(vtkObject *caller, unsigned long, void *) {
    vtkSliderWidget *sliderWidget =
      reinterpret_cast<vtkSliderWidget *>(caller);
    double val = static_cast<vtkSliderRepresentation *>(sliderWidget->GetRepresentation())->GetValue();
    if (polydata) {
      vtkDataArray *colors = polydata->GetPointData()->GetScalars();
      vtkDataArray *origin_colors = originPolyData->GetPointData()->GetScalars();
      for (int i = 0; i < colors->GetDataSize() / 3; i++) {
        auto ch = origin_colors->GetComponent(i, channel);
        colors->SetComponent(i, channel,  ch * val);
      }
      polydata->Modified();
    }
  }
  vtkPolyData *polydata = nullptr;
  vtkPolyData *originPolyData = nullptr;
  int channel = 0;
};

void SetSlider(vtkSmartPointer<vtkSliderRepresentation2D> &sliderRep, double min, double max, double cur, int p1x, int p1y, int p2x, int p2y, const char *str) {
  sliderRep->SetMinimumValue(min);
  sliderRep->SetMaximumValue(max);
  sliderRep->SetValue(cur);
  sliderRep->SetTitleText(str);

  sliderRep->GetPoint1Coordinate()->SetCoordinateSystemToDisplay();
  sliderRep->GetPoint1Coordinate()->SetValue(p1x, p1y);
  sliderRep->GetPoint2Coordinate()->SetCoordinateSystemToDisplay();
  sliderRep->GetPoint2Coordinate()->SetValue(p2x, p2y);
}

vtkSmartPointer<vtkSliderWidget> NewSliderWidget(vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor, vtkSmartPointer<vtkSliderRepresentation2D> sliderGreen) {
  vtkSmartPointer<vtkSliderWidget> sliderWidgetGreen =
    vtkSmartPointer<vtkSliderWidget>::New();
  sliderWidgetGreen->SetInteractor(renderWindowInteractor);
  sliderWidgetGreen->SetRepresentation(sliderGreen);
  sliderWidgetGreen->SetAnimationModeToAnimate();
  sliderWidgetGreen->EnabledOn();
  return sliderWidgetGreen;
}
int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << "  Filename(.ply)" << std::endl;
    return EXIT_FAILURE;
  }

  std::string inputFilename = argv[1];

  vtkSmartPointer<vtkPLYReader> reader =
    vtkSmartPointer<vtkPLYReader>::New();

  reader->SetFileName(inputFilename.c_str());
  reader->Update();
  auto polydata = vtkSmartPointer<vtkPolyData>(reader->GetOutput());

  // Visualize
  vtkSmartPointer<vtkPolyDataMapper> mapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  //mapper->SetInputConnection(reader->GetOutputPort());
  mapper->SetInputData(polydata);
  mapper->Update();

  vtkSmartPointer<vtkActor> actor =
    vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  //actor->GetProperty()->LightingOff();//light off.
  //actor->GetProperty()->ShadingOff();

  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  renderer->AddActor(actor);

  auto prop = actor->GetProperty();
  prop->SetAmbient(0.2);
  //prop->SetDiffuse(0.2);
  //prop->SetSpecular(0.01);
  int x = 500;
  int y = 60;

  vtkSmartPointer<vtkSliderRepresentation2D> sliderRep =
    vtkSmartPointer<vtkSliderRepresentation2D>::New();
  SetSlider(sliderRep, 0.0, 0.6, 0.2, 40, y, 300, y, "Ambient");

  vtkSmartPointer<vtkSliderRepresentation2D> sliderRed =
    vtkSmartPointer<vtkSliderRepresentation2D>::New();
  SetSlider(sliderRed, 0.0, 2.0, 1, 340, y, 600, y, "Red");

  vtkSmartPointer<vtkSliderRepresentation2D> sliderGreen =
    vtkSmartPointer<vtkSliderRepresentation2D>::New();
  SetSlider(sliderGreen, 0.0, 2.0, 1, 640, y, 900, y, "Green");

  vtkSmartPointer<vtkSliderRepresentation2D> sliderBlue =
    vtkSmartPointer<vtkSliderRepresentation2D>::New();
  SetSlider(sliderBlue, 0.0, 2.0, 1, 940, y, 1200, y, "Blue");


  vtkSmartPointer<vtkSliderWidget> sliderWidget =
    vtkSmartPointer<vtkSliderWidget>::New();
  sliderWidget->SetInteractor(renderWindowInteractor);
  sliderWidget->SetRepresentation(sliderRep);
  sliderWidget->SetAnimationModeToAnimate();
  sliderWidget->EnabledOn();

  vtkSmartPointer<vtkSliderCallback> callback =
    vtkSmartPointer<vtkSliderCallback>::New();
  callback->actor = actor;
  sliderWidget->AddObserver(vtkCommand::InteractionEvent, callback);

  vtkSmartPointer<vtkSliderWidget> sliderWidgetRed = NewSliderWidget(renderWindowInteractor, sliderRed);
  vtkSmartPointer<vtkSliderWidget> sliderWidgetGreen = NewSliderWidget(renderWindowInteractor, sliderGreen);
  vtkSmartPointer<vtkSliderWidget> sliderWidgetBlue = NewSliderWidget(renderWindowInteractor, sliderBlue);

  auto originPolyData = vtkSmartPointer<vtkPolyData>::New();
  originPolyData->DeepCopy(polydata);

  auto callbackRed =
    vtkSmartPointer<vtkSliderRGBCallback>::New();
  callbackRed->channel = 0;
  callbackRed->polydata = polydata;
  callbackRed->originPolyData = originPolyData;
  sliderWidgetRed->AddObserver(vtkCommand::InteractionEvent, callbackRed);

  auto callbackGreen =
    vtkSmartPointer<vtkSliderRGBCallback>::New();
  callbackGreen->channel = 1;
  callbackGreen->polydata = polydata;
  callbackGreen->originPolyData = originPolyData;
  sliderWidgetGreen->AddObserver(vtkCommand::InteractionEvent, callbackGreen);

  auto callbackBlue =
    vtkSmartPointer<vtkSliderRGBCallback>::New();
  callbackBlue->channel = 2;
  callbackBlue->polydata = polydata;
  callbackBlue->originPolyData = originPolyData;
  sliderWidgetBlue->AddObserver(vtkCommand::InteractionEvent, callbackBlue);

  renderer->SetBackground(0.1804, 0.5451, 0.3412); // Sea green
  //renderWindow->SetSize(1920, 1080);
  //renderWindow->SetFullScreen(true);
  renderWindow->SetSize(renderWindow->GetScreenSize());
  renderWindow->Render();

  vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
    vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();

  renderWindowInteractor->SetInteractorStyle(style);
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}