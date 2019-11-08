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

static const float sc_inv255 = 1.0f / 255.0f;
static const float sc_inv60 = 1.0f / 60.0f;

// Arbitrary rgb values 0..255
typedef struct rgb {
  unsigned char r, g, b;
  struct rgb() : r(0), g(0), b(0) {};
  struct rgb(unsigned char r1, unsigned char g1, unsigned char b1) : r(r1), g(g1), b(b1) {};
} Rgb;

//! Hue saturation value
typedef struct hsv {
  //! h in the range [0..360], s in the range [0..1], v in the range [0..1]
  float h, s, v;
  struct hsv() : h(0), s(0), v(0) {};
  struct hsv(float h1, float s1, float v1) : h(h1), s(s1), v(v1) {};
} Hsv;

void Convert(const Rgb &rgb, Hsv &hsv) {
  float r = rgb.r * sc_inv255;
  float g = rgb.g * sc_inv255;
  float b = rgb.b * sc_inv255;

  float high = (r > g) ? r : g;
  high = (high > b) ? high : b;
  float low = (r < g) ? r : g;
  low = (low < b) ? low : b;

  float h, s, v;
  v = high;

  float delta = high - low;
  if (delta < 0.00001f) {
    h = 0; // undefined
    s = 0;
  } else {
    s = delta / high;

    if (r == high) {
      h = (g - b) / delta; // yellow and magenta
    } else if (g == high) {
      h = 2.0f + (b - r) / delta; // cyan and yellow
    } else { // b==high
      h = 4.0f + (r - g) / delta; // magenta and cyan
    }
    h *= 60.0f; //degrees

    if (h < 0) {
      h += 360.0f;
    }
  }

  hsv.h = h;
  hsv.s = s;
  hsv.v = v;
}

void Convert(const Hsv &hsv, Rgb &rgb) {
  float h = hsv.h;
  float s = hsv.s;
  float v = hsv.v;

  if (h > 360.0f) h = 0.0f;
  h *= sc_inv60;

  int k = (int)(h);
  float f = h - k; // residual
  float p = v * (1.0f - s);
  float q = v * (1.0f - s * f);
  float t = v * (1.0f - s * (1 - f));

  float r, g, b;
  switch (k) {
    case 0:
      r = v; g = t; b = p; break;
    case 1:
      r = q; g = v; b = p; break;
    case 2:
      r = p; g = v; b = t; break;
    case 3:
      r = p; g = q; b = v; break;
    case 4:
      r = t; g = p; b = v; break;
    case 5:
    default:
      r = v; g = p; b = q; break;
  }

  rgb.r = (unsigned char)(r * 255 + 0.5f);
  rgb.g = (unsigned char)(g * 255 + 0.5f);
  rgb.b = (unsigned char)(b * 255 + 0.5f);
}

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
      if (channel < 3) {
        for (int i = 0; i < colors->GetDataSize() / 3; i++) {
          auto ch = origin_colors->GetComponent(i, channel);
          colors->SetComponent(i, channel, ch * val);
        }
      } else {
        for (int i = 0; i < colors->GetDataSize() / 3; i++) {
          auto r = origin_colors->GetComponent(i, 0);
          auto g = origin_colors->GetComponent(i, 1);
          auto b = origin_colors->GetComponent(i, 2);

          Hsv hsv;
          Rgb rgb(r, g, b);
          Convert(rgb, hsv);
          if (channel == 4) {
            hsv.h += val;
            if (hsv.h > 360) {
              hsv.h -= 360;
            }
          } else {
            hsv.v *= val;
          }
          Convert(hsv, rgb);
          colors->SetComponent(i, 0, rgb.r);
          colors->SetComponent(i, 1, rgb.g);
          colors->SetComponent(i, 2, rgb.b);
        }
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

  for (int i = 0; i < 10; i++) {
    auto Func = [](int x) {
      printf("%s\n", __FUNCTION__);
    };
    Func(i);
  }
  for (int i = 0; i < 10; i++) {
    auto Func = [](int x) {
      printf("%s\n", __FUNCTION__);
    };
    Func(i);
  }
  return 0;
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

  auto sliderRep = vtkSmartPointer<vtkSliderRepresentation2D>::New();
  SetSlider(sliderRep, 0.0, 0.6, 0.2, 40, y, 300, y, "Ambient");

  auto sliderRed = vtkSmartPointer<vtkSliderRepresentation2D>::New();
  SetSlider(sliderRed, 0.0, 2.0, 1, 340, y, 600, y, "Red");

  auto sliderGreen = vtkSmartPointer<vtkSliderRepresentation2D>::New();
  SetSlider(sliderGreen, 0.0, 2.0, 1, 640, y, 900, y, "Green");

  auto sliderBlue = vtkSmartPointer<vtkSliderRepresentation2D>::New();
  SetSlider(sliderBlue, 0.0, 2.0, 1, 940, y, 1200, y, "Blue");

  auto sliderHue = vtkSmartPointer<vtkSliderRepresentation2D>::New();
  SetSlider(sliderHue, 0.0, 360, 0, 1240, y, 1500, y, "Hue");

  auto sliderValue = vtkSmartPointer<vtkSliderRepresentation2D>::New();
  SetSlider(sliderValue, 0.0, 1.5, 1.0, 1540, y, 1800, y, "Value");

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

  auto sliderWidgetRed = NewSliderWidget(renderWindowInteractor, sliderRed);
  auto sliderWidgetGreen = NewSliderWidget(renderWindowInteractor, sliderGreen);
  auto sliderWidgetBlue = NewSliderWidget(renderWindowInteractor, sliderBlue);
  auto sliderWidgetHue = NewSliderWidget(renderWindowInteractor, sliderHue);
  auto sliderWidgetValue = NewSliderWidget(renderWindowInteractor, sliderValue);

  auto originPolyData = vtkSmartPointer<vtkPolyData>::New();
  originPolyData->DeepCopy(polydata);

  auto callbackRed = vtkSmartPointer<vtkSliderRGBCallback>::New();
  callbackRed->channel = 0;
  callbackRed->polydata = polydata;
  callbackRed->originPolyData = originPolyData;
  sliderWidgetRed->AddObserver(vtkCommand::InteractionEvent, callbackRed);

  auto callbackGreen = vtkSmartPointer<vtkSliderRGBCallback>::New();
  callbackGreen->channel = 1;
  callbackGreen->polydata = polydata;
  callbackGreen->originPolyData = originPolyData;
  sliderWidgetGreen->AddObserver(vtkCommand::InteractionEvent, callbackGreen);

  auto callbackBlue = vtkSmartPointer<vtkSliderRGBCallback>::New();
  callbackBlue->channel = 2;
  callbackBlue->polydata = polydata;
  callbackBlue->originPolyData = originPolyData;
  sliderWidgetBlue->AddObserver(vtkCommand::InteractionEvent, callbackBlue);

  auto callBackHue = vtkSmartPointer<vtkSliderRGBCallback>::New();
  callBackHue->channel = 4;
  callBackHue->polydata = polydata;
  callBackHue->originPolyData = originPolyData;
  sliderWidgetHue->AddObserver(vtkCommand::InteractionEvent, callBackHue);

  auto callBackValue = vtkSmartPointer<vtkSliderRGBCallback>::New();
  callBackValue->channel = 5;
  callBackValue->polydata = polydata;
  callBackValue->originPolyData = originPolyData;
  sliderWidgetValue->AddObserver(vtkCommand::InteractionEvent, callBackValue);


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