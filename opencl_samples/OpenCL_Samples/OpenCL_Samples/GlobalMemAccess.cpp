#define __CL_ENABLE_EXCEPTIONS

#include <CL/cl.hpp>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
using namespace std;
#define KERNEL_CODE(...) #__VA_ARGS__

std::string reduce_str  = KERNEL_CODE(
                            __kernel void offsetCopy(
                              __global float *odata,
                              __global float *idata,
int offset) {
  int xid = get_global_id(0) + offset;
  odata[xid] = idata[xid];
}

__kernel void offsetCopy2(
  __global float *odata,
  __global float *idata,
  int offset) {
  int g_id = get_global_id(0);
  int warp_thread_id = g_id % offset;
  int warps = g_id / offset;
  int reverse_id = warps * offset + offset - 1 - warp_thread_id;
  odata[reverse_id] = idata[g_id];
}
                          );

const int N = 1 << 24;  //
std::vector<float> in(N);
std::vector<float> out(N);
size_t bytes = N * sizeof(int);

int Test_OffsetCopy(cl::Context &context, cl::Program &program_, cl::Device &device, const std::string &kernel_str, int globalSize, int offset) {
  //1.define GPU buffer memory
  cl::Buffer buf_in(context, CL_MEM_READ_WRITE, bytes, 0, 0);
  cl::Buffer buf_out(context, CL_MEM_READ_WRITE, bytes, 0, 0);
  cl_int err = 0;
  //2.set kernel
  cl::Kernel kernel(program_, kernel_str.c_str(), &err);
  err |= kernel.setArg(0, buf_out);
  err |= kernel.setArg(1, buf_in);
  err |= kernel.setArg(2, offset);
  //3.define CPU buffer memory
  //std::vector<int> in(N);
  //std::vector<int> out(N);


  //4.define event and queue
  cl::Event event;
  cl_command_queue_properties properties;
  properties = CL_QUEUE_PROFILING_ENABLE;
  cl::CommandQueue queue(context, device, properties, &err);

  //5.copy data from CPU to GPU
  queue.enqueueWriteBuffer(buf_in, false, 0, bytes, &in[0]);

  //event.wait();
  //6.run kernel
  cl::Event kernel_event;
  err |= queue.enqueueNDRangeKernel(
           kernel,
           cl::NullRange,
           cl::NDRange(globalSize),
           cl::NDRange(512),  // cl::NullRange,
           NULL,
           &kernel_event);
  kernel_event.wait();

  //std::cout << "Kernel execution time [ns]:" << endl;
  //    << kernel_event.getProfilingInfo<CL_PROFILING_COMMAND_END>() << endl
  //    << kernel_event.getProfilingInfo<CL_PROFILING_COMMAND_START>() << endl
  //    << std::endl;

  uint64_t nstimestart, nstimeend;
  kernel_event.getProfilingInfo<uint64_t>(CL_PROFILING_COMMAND_START, &nstimestart);
  kernel_event.getProfilingInfo<uint64_t>(CL_PROFILING_COMMAND_END, &nstimeend);

  auto exec_time = nstimeend - nstimestart;
  cout << "offset=" << offset << ", ns_exec_time=" << exec_time << endl;
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

    vector<int> offsets{ 0, 1, 2, 4, 8, 16, 32, 64, 128, 256 };
    for (int offset : offsets) {
      //execute kernel.
      for (int i = 0; i < 1024; i++) {
        in[i] = i;
      }

      Test_OffsetCopy(context, program_, devices[0], "offsetCopy", N - 512, offset );
    }

    cout << "reverse pattern:" << endl;
    vector<int> offsets2{ 1, 2, 4, 8, 16, 32, 64, 128, 256 };
    for (int offset : offsets2) {
      //execute kernel.
      Test_OffsetCopy(context, program_, devices[0], "offsetCopy2", N, offset );
      cout << "output:\n";
      for (int i = 0; i < 32; i++) {
        cout << out[i] << " ";
      }
      cout << endl;
    }
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


