#pragma once
#ifndef __MULTI_OPENCL_DEV_H__
#define __MULTI_OPENCL_DEV_H__

#include <CL/cl.h>
#include <vector>

typedef unsigned int uint;
struct DeviceInfo {
  std::string device_name;
  cl_device_type device_type;
  uint max_compute_unit;
  size_t max_work_group_size;
};
struct OpenCLDevParam {
  cl_platform_id plat_form_id = NULL;
  cl_device_id device_id = NULL;
  cl_device_type device_type;
  cl_context context = NULL;
  cl_program program_ = NULL;
  std::vector<cl_kernel> kernels_;
  cl_command_queue command_queue;
  DeviceInfo device_info;
};
class MultiOpenCLDev {
 public:
  MultiOpenCLDev();
  ~MultiOpenCLDev();

  void BuildContext();
  void oclRelease();
  void BuildProgram(const char *source, const char *kernel_name[], int kernel_num);
  void SortPlatform(cl_uint num_platforms);
  void oclGetDevicesByType(cl_platform_id cpPlatform, cl_device_type dev_type);

  //test function:
  void print_device_list();
  static void sample();
 protected:
  cl_int oclGetPlatformIDs();
  cl_int oclCreateContexts();
  cl_int oclGetDevices();
  cl_int oclCreateCommandQueue();

  cl_int oclBuildProgram(const char *source, const char *kernel_name[], int kernel_num);
 public:
  std::vector<cl_platform_id> platforms_;
  std::vector<OpenCLDevParam> devices_;
 protected:
  bool is_valid = false;//avoid create/release multi times
};

#endif