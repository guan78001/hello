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
#include <vtkImageData.h>

unsigned char* GenColorImageData(int width, int height, int channel = 3)
{
	unsigned char *pData = new unsigned char[width*height*channel];
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			unsigned char *pixel = pData + (y*height + x) * channel;
			if (x < width / 2)
			{
				pixel[0] = 255;
				pixel[1] = 0;
			}
			else
			{
				pixel[0] = 0;
				pixel[1] = 255;
			}

			pixel[2] = 0;
		}
	}
	return pData;
}

vtkSmartPointer<vtkTexture> GenTexture()
{
	int width = 128;
	int height = 128;
	int channel = 3;
	unsigned char *pData = GenColorImageData(width, height, channel);

	vtkSmartPointer<vtkImageData> imageData =vtkSmartPointer<vtkImageData>::New();
	imageData->SetDimensions(width, height, 1);
	imageData->AllocateScalars(VTK_UNSIGNED_CHAR, 3);
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			unsigned char* dst = static_cast<unsigned char*>(imageData->GetScalarPointer(x, y, 0));
			unsigned char *src = pData + (y*height + x)*channel;
			memcpy(dst, src, sizeof(unsigned char)*channel);
		}
	}
	imageData->Modified();

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

  std::string inputFilename = "D:\\github\\hello\\VTKSample\\texture\\TextureMapImageData\\Franklin_Benjamin[2018-05-28_17-26-30]_ACQ_mesh_upper_jaw.ply";
  std::string textureFilename = "D:\\github\\hello\\VTKSample\\texture\\TextureMapImageData\\Franklin_Benjamin[2018-05-28_17-26-30]_upper_texture.jpg";


  vtkSmartPointer<vtkPLYReader> reader =
    vtkSmartPointer<vtkPLYReader>::New();
  reader->SetFileName(inputFilename.c_str());

  // Apply the texture
  vtkSmartPointer<vtkTexture> texture =
	  vtkSmartPointer<vtkTexture>::New();
#if 1
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