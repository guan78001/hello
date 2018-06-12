#include <vtkOBJReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <string>
#include <vtkPLYReader.h>
#include <vtkPLYWriter.h>
#include <vtkPNGReader.h>
int main(int argc, char *argv[]) {
  // Parse command line arguments
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " Filename(.obj)" << std::endl;
    return EXIT_FAILURE;
  }

  std::string filename = argv[1];
  size_t npos = filename.find_last_of(".");
  vtkSmartPointer<vtkAbstractPolyDataReader> reader;
  std::string extType = filename.substr(npos);
  if (extType == ".obj") {
    reader = vtkSmartPointer<vtkOBJReader>::New();
  } else if (extType == ".ply") {
    reader = vtkSmartPointer<vtkPLYReader>::New();
  }

  reader->SetFileName(filename.c_str());
  reader->Update();
  {
    auto writer = vtkSmartPointer<vtkPLYWriter>::New();
    writer->SetInputConnection(reader->GetOutputPort());
    std::string plyFileName = filename.substr(0, npos) + "-export.ply";
    writer->SetFileName(plyFileName.c_str());
    writer->Write();
  }

  std::string imagename = filename.substr(0, npos) + ".png";
  vtkSmartPointer<vtkPNGReader> imageReader =  vtkSmartPointer<vtkPNGReader>::New();
  imageReader->SetFileName(imagename.c_str());
  imageReader->Update();

  vtkSmartPointer<vtkTexture> colorTexture =
    vtkSmartPointer<vtkTexture>::New();
  colorTexture->SetInputConnection(imageReader->GetOutputPort());
  colorTexture->InterpolateOn();

  // Visualize
  vtkSmartPointer<vtkPolyDataMapper> mapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(reader->GetOutputPort());

  vtkSmartPointer<vtkActor> actor =
    vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);

  actor->SetTexture(colorTexture);

  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  renderer->AddActor(actor);
  renderer->SetBackground(.3, .6, .3); // Background color green

  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);

  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}