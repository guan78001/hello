//Add below line if missing include(${VTK_USE_FILE}) in cmake files.
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL);
VTK_MODULE_INIT(vtkInteractionStyle);

//or
//#define vtkRenderingCore_AUTOINIT 4(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingFreeTypeOpenGL,vtkRenderingOpenGL)
//#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL)


#include <vtkVersion.h>
#include <vtkBooleanOperationPolyDataFilter.h>

#include <vtkActor.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkCylinderSource.h>
#include <vtkPLYWriter.h>
#include <vtkPLYReader.h>

int main(int argc, char *argv[]) {
  vtkSmartPointer<vtkPolyData> input1;
  vtkSmartPointer<vtkPolyData> input2;

  //std::string operation("intersection");
  std::string operation("union");
  //std::string operation("difference");
  //argc = -1;
  if (argc == 4) {
    //vtkSmartPointer<vtkPolyDataReader> reader1 =
    //  vtkSmartPointer<vtkPolyDataReader>::New();

    vtkSmartPointer<vtkPLYReader> reader1 =
      vtkSmartPointer<vtkPLYReader>::New();
    reader1->SetFileName(argv[1]);
    reader1->Update();
    input1 = reader1->GetOutput();

    //vtkSmartPointer<vtkPolyDataReader> reader2 =
    //  vtkSmartPointer<vtkPolyDataReader>::New();
    vtkSmartPointer<vtkPLYReader> reader2 =
      vtkSmartPointer<vtkPLYReader>::New();
    reader2->SetFileName(argv[3]);
    reader2->Update();
    input2 = reader2->GetOutput();

    operation = argv[2];
  } else {
    std::string inputFilename1("D:\\mesh\\sphere1.ply");
    std::string inputFilename2("D:\\mesh\\label.ply");

    inputFilename1 = argv[1];
    inputFilename2 = argv[3];

#if 0
    vtkSmartPointer<vtkSphereSource> sphereSource1 =
      vtkSmartPointer<vtkSphereSource>::New();
    sphereSource1->SetCenter(.25, 0, 0);
    sphereSource1->SetRadius(0.25);
    sphereSource1->Update();
    input1 = sphereSource1->GetOutput();

    vtkSmartPointer<vtkSphereSource> sphereSource2 =
      vtkSmartPointer<vtkSphereSource>::New();
    sphereSource2->SetCenter(.5, 0.0, 0);
    sphereSource2->SetRadius(0.25);
    sphereSource2->Update();
    input2 = sphereSource2->GetOutput();

    {
      vtkSmartPointer<vtkPLYWriter> plyWriter =
        vtkSmartPointer<vtkPLYWriter>::New();
      plyWriter->SetFileName(inputFilename1.c_str());
      plyWriter->SetInputConnection(sphereSource1->GetOutputPort());
      plyWriter->Write();
    }

    {
      vtkSmartPointer<vtkPLYWriter> plyWriter =
        vtkSmartPointer<vtkPLYWriter>::New();
      plyWriter->SetFileName(inputFilename2.c_str());
      plyWriter->SetInputConnection(sphereSource2->GetOutputPort());
      //plyWriter->SetInputConnection(sphereSource->GetOutputPort());
      plyWriter->Write();
    }
#else
    vtkSmartPointer<vtkPLYReader> reader1 =
      vtkSmartPointer<vtkPLYReader>::New();
    reader1->SetFileName(inputFilename1.c_str());
    reader1->Update();

    vtkSmartPointer<vtkPLYReader> reader2 =
      vtkSmartPointer<vtkPLYReader>::New();
    reader2->SetFileName(inputFilename2.c_str());
    reader2->Update();

    input1 = reader1->GetOutput();
    input2 = reader2->GetOutput();

    //vtkSmartPointer<vtkCylinderSource> cylinder =
    //  vtkSmartPointer<vtkCylinderSource>::New();
    //cylinder->SetResolution(16);
    //cylinder->Update();
    //input2 = cylinder->GetOutput();
#endif
    if (argc == 2) {
      operation = argv[1];
    }
  }

  vtkSmartPointer<vtkPolyDataMapper> input1Mapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
#if VTK_MAJOR_VERSION <= 5
  input1Mapper->SetInputConnection(input1->GetProducerPort());
#else
  input1Mapper->SetInputData(input1);
#endif
  input1Mapper->ScalarVisibilityOff();
  vtkSmartPointer<vtkActor> input1Actor =
    vtkSmartPointer<vtkActor>::New();
  input1Actor->SetMapper(input1Mapper);
  input1Actor->GetProperty()->SetColor(1, 0, 0);
  input1Actor->SetPosition(
    input1->GetBounds()[1] - input1->GetBounds()[0],
    0, 0);
  vtkSmartPointer<vtkPolyDataMapper> input2Mapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
#if VTK_MAJOR_VERSION <= 5
  input2Mapper->SetInputConnection(input2->GetProducerPort());
#else
  input2Mapper->SetInputData(input2);
#endif
  input2Mapper->ScalarVisibilityOff();
  vtkSmartPointer<vtkActor> input2Actor =
    vtkSmartPointer<vtkActor>::New();
  input2Actor->SetMapper(input2Mapper);
  input2Actor->GetProperty()->SetColor(0, 1, 0);
  input2Actor->SetPosition(
    -(input2->GetBounds()[1] - input2->GetBounds()[0]),
    0, 0);
  vtkSmartPointer<vtkBooleanOperationPolyDataFilter> booleanOperation =
    vtkSmartPointer<vtkBooleanOperationPolyDataFilter>::New();
  if (operation == "union") {
    booleanOperation->SetOperationToUnion();
  } else if (operation == "intersection") {
    booleanOperation->SetOperationToIntersection();
  } else if (operation == "difference") {
    booleanOperation->SetOperationToDifference();
  } else {
    std::cout << "Unknown operation: " << operation << std::endl;
    return EXIT_FAILURE;
  }
#if VTK_MAJOR_VERSION <= 5
  booleanOperation->SetInputConnection(0, input1->GetProducerPort());
  booleanOperation->SetInputConnection(1, input2->GetProducerPort());
#else
  booleanOperation->SetInputData(0, input1);
  booleanOperation->SetInputData(1, input2);
#endif

  vtkSmartPointer<vtkSphereSource> sphereSource =
    vtkSmartPointer<vtkSphereSource>::New();
  sphereSource->Update();
  std::string filename = "d:\\mesh\\xxx" + operation + ".ply";
  vtkSmartPointer<vtkPLYWriter> plyWriter =
    vtkSmartPointer<vtkPLYWriter>::New();
  plyWriter->SetFileName(filename.c_str());
  plyWriter->SetInputConnection(booleanOperation->GetOutputPort());
  //plyWriter->SetInputConnection(sphereSource->GetOutputPort());
  plyWriter->Write();

  vtkSmartPointer<vtkPolyDataMapper> booleanOperationMapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  booleanOperationMapper->SetInputConnection(booleanOperation->GetOutputPort());
  booleanOperationMapper->ScalarVisibilityOff();

  vtkSmartPointer<vtkActor> booleanOperationActor =
    vtkSmartPointer<vtkActor>::New();
  booleanOperationActor->SetMapper(booleanOperationMapper);

  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  renderer->AddViewProp(input1Actor);
  renderer->AddViewProp(input2Actor);
  renderer->AddViewProp(booleanOperationActor);
  renderer->SetBackground(.1, .2, .3);
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);

  vtkSmartPointer<vtkRenderWindowInteractor> renWinInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renWinInteractor->SetRenderWindow(renderWindow);

  renderWindow->Render();
  renWinInteractor->Start();

  return EXIT_SUCCESS;
}

//#include <vtkSmartPointer.h>
//#include "vtkLoopBooleanPolyDataFilter.h"
//
//#include <vtkCleanPolyData.h>
//#include <vtkTriangleFilter.h>
//#include <vtkActor.h>
//#include <vtkPolyDataMapper.h>
//#include <vtkProperty.h>
//#include <vtkRenderer.h>
//#include <vtkRenderWindow.h>
//#include <vtkRenderWindowInteractor.h>
//#include <vtkNamedColors.h>
//
//#include <vtkBYUReader.h>
//#include <vtkOBJReader.h>
//#include <vtkPLYReader.h>
//#include <vtkPolyDataReader.h>
//#include <vtkSTLReader.h>
//#include <vtkXMLPolyDataReader.h>
//#include <vtkSphereSource.h>
//#include <vtksys/SystemTools.hxx>
//
//#include <vtkCamera.h>
//namespace {
//vtkSmartPointer<vtkPolyData> ReadPolyData(const char *fileName);
//void PositionCamera(vtkSmartPointer<vtkRenderer> &renderer,
//                    double *viewUp,
//                    double *position);
//}
//
//int main(int argc, char *argv[]) {
//  vtkSmartPointer<vtkPolyData> input1;
//  vtkSmartPointer<vtkPolyData> input2;
//
//  std::string operation("intersection");
//
//  if (argc == 4) {
//    vtkSmartPointer<vtkPolyData> poly1;
//    poly1 = ReadPolyData(argv[1]);
//    vtkSmartPointer<vtkTriangleFilter> tri1 =
//      vtkSmartPointer<vtkTriangleFilter>::New();
//    tri1->SetInputData(poly1);
//    vtkSmartPointer<vtkCleanPolyData> clean1 =
//      vtkSmartPointer<vtkCleanPolyData>::New();
//    clean1->SetInputConnection(tri1->GetOutputPort());
//    clean1->Update();
//    input1 = clean1->GetOutput();
//
//    vtkSmartPointer<vtkPolyData> poly2;
//    poly2 = ReadPolyData(argv[3]);
//    vtkSmartPointer<vtkTriangleFilter> tri2 =
//      vtkSmartPointer<vtkTriangleFilter>::New();
//    tri2->SetInputData(poly2);
//    tri2->Update();
//    vtkSmartPointer<vtkCleanPolyData> clean2 =
//      vtkSmartPointer<vtkCleanPolyData>::New();
//    clean2->SetInputConnection(tri2->GetOutputPort());
//    clean2->Update();
//    input2 = clean2->GetOutput();
//    operation = argv[2];
//  } else {
//    vtkSmartPointer<vtkSphereSource> sphereSource1 =
//      vtkSmartPointer<vtkSphereSource>::New();
//    sphereSource1->SetCenter(.25, 0, 0);
//    sphereSource1->SetPhiResolution(21);
//    sphereSource1->SetThetaResolution(21);
//    sphereSource1->Update();
//    input1 = sphereSource1->GetOutput();
//
//    vtkSmartPointer<vtkSphereSource> sphereSource2 =
//      vtkSmartPointer<vtkSphereSource>::New();
//    sphereSource2->Update();
//    input2 = sphereSource2->GetOutput();
//
//    if (argc == 2) {
//      operation = argv[1];
//    }
//  }
//
//  vtkSmartPointer<vtkNamedColors> colors =
//    vtkSmartPointer<vtkNamedColors>::New();
//
//  vtkSmartPointer<vtkPolyDataMapper> input1Mapper =
//    vtkSmartPointer<vtkPolyDataMapper>::New();
//  input1Mapper->SetInputData(input1);
//  input1Mapper->ScalarVisibilityOff();
//  vtkSmartPointer<vtkActor> input1Actor =
//    vtkSmartPointer<vtkActor>::New();
//  input1Actor->SetMapper(input1Mapper);
//  input1Actor->GetProperty()->SetDiffuseColor(colors->GetColor3d("Tomato").GetData());
//  input1Actor->GetProperty()->SetSpecular(.6);
//  input1Actor->GetProperty()->SetSpecularPower(20);
//  input1Actor->SetPosition(
//    input1->GetBounds()[1] - input1->GetBounds()[0],
//    0, 0);
//  vtkSmartPointer<vtkPolyDataMapper> input2Mapper =
//    vtkSmartPointer<vtkPolyDataMapper>::New();
//  input2Mapper->SetInputData(input2);
//  input2Mapper->ScalarVisibilityOff();
//  vtkSmartPointer<vtkActor> input2Actor =
//    vtkSmartPointer<vtkActor>::New();
//  input2Actor->SetMapper(input2Mapper);
//  input2Actor->GetProperty()->SetDiffuseColor(colors->GetColor3d("Mint").GetData());
//  input2Actor->GetProperty()->SetSpecular(.6);
//  input2Actor->GetProperty()->SetSpecularPower(20);
//  input2Actor->SetPosition(
//    -(input1->GetBounds()[1] - input1->GetBounds()[0]),
//    0, 0);
//  vtkSmartPointer<vtkLoopBooleanPolyDataFilter> booleanOperation =
//    vtkSmartPointer<vtkLoopBooleanPolyDataFilter>::New();
//  if (operation == "union") {
//    booleanOperation->SetOperationToUnion();
//  } else if (operation == "intersection") {
//    booleanOperation->SetOperationToIntersection();
//  } else if (operation == "difference") {
//    booleanOperation->SetOperationToDifference();
//  } else {
//    std::cout << "Unknown operation: " << operation << std::endl;
//    return EXIT_FAILURE;
//  }
//  booleanOperation->SetInputData(0, input1);
//  booleanOperation->SetInputData(1, input2);
//
//  vtkSmartPointer<vtkPolyDataMapper> booleanOperationMapper =
//    vtkSmartPointer<vtkPolyDataMapper>::New();
//  booleanOperationMapper->SetInputConnection(booleanOperation->GetOutputPort());
//  booleanOperationMapper->ScalarVisibilityOff();
//
//  vtkSmartPointer<vtkActor> booleanOperationActor =
//    vtkSmartPointer<vtkActor>::New();
//  booleanOperationActor->SetMapper(booleanOperationMapper);
//  booleanOperationActor->GetProperty()->SetDiffuseColor(colors->GetColor3d("Banana").GetData());
//  booleanOperationActor->GetProperty()->SetSpecular(.6);
//  booleanOperationActor->GetProperty()->SetSpecularPower(20);
//
//  vtkSmartPointer<vtkRenderer> renderer =
//    vtkSmartPointer<vtkRenderer>::New();
//  renderer->AddViewProp(input1Actor);
//  renderer->AddViewProp(input2Actor);
//  renderer->AddViewProp(booleanOperationActor);
//  renderer->SetBackground(colors->GetColor3d("Silver").GetData());
//  vtkSmartPointer<vtkRenderWindow> renderWindow =
//    vtkSmartPointer<vtkRenderWindow>::New();
//  renderWindow->AddRenderer(renderer);
//  renderWindow->SetSize(640, 480);
//  double viewUp[3] = { 0.0, 0.0, 1.0 };
//  double position[3] = { 0.0, -1.0, 0.0 };
//  PositionCamera(renderer, viewUp, position);
//  renderer->GetActiveCamera()->Dolly(1.5);
//  renderer->ResetCameraClippingRange();
//
//  vtkSmartPointer<vtkRenderWindowInteractor> renWinInteractor =
//    vtkSmartPointer<vtkRenderWindowInteractor>::New();
//  renWinInteractor->SetRenderWindow(renderWindow);
//
//  renderWindow->Render();
//  renWinInteractor->Start();
//
//  return EXIT_SUCCESS;
//}
//namespace {
//vtkSmartPointer<vtkPolyData> ReadPolyData(const char *fileName) {
//  vtkSmartPointer<vtkPolyData> polyData;
//  std::string extension = vtksys::SystemTools::GetFilenameExtension(std::string(fileName));
//  if (extension == ".ply") {
//    vtkSmartPointer<vtkPLYReader> reader =
//      vtkSmartPointer<vtkPLYReader>::New();
//    reader->SetFileName(fileName);
//    reader->Update();
//    polyData = reader->GetOutput();
//  } else if (extension == ".vtp") {
//    vtkSmartPointer<vtkXMLPolyDataReader> reader =
//      vtkSmartPointer<vtkXMLPolyDataReader>::New();
//    reader->SetFileName(fileName);
//    reader->Update();
//    polyData = reader->GetOutput();
//  } else if (extension == ".obj") {
//    vtkSmartPointer<vtkOBJReader> reader =
//      vtkSmartPointer<vtkOBJReader>::New();
//    reader->SetFileName(fileName);
//    reader->Update();
//    polyData = reader->GetOutput();
//  } else if (extension == ".stl") {
//    vtkSmartPointer<vtkSTLReader> reader =
//      vtkSmartPointer<vtkSTLReader>::New();
//    reader->SetFileName(fileName);
//    reader->Update();
//    polyData = reader->GetOutput();
//  } else if (extension == ".vtk") {
//    vtkSmartPointer<vtkPolyDataReader> reader =
//      vtkSmartPointer<vtkPolyDataReader>::New();
//    reader->SetFileName(fileName);
//    reader->Update();
//    polyData = reader->GetOutput();
//  } else if (extension == ".g") {
//    vtkSmartPointer<vtkBYUReader> reader =
//      vtkSmartPointer<vtkBYUReader>::New();
//    reader->SetGeometryFileName(fileName);
//    reader->Update();
//    polyData = reader->GetOutput();
//  } else {
//    vtkSmartPointer<vtkSphereSource> source =
//      vtkSmartPointer<vtkSphereSource>::New();
//    source->Update();
//    polyData = source->GetOutput();
//  }
//  return polyData;
//}
//
//void PositionCamera(vtkSmartPointer<vtkRenderer> &renderer,
//                    double *viewUp,
//                    double *position) {
//  renderer->GetActiveCamera()->SetFocalPoint(0.0, 0.0, 0.0);
//  renderer->GetActiveCamera()->SetViewUp(viewUp);
//  renderer->GetActiveCamera()->SetPosition(position);
//  renderer->ResetCamera();
//  return;
//}
//}

//#include <vtkSmartPointer.h>
//#include <vtkVectorText.h>
//#include <vtkLinearExtrusionFilter.h>
//#include <vtkTriangleFilter.h>
//
//#include <vtkDataSetMapper.h>
//#include <vtkActor.h>
//#include <vtkProperty.h>
//#include <vtkCamera.h>
//#include <vtkRenderWindow.h>
//#include <vtkRenderer.h>
//#include <vtkRenderWindowInteractor.h>
//
//int main(int, char *[]) {
//  // Create vector text
//  vtkSmartPointer<vtkVectorText> vecText =
//    vtkSmartPointer<vtkVectorText>::New();
//  vecText->SetText("iText!");
//
//  // Apply linear extrusion
//  vtkSmartPointer<vtkLinearExtrusionFilter> extrude =
//    vtkSmartPointer<vtkLinearExtrusionFilter>::New();
//  extrude->SetInputConnection(vecText->GetOutputPort());
//  extrude->SetExtrusionTypeToNormalExtrusion();
//  extrude->SetVector(0, 0, 1);
//  extrude->SetScaleFactor(0.5);
//
//  vtkSmartPointer<vtkTriangleFilter> triangleFilter =
//    vtkSmartPointer<vtkTriangleFilter>::New();
//  triangleFilter->SetInputConnection(extrude->GetOutputPort());
//
//  vtkSmartPointer<vtkDataSetMapper> mapper =
//    vtkSmartPointer<vtkDataSetMapper>::New();
//  mapper->SetInputConnection(triangleFilter->GetOutputPort());
//
//  vtkSmartPointer<vtkActor> actor =
//    vtkSmartPointer<vtkActor>::New();
//  actor->SetMapper(mapper);
//  actor->GetProperty()->SetColor(0.8900, 0.8100, 0.3400);
//
//  vtkSmartPointer<vtkRenderWindow> renderWindow =
//    vtkSmartPointer<vtkRenderWindow>::New();
//
//  vtkSmartPointer<vtkRenderer> renderer =
//    vtkSmartPointer<vtkRenderer>::New();
//  renderer->SetBackground(.4, .5, .7);
//
//  renderWindow->AddRenderer(renderer);
//
//  renderer->AddActor(actor);
//
//  renderer->ResetCamera();
//  // Generate an interesting view
//  renderer->ResetCamera();
//  renderer->GetActiveCamera()->Azimuth(30);
//  renderer->GetActiveCamera()->Elevation(30);
//  renderer->GetActiveCamera()->Dolly(1.0);
//  renderer->ResetCameraClippingRange();
//
//  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
//    vtkSmartPointer<vtkRenderWindowInteractor>::New();
//  renderWindowInteractor->SetRenderWindow(renderWindow);
//  renderWindow->Render();
//  renderWindowInteractor->Start();
//
//  return EXIT_SUCCESS;
//}