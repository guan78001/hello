
#define __CL_ENABLE_EXCEPTIONS

#if defined(__APPLE__) || defined(__MACOSX)
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
using namespace std;
#define KERNEL_CODE(...) #__VA_ARGS__

std::string reduce_str  = KERNEL_CODE(
__kernel void reduce1(__global int *g_idata, __global int *g_odata) {
  __local int sdata[512];

  // each thread loads one element from global to shared mem
  unsigned int tid = get_local_id(0);
  unsigned int i = get_global_id(0);
  sdata[tid] = g_idata[i];
  barrier(CLK_LOCAL_MEM_FENCE);
  // do reduction in shared mem
  int blockSize = get_local_size(0);
  for (unsigned int s = 1; s < blockSize; s *= 2) {
    if (tid % (2 * s) == 0) {
      sdata[tid] += sdata[tid + s];
    }
    barrier(CLK_LOCAL_MEM_FENCE);
  }
  int groupId = get_group_id(0);
  // write result for this block to global mem
  if (tid == 0) g_odata[groupId] = sdata[0];
}

__kernel void reduce2(__global int *g_idata, __global int *g_odata) {
  __local int sdata[512];

  // each thread loads one element from global to shared mem
  unsigned int tid = get_local_id(0);
  unsigned int i = get_global_id(0);
  sdata[tid] = g_idata[i];
  barrier(CLK_LOCAL_MEM_FENCE);
  // do reduction in shared mem
  int blockSize = get_local_size(0);
  for (unsigned int s = 1; s < blockSize; s *= 2) {
    //if (tid % (2 * s) == 0) {
    //  sdata[tid] += sdata[tid + s];
    //}
    int index = 2 * s * tid;
    if (index < blockSize) {
      sdata[index] += sdata[index + s];
    }
    barrier(CLK_LOCAL_MEM_FENCE);
  }
  int groupId = get_group_id(0);
  // write result for this block to global mem
  if (tid == 0) g_odata[groupId] = sdata[0];
}

__kernel void reduce3(__global int *g_idata, __global int *g_odata) {
  __local int sdata[512];

  // each thread loads one element from global to shared mem
  unsigned int tid = get_local_id(0);
  unsigned int i = get_global_id(0);
  sdata[tid] = g_idata[i];
  barrier(CLK_LOCAL_MEM_FENCE);
  // do reduction in shared mem
  int blockSize = get_local_size(0);
  for (unsigned int s = blockSize / 2; s > 0; s >>= 1) {
    if (tid < s) {
      sdata[tid] += sdata[tid + s];
    }
    barrier(CLK_LOCAL_MEM_FENCE);
  }
  int groupId = get_group_id(0);
  // write result for this block to global mem
  if (tid == 0) g_odata[groupId] = sdata[0];
}

__kernel void reduce4(__global int *g_idata, __global int *g_odata) {
  __local int sdata[512];

  // perform first level of reduction,
  // reading from global memory, writing to shared memory
  unsigned int tid = get_local_id(0);
  //unsigned int i = get_global_id(0);
  unsigned int i = get_group_id(0) * get_local_size(0) * 2 + tid;  // blockIdx.x * (blockDim.x * 2) + threadIdx.x;
  int blockSize = get_local_size(0);
  sdata[tid] = g_idata[i] + g_idata[i + blockSize];
  barrier(CLK_LOCAL_MEM_FENCE);

  // do reduction in shared mem
  for (unsigned int s = blockSize / 2; s > 0; s >>= 1) {
    if (tid < s) {
      sdata[tid] += sdata[tid + s];
    }
    barrier(CLK_LOCAL_MEM_FENCE);
  }
  int groupId = get_group_id(0);
  // write result for this block to global mem
  if (tid == 0) g_odata[groupId] = sdata[0];
}

__kernel void reduce5(__global int *g_idata, __global int *g_odata) {
  __local int sdata[512];

  // perform first level of reduction,
  // reading from global memory, writing to shared memory
  unsigned int tid = get_local_id(0);
  // unsigned int i = get_global_id(0);
  unsigned int i = get_group_id(0) * get_local_size(0) * 2 +
                   tid;  // blockIdx.x * (blockDim.x * 2) + threadIdx.x;
  int blockSize = get_local_size(0);
  sdata[tid] = g_idata[i] + g_idata[i + blockSize];
  barrier(CLK_LOCAL_MEM_FENCE);

  // do reduction in shared mem
  for (unsigned int s = blockSize / 2; s > 32; s >>= 1) {
    if (tid < s) {
      sdata[tid] += sdata[tid + s];
    }
    barrier(CLK_LOCAL_MEM_FENCE);
  }
  if (tid < 32) {
    sdata[tid] += sdata[tid + 16];
    sdata[tid] += sdata[tid + 8];
    sdata[tid] += sdata[tid + 4];
    sdata[tid] += sdata[tid + 2];
    sdata[tid] += sdata[tid + 1];
  }

  int groupId = get_group_id(0);
  // write result for this block to global mem
  if (tid == 0) g_odata[groupId] = sdata[0];
}

__kernel void reduce6(__global int *g_idata, __global int *g_odata) {
  __local int sdata[512];

  // perform first level of reduction,
  // reading from global memory, writing to shared memory
  unsigned int tid = get_local_id(0);
  // unsigned int i = get_global_id(0);
  unsigned int i = get_group_id(0) * get_local_size(0) * 2 +
                   tid;  // blockIdx.x * (blockDim.x * 2) + threadIdx.x;
  int blockSize = get_local_size(0);
  sdata[tid] = g_idata[i] + g_idata[i + blockSize];
  barrier(CLK_LOCAL_MEM_FENCE);

  if (blockSize > 256) {
    if (tid < 256) sdata[tid] += sdata[tid + 256];
    barrier(CLK_LOCAL_MEM_FENCE);
  }

  if (blockSize > 128) {
    if (tid < 128) sdata[tid] += sdata[tid + 128];
    barrier(CLK_LOCAL_MEM_FENCE);
  }

  if (blockSize > 64) {
    if (tid < 64) sdata[tid] += sdata[tid + 64];
    barrier(CLK_LOCAL_MEM_FENCE);
  }

  if (tid < 32) {
    sdata[tid] += sdata[tid + 16];
    sdata[tid] += sdata[tid + 16];
    sdata[tid] += sdata[tid + 8];
    sdata[tid] += sdata[tid + 4];
    sdata[tid] += sdata[tid + 2];
    sdata[tid] += sdata[tid + 1];
  }

  int groupId = get_group_id(0);
  // write result for this block to global mem
  if (tid == 0) g_odata[groupId] = sdata[0];
}

                          );

const int N = 1 << 22;  // 1024 * 64;
std::vector<int> in(N);
std::vector<int> out(N);
size_t bytes = N * sizeof(int);

int Test_reduction_str(cl::Context &context, cl::Program &program_, cl::Device &device, const std::string &kernel_str, int globalSize) {
  //1.define GPU buffer memory
  cl::Buffer buf_in(context, CL_MEM_READ_WRITE, bytes, 0, 0);
  cl::Buffer buf_out(context, CL_MEM_READ_WRITE, bytes, 0, 0);
  cl_int err = 0;
  //2.set kernel
  cl::Kernel kernel(program_, kernel_str.c_str(), &err);
  err |= kernel.setArg(0, buf_in);
  err |= kernel.setArg(1, buf_out);

  //3.define CPU buffer memory
  //std::vector<int> in(N);
  //std::vector<int> out(N);


  //4.define event and queue
  cl::Event event;
  cl::CommandQueue queue(context, device, 0, &err);

  //5.copy data from CPU to GPU
  queue.enqueueWriteBuffer(buf_in, false, 0, bytes, &in[0]);

  //6.run kernel
  err |= queue.enqueueNDRangeKernel(
           kernel,
           cl::NullRange,
           cl::NDRange(globalSize),
           cl::NDRange(512),  // cl::NullRange,
           NULL,
           &event);

  //7.read result from GPU
  err |= queue.enqueueReadBuffer(buf_out, false, 0, bytes, out.data(), NULL, &event);
  event.wait();
  return err;
}

int main(void) {
  cl_int err = CL_SUCCESS;
  try {
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    cl::STRING_CLASS str;

    int platformId = 0;
    for (int i = 0; i < platforms.size(); i++) {
      platforms[i].getInfo(CL_PLATFORM_VENDOR, &str);
      cout << i << " :" << str << endl;
      if (str.find("NVIDIA") != str.npos) {
        platformId = i;
        printf("use platform id=%d\n", platformId);
      }
    }

    if (platforms.size() == 0) {
      std::cout << "Platform size 0\n";
      return -1;

    }

    cl_context_properties properties[] =
    { CL_CONTEXT_PLATFORM, (cl_context_properties)(platforms[platformId])(), 0 };
    cl::Context context(CL_DEVICE_TYPE_GPU, properties);

    std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();

    cl::Program::Sources source;
    source.push_back(std::make_pair(reduce_str.c_str(), reduce_str.length()));

    cl::Program program_ = cl::Program(context, source);
    err = program_.build(devices);

    if (devices.empty()) {
      printf("[Error], devices is empty.\n");
    }

    for (int i = 0; i < N; i++) {
      in[i] = 1;
    }
    //execute kernel.
    err = Test_reduction_str(context, program_, devices[0], "reduce1", N);
    err = Test_reduction_str(context, program_, devices[0], "reduce2", N);
    err = Test_reduction_str(context, program_, devices[0], "reduce3", N);
    err = Test_reduction_str(context, program_, devices[0], "reduce4", N / 2);
    err = Test_reduction_str(context, program_, devices[0], "reduce5", N / 2);
    err = Test_reduction_str(context, program_, devices[0], "reduce6", N / 2);
  } catch (cl::Error err) {
    std::cerr
        << "ERROR: "
        << err.what()
        << "("
        << err.err()
        << ")"
        << std::endl;

  }

  return EXIT_SUCCESS;
}


