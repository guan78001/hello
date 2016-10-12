#include "MultiOpenCLDev.h"

#include <stdio.h>
#include <string>
#include <inttypes.h>
#include "trace_shared.h"
MultiOpenCLDev::MultiOpenCLDev() {
}
MultiOpenCLDev::~MultiOpenCLDev() {
  oclRelease();
}

void MultiOpenCLDev::BuildContext() {
  if (is_valid) return;

  oclGetPlatformIDs();
  oclGetDevices();
  oclCreateContexts();
  oclCreateCommandQueue();
  print_device_list();

  is_valid = true;
}
void MultiOpenCLDev::oclRelease() {
  if (!is_valid) return;
  is_valid = false;

  for (size_t i = 0; i < devices_.size(); ++i) {
    cl_int ret = 0;

    if (devices_[i].command_queue) {
      ret = clReleaseCommandQueue(devices_[i].command_queue);
    }

    for (size_t j = 0; j < devices_[i].kernels_.size(); j++) {
      clReleaseKernel(devices_[i].kernels_[j]);
    }
    devices_[i].kernels_.clear();

    if (devices_[i].program_) {
      ret = clReleaseProgram(devices_[i].program_);
      devices_[i].program_ = NULL;
    }

    if (devices_[i].context) {
      ret = clReleaseContext(devices_[i].context);
      devices_[i].context = NULL;
    }
  }
  devices_.clear();
  platforms_.clear();
}
void MultiOpenCLDev::BuildProgram(const char *source, const char *kernel_name[], int kernel_num) {
  oclBuildProgram(source, kernel_name, kernel_num);
}
cl_int MultiOpenCLDev::oclGetPlatformIDs() {
  cl_uint num_platforms;
  cl_int ciErrNum;

  // Get OpenCL platform count
  ciErrNum = clGetPlatformIDs(0, NULL, &num_platforms);
  if (ciErrNum != CL_SUCCESS) {
    printf(" Error %i in clGetPlatformIDs Call !!!\n\n", ciErrNum);
    return -1000;
  } else {
    if (num_platforms == 0) {
      printf("No OpenCL platform found!\n\n");
      return -2000;
    } else {
      // if there's a platform or more, make space for ID's
      platforms_.resize(num_platforms);
      // get platform info for each platform and trap the NVIDIA platform if found
      ciErrNum = clGetPlatformIDs(num_platforms, &platforms_[0], NULL);
    }
  }
  //sort platform as order: NVDIA, AMD, intel
  SortPlatform(num_platforms);

  return CL_SUCCESS;
}


cl_int MultiOpenCLDev::oclGetDevices() {
  for (size_t i = 0; i < platforms_.size(); i++) {
    cl_platform_id cpPlatform = platforms_[i];
    oclGetDevicesByType(cpPlatform, CL_DEVICE_TYPE_GPU); //only use GPU
    //oclGetDevicesByType(cpPlatform, CL_DEVICE_TYPE_CPU);
  }
  return CL_SUCCESS;
}

#define CL_NV_DEVICE_COMPUTE_CAPABILITY_MAJOR       0x4000
#define CL_NV_DEVICE_COMPUTE_CAPABILITY_MINOR       0x4001
#define CL_NV_DEVICE_REGISTERS_PER_BLOCK            0x4002
#define CL_NV_DEVICE_WARP_SIZE                      0x4003
#define CL_NV_DEVICE_GPU_OVERLAP                    0x4004
#define CL_NV_DEVICE_KERNEL_EXEC_TIMEOUT            0x4005
#define CL_NV_DEVICE_INTEGRATED_MEMORY              0x4006

void MultiOpenCLDev::oclGetDevicesByType(cl_platform_id cpPlatform, cl_device_type dev_type) {
  //Get the devices
  cl_uint ciDeviceCount;
  cl_int ciErrNum;
  ciErrNum = clGetDeviceIDs(cpPlatform, dev_type, 0, NULL, &ciDeviceCount);
  if (ciErrNum == CL_SUCCESS) {
    std::vector<cl_device_id> cdDevices(ciDeviceCount);
    ciErrNum = clGetDeviceIDs(cpPlatform, dev_type, ciDeviceCount, &cdDevices[0], NULL);
    if (ciErrNum == CL_SUCCESS) {
      for (cl_uint i = 0; i < ciDeviceCount; i++) {
        OpenCLDevParam dev;
        dev.device_id = cdDevices[i];
        dev.device_type = dev_type;
        dev.plat_form_id = cpPlatform;

        cl_device_id device_id = dev.device_id;
        char cDeviceName[256];
        ciErrNum = clGetDeviceInfo(device_id, CL_DEVICE_NAME, sizeof(cDeviceName), cDeviceName, NULL);
        dev.device_info.device_name = cDeviceName;
        dev.device_info.device_type = dev_type;
        clGetDeviceInfo(device_id, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &dev.device_info.max_compute_unit, NULL);
        clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &dev.device_info.max_work_group_size, NULL);

        cl_uint major = 0;
        clGetDeviceInfo(device_id, CL_NV_DEVICE_COMPUTE_CAPABILITY_MAJOR, sizeof(major), &major, NULL);
        printf("major=%d\n", major);

        devices_.push_back(dev);
      }
    }
  }
}
cl_int MultiOpenCLDev::oclCreateContexts() {
  for (cl_uint i = 0; i < devices_.size(); ++i) {
    // Form common context properties which just select needed platform:
    cl_context_properties context_props[] = {
      CL_CONTEXT_PLATFORM,
      cl_context_properties(devices_[i].plat_form_id),
      0
    };
    cl_int err;
    // Now create context with all devices of a given type available for the selected platform
    devices_[i].context = clCreateContext(
                            context_props,
                            1,
                            &devices_[i].device_id,
                            0,
                            0,
                            &err
                          );
    if (err != CL_SUCCESS) {
      printf("oclCreateContexts failed at i=%d, err=%d\n", i, err);
    }
  }
  return CL_SUCCESS;
}
cl_int MultiOpenCLDev::oclBuildProgram(const char *source, const char *kernel_name[], int kernel_num) {
  for (int i = 0; i < devices_.size(); i++) {
    cl_int err;
    const char *build_option = "";
    cl_program program = clCreateProgramWithSource(devices_[i].context, 1, &source, 0, &err);
    err = clBuildProgram(program, 0, 0, build_option, 0, 0);
    if (err != CL_SUCCESS) {
      printf("oclBuildProgram failed at i=%d, err=%d\n", i, err);
    }
    for (int j = 0; j < kernel_num; j++) {
      cl_kernel kernel = clCreateKernel(program, kernel_name[j], &err);
      devices_[i].kernels_.push_back(kernel);
    }
    devices_[i].program_ = program;
  }
  return CL_SUCCESS;
}

cl_int MultiOpenCLDev::oclCreateCommandQueue() {
  for (size_t i = 0; i < devices_.size(); i++) {
    cl_int err;
    devices_[i].command_queue = clCreateCommandQueue(devices_[i].context, devices_[i].device_id, CL_QUEUE_PROFILING_ENABLE, &err);
    if (err != CL_SUCCESS) {
      printf("oclCreateCommandQueue err=%d at i=%d\n", err, i);
    }
  }
  return CL_SUCCESS;
}

void MultiOpenCLDev::SortPlatform(cl_uint num_platforms) {
  const char *szNVIDIA = "NVIDIA Corporation";
  const char *szIntel = "Intel(R) Corporation";
  const char *szAMD = "Advanced Micro Devices, Inc.";
  for (int i = 0; i < num_platforms; i++) {
    const int Len = 256;
    char name[Len];
    clGetPlatformInfo(platforms_[i], CL_PLATFORM_VENDOR, Len, name, NULL);
    //printf("platform name:%s\n", name);
    std::string str(name);
    if (str.find(szNVIDIA) != str.npos) {
      //printf("found NVIDA Card,set it to rank 1!\n");
      if (i != 0) {
        std::swap(platforms_[0], platforms_[i]);
      }
    }
    if (str.find(szIntel) != str.npos) {
      //printf("found Intel Card,set it to rank last!\n");
      if (i != num_platforms - 1) {
        std::swap(platforms_[num_platforms - 1], platforms_[i]);
      }
    }
  }
}

void MultiOpenCLDev::print_device_list() {
  int maxLen = 0;
  for (int i = 0; i < devices_.size(); i++) {
    int len = devices_[i].device_info.device_name.size();
    if (maxLen < len) {
      maxLen = len;
    }
  }
  for (int i = 0; i < devices_.size(); i++) {
    DeviceInfo &info = devices_[i].device_info;
    PTRACE_INFORMATION("OpenCL_Dev_%d:", i);
    PTRACE_INFORMATION("dev_name=%s", info.device_name.c_str());
    PTRACE_INFORMATION("dev_type=%" PRIu64 "", info.device_type);//64bit
    PTRACE_INFORMATION("max_compute_unit=%d", info.max_compute_unit);
    PTRACE_INFORMATION("max_work_group_size=%d", info.max_work_group_size);
    //PTRACE_INFORMATION("\n");
  }
}

void MultiOpenCLDev::sample() {
#define KERNEL(...) "" #__VA_ARGS__
  const char *
  kernelSourceCode =
    KERNEL(
  __kernel void reduce(__global float * d_Result, __global float * d_Input, int N) {
    const int tid = get_global_id(0);
    const int threadN = get_global_size(0);
    float sum = 0;
    for (int pos = tid; pos < N; pos += threadN) {
      sum += d_Input[pos];
    }
    d_Result[tid] = sum;
  }
    );
  const char *kernel_names[] = { "reduce" };
  enum { KERNEL_REDUCE };
  int kernel_count = sizeof(kernel_names) / sizeof(kernel_names[0]);

  MultiOpenCLDev obj;
  obj.BuildContext();
  obj.BuildProgram(kernelSourceCode, kernel_names, kernel_count);

  for (int dev_id = 0; dev_id < obj.devices_.size(); dev_id++) {
    cl_context context = obj.devices_[dev_id].context;
    cl_command_queue command_queue = obj.devices_[dev_id].command_queue;
    cl_kernel kernel = obj.devices_[dev_id].kernels_[0];

    const int N = 32 * 1024 * 1024;
    const unsigned int BLOCK_N = 128;
    const unsigned int THREAD_N = obj.devices_[dev_id].device_info.max_work_group_size / 2;
    const unsigned int ACCUM_N = BLOCK_N * THREAD_N;

    size_t localWorkSize[] = { THREAD_N };
    size_t globalWorkSize[] = { ACCUM_N };
    std::vector<float> input(N, 1);
    std::vector<float> result(ACCUM_N);

    cl_mem mem_input = clCreateBuffer(context, CL_MEM_READ_ONLY, input.size() * sizeof(float), NULL, NULL);
    cl_mem mem_result = clCreateBuffer(context, CL_MEM_WRITE_ONLY, result.size() * sizeof(float), NULL, NULL);
    clEnqueueWriteBuffer(command_queue, mem_input, CL_TRUE, 0, input.size() * sizeof(float), &input[0], NULL, NULL, NULL);

    cl_int ciErrNum = CL_SUCCESS;
    ciErrNum |= clSetKernelArg(kernel, 0, sizeof(mem_result), &mem_result);
    ciErrNum |= clSetKernelArg(kernel, 1, sizeof(mem_input), &mem_input);
    ciErrNum |= clSetKernelArg(kernel, 2, sizeof(N), &N);

    ciErrNum = clEnqueueNDRangeKernel(command_queue, kernel, 1, 0, globalWorkSize, localWorkSize,
                                      0, NULL, NULL);

    clEnqueueReadBuffer(command_queue, mem_result, CL_TRUE, 0, result.size() * sizeof(float), &result[0], NULL, NULL, NULL);
    //clFinish(command_queue);
    float sum = 0;
    for (int i = 0; i < result.size(); i++) {
      sum += result[i];
    }
    printf("ocl_reduce_sum=%f\n", sum);
    clReleaseMemObject(mem_input);
    clReleaseMemObject(mem_result);
  }
}