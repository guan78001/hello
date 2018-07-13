#include <vtkOBJReader.h>
#include <vtkOBJExporter.h>

#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <string>
#include <sstream>
#include <vtkPLYReader.h>
#include <vtkPLYWriter.h>
#include <vtkPNGReader.h>
#include <vtkTransform.h>
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
  //{
  //  auto writer = vtkSmartPointer<vtkPLYWriter>::New();
  //  writer->SetInputConnection(reader->GetOutputPort());
  //  std::string plyFileName = filename.substr(0, npos) + "-export.ply";
  //  writer->SetFileName(plyFileName.c_str());
  //  writer->Write();
  //}
  std::string imagename = filename.substr(0, npos) + ".png";
  vtkSmartPointer<vtkPNGReader> imageReader =  vtkSmartPointer<vtkPNGReader>::New();
  imageReader->SetFileName(imagename.c_str());
  imageReader->Update();

  vtkSmartPointer<vtkTexture> colorTexture =
    vtkSmartPointer<vtkTexture>::New();
  colorTexture->SetInputConnection(imageReader->GetOutputPort());
  colorTexture->InterpolateOn();

  std::vector<vtkSmartPointer<vtkTexture>> all_textures;
  // Visualize
  vtkSmartPointer<vtkPolyDataMapper> mapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(reader->GetOutputPort());

  vtkSmartPointer<vtkActor> actor =
    vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  actor->SetTexture(colorTexture);

  //vtkSmartPointer<vtkActor> actor2 =
  //  vtkSmartPointer<vtkActor>::New();
  //actor2->SetMapper(mapper);
  //actor2->SetTexture(colorTexture);

  //vtkSmartPointer<vtkTransform> transform =
  //  vtkSmartPointer<vtkTransform>::New();
  //transform->PostMultiply(); //this is the key line
  //transform->Translate(3.0, 0, 0);
  //actor2->SetUserTransform(transform);

  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  renderer->AddActor(actor);

  for (int i = 1; i < 33; i++) {
    vtkSmartPointer<vtkActor> actor2 =
      vtkSmartPointer<vtkActor>::New();
    actor2->SetMapper(mapper);

    std::string imagename = filename.substr(0, npos);
    size_t nPos = imagename.find_last_of("\\/");
    std::stringstream ss;
    ss << imagename.substr(0, nPos + 1) << i << ".png";
    imagename = ss.str();

    vtkSmartPointer<vtkPNGReader> imageReader = vtkSmartPointer<vtkPNGReader>::New();
    imageReader->SetFileName(imagename.c_str());
    imageReader->Update();

    vtkSmartPointer<vtkTexture> colorTexture =
      vtkSmartPointer<vtkTexture>::New();
    colorTexture->SetInputConnection(imageReader->GetOutputPort());
    colorTexture->InterpolateOn();
    actor2->SetTexture(colorTexture);

    vtkSmartPointer<vtkTransform> transform =
      vtkSmartPointer<vtkTransform>::New();
    transform->PostMultiply(); //this is the key line
    transform->Translate(3 * i, 0, 0);
    actor2->SetUserTransform(transform);

    renderer->AddActor(actor2);
  }
  //renderer->AddActor(actor2);
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