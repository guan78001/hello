#pragma once
#ifndef __OPENCL_INSTANCE_H__
#define __OPENCL_INSTANCE_H__

#include <CL/cl.h>
#include <stdio.h>
#include <vector>
#include <string>
#include "MultiOpenCLDev.h"

class Opencl_Instance {
 public:
  struct KernelArgs {
    size_t arg_size;
    const void *ptr_arg_value;
  };

  Opencl_Instance();

  void Init();

  ~Opencl_Instance();

  //!Release all allocated OpenCL resource
  void Release();

  //!Build OpenCL program
  int BuildProgram(const char *kernelSourceCode, const char *kernel_name[], unsigned int kernel_count);

  //!Run OpenCL kernel
  int runKernel(
    cl_kernel kernel,
    KernelArgs *args,
    unsigned int arg_count,
    cl_uint           work_dim ,
    const size_t     *global_work_size ,
    const size_t     *local_work_size);

  cl_context &GetContext() { return m_context; }
  cl_command_queue &GetCommandQueue() { return m_commandQueue; }
  cl_kernel GetKernel(unsigned int kernelId) {
    if (kernelId < m_kernel_count) {
      return m_kernel[kernelId];
    }
    printf("ERROR: Invalid kernelId\n");
    return NULL;
  }

  //!Get MaxWorkGroupSize of selected GPU device.
  unsigned int GetMaxWorkGroupSize() { return m_dev_max_work_group_size; }

  //!Check OpenCL is valid
  bool IsOpenCLValid() { return m_is_opencl_valid; }
 private:
  void SelectBestDevice();
  void ReleaseKernelAndProgram();
 private:
  static MultiOpenCLDev g_m_dev;
  cl_context m_context;
  cl_command_queue m_commandQueue;
  cl_platform_id m_platform;
  cl_device_id opencl_device_id_;

  cl_program m_program;
  cl_kernel *m_kernel;
  unsigned int m_kernel_count;
  unsigned int m_dev_max_work_group_size;
  bool m_is_opencl_valid;
  std::vector<std::string> m_kernel_name;
};

#endif