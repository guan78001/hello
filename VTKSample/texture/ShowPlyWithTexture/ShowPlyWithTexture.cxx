#include <vtkPolyData.h>
#include <vtkPLYReader.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTerrain.h>

#include <vtkTexture.h>
#include <vtkJPEGReader.h>
#include <vtkJPEGWriter.h>
#include <vtkImageData.h>
#include <vtkImageImport.h>
std::string folder = "D:\\github\\hello\\VTKSample\\texture\\ShowPlyWithTexture\\";

unsigned char *GenColorImageData(int width, int height, int channel = 3) {
  unsigned char *pData = new unsigned char[width * height * channel];
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      unsigned char *pixel = pData + (y * width + x) * channel;
      if (x < width / 2) {
        if (y < height / 2) {
          pixel[0] = 255;
          pixel[1] = 0;
        } else {
          pixel[0] = 128;
          pixel[1] = 128;
        }
      } else {
        pixel[0] = 0;
        pixel[1] = 255;
      }

      pixel[2] = 0;
    }
  }
  return pData;
}
//fast
vtkSmartPointer<vtkImageData> GetVTKImageData(int width, int height, int channel, unsigned char *pData) {
  vtkSmartPointer<vtkImageImport> importer = vtkSmartPointer<vtkImageImport>::New();
  importer->SetDataScalarTypeToUnsignedChar();
  importer->SetImportVoidPointer(pData);
  importer->SetWholeExtent(0, width - 1, 0, height - 1, 0, 0);
  importer->SetDataExtentToWholeExtent();
  importer->SetNumberOfScalarComponents(3);
  importer->Update();

  vtkSmartPointer<vtkImageData> outp = importer->GetOutput();
  outp->Modified();
  return outp;
}
//slow
vtkSmartPointer<vtkImageData> GetVTKImageData2(int width, int height, int channel, unsigned char *pData) {
  vtkSmartPointer<vtkImageData> imageData = vtkSmartPointer<vtkImageData>::New();
  imageData->SetDimensions(width, height, 1);
  imageData->AllocateScalars(VTK_UNSIGNED_CHAR, 3);
  #pragma omp parallel for
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      unsigned char *dst = static_cast<unsigned char *>(imageData->GetScalarPointer(x, y, 0));
      unsigned char *src = pData + ((height - 1 - y) * width + x) * channel;
      dst[0] = src[2];
      dst[1] = src[1];
      dst[2] = src[0];
    }
  }
  imageData->Modified();
  return imageData;
}
vtkSmartPointer<vtkTexture> GenTexture() {
  int width = 4096;
  int height = 8192;
  int channel = 3;
  unsigned char *pData = GenColorImageData(width, height, channel);
  vtkSmartPointer<vtkImageData> imageData = vtkSmartPointer<vtkImageData>::New();
  imageData = GetVTKImageData2(width, height, channel, pData);

  vtkSmartPointer<vtkJPEGWriter> wirter = vtkSmartPointer<vtkJPEGWriter>::New();
  wirter->SetInputData(imageData);
  std::string filename = folder + "fake.jpg";
  wirter->SetFileName(filename.c_str());
  wirter->Write();

  delete[] pData;
  vtkSmartPointer<vtkTexture> texture = vtkSmartPointer<vtkTexture>::New();
  texture->SetInputData(imageData);
  return texture;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << "  Filename(.ply)" << std::endl;
    return EXIT_FAILURE;
  }

  //std::string inputFilename = "D:\\github\\hello\\VTKSample\\texture\\TextureMapImageData\\texturedknot2.ply";
  //std::string textureFilename = "D:\\github\\hello\\VTKSample\\texture\\TextureMapImageData\\texturedknot2.jpg";
  std::string inputFilename = folder + "mesh.ply";
  std::string textureFilename = folder + "texture.jpg";
  //std::string inputFilename = folder + "laurana500-jpg.ply";
  //std::string textureFilename = folder + "laurana500.jpg";

  vtkSmartPointer<vtkPLYReader> reader =
    vtkSmartPointer<vtkPLYReader>::New();
  reader->SetFileName(inputFilename.c_str());

  // Apply the texture
  vtkSmartPointer<vtkTexture> texture =
    vtkSmartPointer<vtkTexture>::New();
#if 0
  // Read the image which will be the texture
  vtkSmartPointer<vtkJPEGReader> jPEGReader = vtkSmartPointer<vtkJPEGReader>::New();
  jPEGReader->SetFileName(textureFilename.c_str());
  texture->SetInputConnection(jPEGReader->GetOutputPort());
#else
  texture = GenTexture();
#endif
  // Visualize
  vtkSmartPointer<vtkPolyDataMapper> mapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(reader->GetOutputPort());

  vtkSmartPointer<vtkActor> actor =
    vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);

  // Apply the texture
  actor->SetTexture(texture);

  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  renderer->AddActor(actor);
  renderer->SetBackground(0.1804, 0.5451, 0.3412); // Sea green
  renderWindow->SetSize(600, 600);
  renderWindow->Render();

  vtkSmartPointer<vtkInteractorStyleTerrain> style =
    vtkSmartPointer<vtkInteractorStyleTerrain>::New();

  renderWindowInteractor->SetInteractorStyle(style);

  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}