#include "Opencl_Instance.h"

#include <memory>
#include <vector>

#include <vector>
#include <string>

#include "MultiOpenCLDev.h"
//#include "Omp_Timer.h"
#define IS_TEST_CL_PROFLING 0

typedef unsigned int uint;

MultiOpenCLDev Opencl_Instance::g_m_dev;
Opencl_Instance::Opencl_Instance() {
  Init();

}
Opencl_Instance::~Opencl_Instance() {
  //printf("Opencl_Instance::~Opencl_Instance\n");
  Release();
}
#define DEVICE_INFO(device,cl_name) {		cl_uint buf_uint;\
clGetDeviceInfo(device, cl_name, sizeof(buf_uint), &buf_uint, NULL);\
	printf(#cl_name ## "=%d\n",buf_uint);}


#define DEVICE_STR(device,cl_name) {		char buf_str[512];\
clGetDeviceInfo(device, cl_name, sizeof(buf_str), &buf_str, NULL);\
	printf(#cl_name ## "=%s\n",buf_str);}

//#define DEBUG(str) printf(str ## "\n");
int Opencl_Instance::BuildProgram(const char *kernelSourceCode, const char *kernel_name[], unsigned int kernel_count) {
  m_is_opencl_valid = false;//fail

  SelectBestDevice();
  //clCreateProgramWithSource
  size_t sourceSize[] = {strlen(kernelSourceCode)};
  cl_int status = 0;
  m_program = clCreateProgramWithSource(m_context, 1, &kernelSourceCode, sourceSize, &status);
  if (status != CL_SUCCESS) {
    printf("Error: Loading source code into cl_program (clCreateProgramWithSource),err=%d\n", status);
    return EXIT_FAILURE;
  }

  //clBuildProgram
  status = clBuildProgram(m_program, 1, &opencl_device_id_, NULL, NULL, NULL);
// g_m_dev.oclCreateCommandQueue();
  if (status != CL_SUCCESS) {
    printf("Error: Building Program (clBuildProgram),err=%d\n", status);
    size_t buffer_len = 0;
    cl_int cl_error_num = clGetProgramBuildInfo(m_program, opencl_device_id_, CL_PROGRAM_BUILD_LOG, 0, 0, &buffer_len);

    char *ptr_build_log = new char[buffer_len];
    cl_error_num = clGetProgramBuildInfo(m_program, opencl_device_id_, CL_PROGRAM_BUILD_LOG, buffer_len, ptr_build_log, NULL);
    printf("OpenCLManager [CreateProgramWithSource]: ProgramBuildInfo \n%s\r\n", ptr_build_log);

    delete[] ptr_build_log;
    return EXIT_FAILURE;
  }
  //cl_int err;
  //m_commandQueue = clCreateCommandQueue(m_context, opencl_device_id_, CL_QUEUE_PROFILING_ENABLE, &err);

  //clCreateKernel
  m_kernel_count = kernel_count;
  m_kernel = (cl_kernel *)malloc(sizeof(cl_kernel) * m_kernel_count);
  m_kernel_name.reserve(m_kernel_count);
  for (unsigned int i = 0; i < m_kernel_count; i++) {
    m_kernel[i] = clCreateKernel(m_program, kernel_name[i], &status);
    m_kernel_name.push_back(kernel_name[i]);
    if (status != CL_SUCCESS) {
      printf("Error: Creating Kernel from program. (clCreateKernel),err=%d\n", status);
      return EXIT_FAILURE;
    }
  }
  //printf("m_kernel_name.size()=%d", m_kernel_name.size());
  m_is_opencl_valid = true;
  return status;
}

int Opencl_Instance::runKernel(cl_kernel kernel, KernelArgs *args,  unsigned int arg_count, cl_uint work_dim , const size_t *global_work_size , const size_t *local_work_size ) {
  //printf("\n");
  //OMP_Timer timer("Opencl_Instance::runKernel");

  int kernel_id = 0;
  for (int i = 0; i < m_kernel_count; i++) {
    if (m_kernel[i] == kernel) {
      kernel_id = i;
      break;
    }
  }

  cl_int status = 0;
  if (!kernel || !args || work_dim < 1  || !global_work_size  ) {
    printf("Error: runKernel param invalid! err=%d\n", status);
    return EXIT_FAILURE;
  }
  cl_kernel  cur_kernel = kernel;

  //clSetKernelArg
  for (unsigned int i = 0; i < arg_count; i++) {
    status = clSetKernelArg(cur_kernel, i, args[i].arg_size, args[i].ptr_arg_value);
    if (status != CL_SUCCESS) {
      printf("Error: Setting kernel argument.err=%d\n", status);
      return EXIT_FAILURE;
    }
  }
  //timer.PrintStepTime("set kernel Arg");
// cl_event ev=NULL;
  //run kernel
  status = clEnqueueNDRangeKernel(
             m_commandQueue, cur_kernel,
             work_dim, NULL, global_work_size,
             local_work_size, 0, NULL, NULL);
  if (status != CL_SUCCESS) {
    printf("Error: Enqueue kernel %s ret=%d\n", m_kernel_name[kernel_id].c_str(), status);
    return EXIT_FAILURE;
  }
// timer.PrintStepTime("clEnqueueNDRangeKernel");
  //clFlush(m_commandQueue);
  //timer.PrintStepTime("clFlush");
  //wait finish
  if (0 && status == CL_SUCCESS) {
    status = clFinish(m_commandQueue);
    if (status != CL_SUCCESS) {
      printf("Error: clFinish kernel %s ret=%d\n", m_kernel_name[kernel_id].c_str(), status);
      return EXIT_FAILURE;
    }
  }
  //timer.PrintStepTime("clFinish");

#if IS_TEST_CL_PROFLING
  cl_ulong queuedTime = 0, submitTime = 0, startTime = 0, endTime = 0;
  clGetEventProfilingInfo(ev, CL_PROFILING_COMMAND_QUEUED,
                          sizeof(cl_ulong), &queuedTime, NULL);
  clGetEventProfilingInfo(ev, CL_PROFILING_COMMAND_SUBMIT,
                          sizeof(cl_ulong), &submitTime, NULL);
  clGetEventProfilingInfo(ev, CL_PROFILING_COMMAND_START,
                          sizeof(cl_ulong), &startTime, NULL);
  clGetEventProfilingInfo(ev, CL_PROFILING_COMMAND_END,
                          sizeof(cl_ulong), &endTime, NULL);
  cl_ulong queued_submit_time = submitTime - queuedTime;
  cl_ulong submit_start_time = startTime - submitTime;
  cl_ulong excute_time = endTime - startTime;
  cl_ulong all_time = endTime - queuedTime;
  printf("%s exec time=%8.6f, queued_submit_time=%8.6f,submit_start_time=%8.6f excute_time=%8.6f ms\n",
         m_kernel_name[kernel_id].c_str(),
         all_time * 1e-6,
         queued_submit_time * 1e-6,
         submit_start_time * 1e-6,
         excute_time * 1e-6);
  timer.PrintStepTime("ProfilingInfo");
#endif
  return status;
}

void Opencl_Instance::ReleaseKernelAndProgram() {
  //printf("Opencl_Instance::ReleaseKernelAndProgram\n");
  if (m_kernel) {
    for (unsigned int i = 0; i < m_kernel_count; i++) {
      cl_int ret = clReleaseKernel(m_kernel[i]);
      //printf("clReleaseKernel %d,ret=%d\n", i, ret);
    }
    free(m_kernel);
    //printf("free all kernels\n");
  }
  if (m_program) {
    cl_int ret = clReleaseProgram(m_program);
    //printf("clReleaseProgram,ret=%d\n", ret);
  }
}

void Opencl_Instance::Release() {
  if (this->m_is_opencl_valid) {
    ReleaseKernelAndProgram();
    g_m_dev.oclRelease();
    m_is_opencl_valid = false;
  }
  m_kernel_name.clear();
  Init();
}

void Opencl_Instance::SelectBestDevice() {
  int nSelected_DevId = 0;//the first is the best device.

  g_m_dev.BuildContext();
  if (g_m_dev.devices_.size() > 0) {
    m_context = g_m_dev.devices_[nSelected_DevId].context;
    opencl_device_id_ = g_m_dev.devices_[nSelected_DevId].device_id;
    m_commandQueue = g_m_dev.devices_[nSelected_DevId].command_queue;
    m_dev_max_work_group_size = g_m_dev.devices_[nSelected_DevId].device_info.max_work_group_size;
  }
}

void Opencl_Instance::Init() {
  this->m_platform = NULL;
  this->m_context = NULL;
  this->m_commandQueue = NULL;
  this->m_program = NULL;
  this->m_kernel = NULL;
  m_dev_max_work_group_size = 1024;
  m_is_opencl_valid = false;
}

