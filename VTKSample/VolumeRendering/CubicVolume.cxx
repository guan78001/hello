//https://stackoverflow.com/questions/39976669/vtk-how-to-import-3d-image-data-via-imageimport-on-c-for-volume-renderings

#include <iostream>
#include <vtkStructuredPointsReader.h>
#include <vtkVolumeRayCastMIPFunction.h>
#include <vtkVolumeRayCastMapper.h>
#include <vtkVolumeRayCastMapper.h>
#include <vtkRendererCollection.h>
#include <vtkSmartPointer.h>
#include <vtkImageImport.h>
#include <stdlib.h>
// VTK includes
#include "vtkBoxWidget.h"
#include "vtkCamera.h"
#include "vtkCommand.h"
#include "vtkColorTransferFunction.h"
#include "vtkDICOMImageReader.h"
#include "vtkImageData.h"
#include "vtkImageResample.h"
#include "vtkMetaImageReader.h"
#include "vtkPiecewiseFunction.h"
#include "vtkPlanes.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkVolume.h"
#include "vtkVolumeProperty.h"
#include "vtkXMLImageDataReader.h"
#include "vtkFixedPointVolumeRayCastMapper.h"

int main() {
  std::cout << "Hello, World!" << std::endl;
  // create data
  // Create a c-style image
  const int width = 4;
  const int height = 4;
  const int depth = 4;
  const int channel = 1;//commonly channel is 1.
  unsigned char cImage[width * height * depth * channel];
  unsigned char value = 0;
  for (unsigned int row = 0; row < height; ++row) {
    for (unsigned int col = 0; col < width; ++col) {
      for (unsigned int z = 0; z < depth; ++z) {
        cImage[row * width * depth + col * depth + z] = 50 + (row * 2 >= width) * 50;
        //cImage[3 * (row * width * depth + col * depth + z) + 0] = 250;
        //cImage[3 * (row * width * depth + col * depth + z) + 1] = 250;
        //cImage[3 * (row * width * depth + col * depth + z) + 2] = 250;
      }
    }
  }
  // Convert the c-style image to a vtkImageData
  vtkSmartPointer<vtkImageImport> imageImport =
    vtkSmartPointer<vtkImageImport>::New();

  imageImport->SetImportVoidPointer(cImage);
  imageImport->SetDataScalarTypeToUnsignedChar();
  //imageImport->SetNumberOfScalarComponents(1);
  imageImport->SetNumberOfScalarComponents(channel);
  imageImport->SetDataSpacing(1, 1, 1);
  imageImport->SetDataOrigin(0, 0, 0);
  imageImport->SetDataExtent(0, width - 1, 0, height - 1, 0, depth - 1);
  imageImport->SetWholeExtent(0, width - 1, 0, height - 1, 0, depth - 1);
  //    imageImport->SetDataExtentToWholeExtent();
  //    imageImport->SetDataScalarTypeToUnsignedChar();
  //    imageImport->SetNumberOfScalarComponents(1);
  imageImport->Update();

  // Create the standard ren, render window and interactor
  vtkRenderer *ren = vtkRenderer::New();
  vtkRenderWindow *renWin = vtkRenderWindow::New();
  renWin->AddRenderer(ren);
  vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();

  iren->SetRenderWindow(renWin);

  // Create the reader for the data
  //    vtkStructuredPointsReader *reader = vtkStructuredPointsReader::New();
  //    reader->SetFileName("/home/honhe/ClionProjects/Demo_vtk_SimpleRayCast/ironProt.vtk");

  // Create transfer mapping scalar value to opacity
  vtkPiecewiseFunction *opacityTransferFunction = vtkPiecewiseFunction::New();
  opacityTransferFunction->AddPoint(0, 0.0);
  opacityTransferFunction->AddPoint(100, 0.5);

  // Create transfer mapping scalar value to color
  vtkColorTransferFunction *colorTransferFunction = vtkColorTransferFunction::New();
  colorTransferFunction->AddRGBPoint(0.0, 0.0, 0.0, 0.0);
  colorTransferFunction->AddRGBPoint(50.0, 0.0, 0.0, 1.0);
  colorTransferFunction->AddRGBPoint(100.0, 0.0, 1.0, 0.0);
  //colorTransferFunction->AddRGBPoint(100.0, 1.0, 1.0, 1.0);

  // The property describes how the data will look
  vtkVolumeProperty *volumeProperty = vtkVolumeProperty::New();
  volumeProperty->SetColor(colorTransferFunction);
  volumeProperty->SetScalarOpacity(opacityTransferFunction);
  volumeProperty->ShadeOn();
  volumeProperty->SetInterpolationTypeToLinear();

  // The mapper / ray cast function know how to render the data
  //vtkVolumeRayCastCompositeFunction *compositeFunction = vtkVolumeRayCastCompositeFunction::New();
  //vtkVolumeRayCastMapper *volumeMapper = vtkVolumeRayCastMapper::New();

  vtkSmartPointer<vtkVolumeRayCastMIPFunction> compositeFunction = vtkSmartPointer<vtkVolumeRayCastMIPFunction>::New();
  vtkSmartPointer<vtkVolumeRayCastMapper> volumeMapper = vtkSmartPointer<vtkVolumeRayCastMapper>::New();

  volumeMapper->SetVolumeRayCastFunction(compositeFunction);
  volumeMapper->SetInputConnection(imageImport->GetOutputPort());

  // The volume holds the mapper and the property and
  // can be used to position/orient the volume
  vtkVolume *volume = vtkVolume::New();
  volume->SetMapper(volumeMapper);
  volume->SetProperty(volumeProperty);

  ren->AddVolume(volume);
  ren->SetBackground(1, 1, 1);
  renWin->SetSize(400, 400);

  iren->Initialize();
  renWin->Render();
  iren->Start();
  return 0;
}