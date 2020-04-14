//Add below line if missing include(${VTK_USE_FILE}) in cmake files.
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL);
VTK_MODULE_INIT(vtkInteractionStyle);
#include <vtkSmartPointer.h>

#include <vtkSmartPointer.h>
#include <vtkPointData.h>

#include <vtkBYUReader.h>
#include <vtkOBJReader.h>
#include <vtkPLYReader.h>
#include <vtkPolyDataReader.h>
#include <vtkSTLReader.h>
//#include <vtkXMLPolyDataReader.h>
#include <vtkSphereSource.h>

#include <vtkUnsignedCharArray.h>
#include <vtkGlyph3DMapper.h>
#include <vtkPointData.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkCamera.h>

#include <vtkNamedColors.h>
#include <vtkTimerLog.h>
#include <vtksys/SystemTools.hxx>

namespace {
vtkSmartPointer<vtkPolyData> ReadPolyData(const char *fileName);
}

int main (int argc, char *argv[]) {
  if (argc < 2) {
    printf("usage DisplayPoints filename(only contain points).\n");
    return 0;
  }
  vtkSmartPointer<vtkPolyData> polyData = ReadPolyData(argc > 1 ? argv[1] : "");;
  polyData->Print(std::cout);

  // Visualize
  vtkSmartPointer<vtkNamedColors> colors =
    vtkSmartPointer<vtkNamedColors>::New();

  vtkSmartPointer<vtkSphereSource> source =
    vtkSmartPointer<vtkSphereSource>::New();
  source->SetRadius(.0005935);

  vtkSmartPointer<vtkGlyph3DMapper> glyph3Dmapper =
    vtkSmartPointer<vtkGlyph3DMapper>::New();
  glyph3Dmapper->SetSourceConnection(source->GetOutputPort());
  glyph3Dmapper->SetInputData(polyData);
  glyph3Dmapper->SelectColorArray("RGB");
  glyph3Dmapper->Update();

  vtkSmartPointer<vtkActor> actor =
    vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(glyph3Dmapper);
  actor->GetProperty()->SetDiffuseColor(colors->GetColor3d("Crimson").GetData());
  actor->GetProperty()->SetSpecular(.6);
  actor->GetProperty()->SetSpecularPower(30);
  ;

  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  renderer->AddActor(actor);
  renderer->SetBackground(colors->GetColor3d("Silver").GetData());

  renderWindow->Render();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}

namespace {
vtkSmartPointer<vtkPolyData> ReadPolyData(const char *fileName) {
  vtkSmartPointer<vtkPolyData> polyData;
  std::string str(fileName);
  auto pos = str.find_last_of(".");
  std::string extension = str.substr(pos);
  if (extension == ".ply") {
    vtkSmartPointer<vtkPLYReader> reader =
      vtkSmartPointer<vtkPLYReader>::New();
    reader->SetFileName (fileName);
    reader->Update();
    polyData = reader->GetOutput();
  }/* else if (extension == ".vtp") {
    vtkSmartPointer<vtkXMLPolyDataReader> reader =
      vtkSmartPointer<vtkXMLPolyDataReader>::New();
    reader->SetFileName (fileName);
    reader->Update();
    polyData = reader->GetOutput();
  } */else if (extension == ".obj") {
    vtkSmartPointer<vtkOBJReader> reader =
      vtkSmartPointer<vtkOBJReader>::New();
    reader->SetFileName (fileName);
    reader->Update();
    polyData = reader->GetOutput();
  } else if (extension == ".stl") {
    vtkSmartPointer<vtkSTLReader> reader =
      vtkSmartPointer<vtkSTLReader>::New();
    reader->SetFileName (fileName);
    reader->Update();
    polyData = reader->GetOutput();
  } else if (extension == ".vtk") {
    vtkSmartPointer<vtkPolyDataReader> reader =
      vtkSmartPointer<vtkPolyDataReader>::New();
    reader->SetFileName (fileName);
    reader->Update();
    polyData = reader->GetOutput();
  } else if (extension == ".g") {
    vtkSmartPointer<vtkBYUReader> reader =
      vtkSmartPointer<vtkBYUReader>::New();
    reader->SetGeometryFileName (fileName);
    reader->Update();
    polyData = reader->GetOutput();
  } else {
    vtkSmartPointer<vtkSphereSource> source =
      vtkSmartPointer<vtkSphereSource>::New();
    source->Update();
    polyData = source->GetOutput();
  }
  return polyData;
}
}