#ifndef __vtkTestFilter_h
#define __vtkTestFilter_h

#include "vtkPolyDataAlgorithm.h"
#include <thread>
class vtkTestFilter : public vtkPolyDataAlgorithm {
 public:
  vtkTypeMacro(vtkTestFilter, vtkPolyDataAlgorithm);
  static vtkTestFilter *New();
  void Test() {
    std::thread th(
    [&]() {
      for (size_t i = 0; i < 10; i++) {
        this->InvokeEvent(this->RefreshEvent, NULL);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      }
    });
    th.join();
  }
  int RefreshEvent;

 protected:
  vtkTestFilter();

  int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

 private:
  vtkTestFilter(const vtkTestFilter &); // Not implemented.
  void operator=(const vtkTestFilter &); // Not implemented.

};

#endif

#include "vtkObjectFactory.h"
#include "vtkCommand.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"
#include "vtkDataObject.h"
#include "vtkSmartPointer.h"
#include "vtkAppendPolyData.h"
#include "vtkSphereSource.h"

vtkStandardNewMacro(vtkTestFilter);

vtkTestFilter::vtkTestFilter() {
  this->SetNumberOfInputPorts(0);

  this->RefreshEvent = vtkCommand::UserEvent + 1;

}

int vtkTestFilter::RequestData(vtkInformation *vtkNotUsed(request),
                               vtkInformationVector **inputVector,
                               vtkInformationVector *outputVector) {
  // Get the info object
//  vtkInformation *outInfo = outputVector->GetInformationObject(0);

//  vtkPolyData *output = vtkPolyData::SafeDownCast(
//      outInfo->Get(vtkDataObject::DATA_OBJECT()));
  this->InvokeEvent(this->RefreshEvent, NULL);

  return 1;
}