#include <vtkSmartPointer.h>
 
#include <vtkImageCanvasSource2D.h>
#include <vtkPlaneSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkTexture.h>
#include <vtkJPEGReader.h> 
#include <vtkImageImport.h>
#include <vtkImageData.h>
#include <memory>

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
void FillImageData(vtkImageData* image,unsigned char *pData, int width, int height, int channel = 3)
{
	image->SetDimensions(width, height, 1);
	image->AllocateScalars(VTK_UNSIGNED_CHAR, 3);
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			unsigned char* dst = static_cast<unsigned char*>(image->GetScalarPointer(x, y, 0));
			unsigned char *src = pData + (y*height + x)*channel;
			memcpy(dst, src, sizeof(unsigned char)*channel);
		}
	}
	image->Modified();
}

int main(int, char *argv[])
{
#if 0
  std::string inputFilename = "taiji.jpg";

  // Read the image which will be the texture
  vtkSmartPointer<vtkJPEGReader> jPEGReader =
	  vtkSmartPointer<vtkJPEGReader>::New();
  jPEGReader->SetFileName(inputFilename.c_str());
#else
  vtkSmartPointer<vtkImageData> imageData =
	  vtkSmartPointer<vtkImageData>::New();

  int width = 128;
  int height = 128;
  int channel = 3;
  unsigned char *pData = GenColorImageData(width, height, channel);
  FillImageData(imageData, pData,width, height, channel);
  delete[] pData;
#endif
   // Create a plane
  vtkSmartPointer<vtkPlaneSource> plane = 
    vtkSmartPointer<vtkPlaneSource>::New();
  plane->SetCenter(0.0, 0.0, 0.0);
  plane->SetNormal(0.0, 0.0, 1.0);
 
  // Apply the texture
  vtkSmartPointer<vtkTexture> texture = 
    vtkSmartPointer<vtkTexture>::New();
  texture->SetInputData(imageData);

  vtkSmartPointer<vtkPolyDataMapper> planeMapper = 
    vtkSmartPointer<vtkPolyDataMapper>::New();
  planeMapper->SetInputConnection(plane->GetOutputPort());
 
  vtkSmartPointer<vtkActor> texturedPlane = 
    vtkSmartPointer<vtkActor>::New();
  texturedPlane->SetMapper(planeMapper);
  texturedPlane->SetTexture(texture);
 
  // Visualize the textured plane
  vtkSmartPointer<vtkRenderer> renderer = 
    vtkSmartPointer<vtkRenderer>::New();
  renderer->AddActor(texturedPlane);
  renderer->SetBackground(1,1,1); // Background color white
  renderer->ResetCamera();
 
  vtkSmartPointer<vtkRenderWindow> renderWindow = 
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
 
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = 
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);
 
  renderWindow->Render();
 
  renderWindowInteractor->Start();
 
  return EXIT_SUCCESS;
}