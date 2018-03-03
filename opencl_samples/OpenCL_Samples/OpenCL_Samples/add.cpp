
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
using namespace std;
#define KERNEL_CODE(...) #__VA_ARGS__

const char*  vecAdd  = KERNEL_CODE(
__kernel void vecAdd(__global double *a, __global double *b, __global double *c, /*const unsigned */int n) {
  //Get our global thread ID
  int id = get_global_id(0);

  //Make sure we do not go out of bounds
  if (id < n) c[id] = a[id] + b[id];
}

__kernel void cubeScale(__global int *a, __global int *b,int N) {
  int x = get_global_id(0);
  int y = get_global_id(1);
  int z = get_global_id(2);

  int pos = z*N*N + y*N + x;
  b[pos] =2*a[pos];
}
                       );

void Test_cubeScale(cl::Context context, cl::Program program_, cl_int& err, std::vector<cl::Device>& devices) {
  const int N = 32;
  int len = N*N*N;
  size_t bytes = len * sizeof(int);

  //1.define GPU buffer memory
  cl::Buffer buf_a(context, CL_MEM_READ_WRITE, bytes, 0, 0);
  cl::Buffer buf_b(context, CL_MEM_READ_WRITE, bytes, 0, 0);

  //2.set kernel
  cl::Kernel kernel(program_, "cubeScale", &err);
  kernel.setArg(0, buf_a);
  kernel.setArg(1, buf_b);
  kernel.setArg(2, N);

  //3.define CPU buffer memory
  std::vector<int> va(len);
  std::vector<int> vb(len);

  for (int i = 0; i < N; i++) {
    va[i] = 1;
  }

  //4.define event and queue
  cl::Event event;
  cl::CommandQueue queue(context, devices[0], 0, &err);

  //5.copy data from CPU to GPU
  queue.enqueueWriteBuffer(buf_a, false, 0, bytes, &va[0]);

  //6.run kernel
  queue.enqueueNDRangeKernel(
    kernel,
    cl::NullRange,
    cl::NDRange(N,N,N),
    cl::NullRange,
    NULL,
    &event);

  //7.read result from GPU
  queue.enqueueReadBuffer(buf_b, false, 0, bytes, &vb[0], NULL, &event);
  event.wait();

  //8.compare result
  bool isSame = true;
  for (int i = 0; i < len; i++) {
    if (va[i]*2!= vb[i]) {
      cout << "different at i=" << i << ", va[i]*2=" << va[i] * 2 << ", vb=" << vb[i] << endl;
      isSame = false;
      //break;
    }
  }
  cout << "isSame=" << isSame << endl;
}
void Test_vecAdd(cl::Context context, cl::Program program_, cl_int& err, std::vector<cl::Device>& devices) {
  const int N = 1024;
  size_t bytes = N * sizeof(double);

  //1.define GPU buffer memory
  cl::Buffer buf_a(context, CL_MEM_READ_WRITE, bytes, 0, 0);
  cl::Buffer buf_b(context, CL_MEM_READ_WRITE, bytes, 0, 0);
  cl::Buffer buf_c(context, CL_MEM_READ_WRITE, bytes, 0, 0);

  //2.set kernel
  cl::Kernel kernel(program_, "vecAdd", &err);
  kernel.setArg(0, buf_a);
  kernel.setArg(1, buf_b);
  kernel.setArg(2, buf_c);
  kernel.setArg(3, N);

  //3.define CPU buffer memory
  std::vector<double> va(N);
  std::vector<double> vb(N);
  std::vector<double> vc(N);
  for (int i = 0; i < N; i++) {
    va[i] = 1;
    vb[i] = 2;
  }

  //4.define event and queue
  cl::Event event;
  cl::CommandQueue queue(context, devices[0], 0, &err);

  //5.copy data from CPU to GPU
  queue.enqueueWriteBuffer(buf_a, false, 0, bytes, &va[0]);
  queue.enqueueWriteBuffer(buf_b, false, 0, bytes, &vb[0]);

  //6.run kernel
  queue.enqueueNDRangeKernel(
    kernel,
    cl::NullRange,
    cl::NDRange(N),
    cl::NullRange,
    NULL,
    &event);

  //7.read result from GPU
  queue.enqueueReadBuffer(buf_c, false, 0, bytes, &vc[0], NULL, &event);
  event.wait();

  //8.compare result
  bool isSame = true;
  for (int i = 0; i < N; i++) {
    if (va[i] + vb[i] != vc[i]) {
      cout << "different at i=" << i << ", va+vb=" << va[i] + vb[i] << ", vc=" << vc[i] << endl;
      isSame = false;
      break;
    }
  }
  cout << "isSame=" << isSame << endl;
}

int
main(void) {
  cl_int err = CL_SUCCESS;
  try {
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    cl::STRING_CLASS str;

    int platformId = 0;
    for (int i = 0; i < platforms.size(); i++) {
      platforms[i].getInfo(CL_PLATFORM_VENDOR, &str);
      cout <<i<<" :"<< str << endl;
      if (str.find("NVIDIA")!=str.npos) {
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
    source.push_back(std::make_pair(vecAdd, strlen(vecAdd)));

    cl::Program program_ = cl::Program(context, source);
    program_.build(devices);

    //execute kernel.
    {
      //Test_vecAdd(context, program_, err, devices);
      Test_cubeScale(context, program_, err, devices);
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


