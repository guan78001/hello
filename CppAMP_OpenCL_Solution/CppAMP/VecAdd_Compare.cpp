#include <iostream>
#include <vector>
#include <string>
#include <omp.h>
#include "Opencl_Instance.h"

struct Timer {
  Timer(const std::string &info): m_info(info) {
    m_t0 = omp_get_wtime();
  }
  ~Timer() {

    printf("%s %f ms\n", m_info.c_str(), GetStepTime());
  }
  double GetStepTime() {
    double t1 = omp_get_wtime();
    return (t1 - m_t0) * 1000;
  }
  double m_t0;
  std::string m_info;
};
int nTestCount = 100;
void StandardMethod(const std::vector<float> &vec_a, const std::vector<float> &vec_b) {
  Timer timer("CPU-Ref");
  const int size = vec_a.size();
  std::vector<float> sum(size);
  {
    for (int testCount = 0; testCount < nTestCount; testCount++) {
      for (int idx = 0; idx < size; idx++) {
        sum[idx] += vec_a[idx] * vec_b[idx];
      }
    }
  }
  double totalsum = 0;
  for (int idx = 0; idx < size; idx++) {
    totalsum += sum[idx];
  }
  printf("totalsum=%f\n", totalsum);
}

void OpenMPMethod(const std::vector<float> &vec_a, const std::vector<float> &vec_b) {
  Timer timer("OpenMPMethod");
  //#pragma omp parallel for
  //for (int i = 0; i < omp_get_num_procs(); i++) {
  //  printf("i=%d,thread=%d\n", i, omp_get_thread_num());
  //}
  const int size = vec_a.size();
  std::vector<float> sum(size);
  {
    for (int testCount = 0; testCount < nTestCount; testCount++) {

      #pragma omp parallel for schedule(static,16)
      for (int idx = 0; idx < size; idx++) {
        sum[idx] = vec_a[idx] * vec_b[idx];
      }
    }
  }
  double totalsum = 0;
  for (int idx = 0; idx < size; idx++) {
    totalsum += sum[idx];
  }
  printf("totalsum=%f\n", totalsum);
}

#include <algorithm>
#include <iostream>
#include <limits>
#include <random>
#include <vector>

static std::vector<float> generate_data(size_t size) {
  using value_type = int;
  // We use static in order to instantiate the random engine
  // and the distribution once only.
  // It may provoke some thread-safety issues.
  static std::uniform_int_distribution<value_type> distribution(
    std::numeric_limits<value_type>::min(),
    std::numeric_limits<value_type>::max());
  static std::default_random_engine generator;

  std::vector<float> data(size);
  std::generate(data.begin(), data.end(), []() { return distribution(generator); });
  return data;
}

#include <amp.h>
#include <iostream>
using namespace concurrency;

void CppAmpMethod(std::vector<float> &vec_a, std::vector<float> &vec_b) {
  Timer timer("AMP");
  const int size = vec_a.size();
  std::vector<float> vec_sum(size);
  // Create C++ AMP objects.
  array_view<const float, 1> a(size, &vec_a[0]);
  array_view<const float, 1> b(size, &vec_b[0]);
  array_view<float, 1> sum(size, &vec_sum[0]);
  //sum.discard_data();
  {
    Timer timer("CppAmpMethod_runkernel");
    for (int testCount = 0; testCount < nTestCount; testCount++) {

      parallel_for_each(
        // Define the compute domain, which is the set of threads that are created.
        sum.extent,
        // Define the code to run on each thread on the accelerator.
      [ = ](index<1> idx) restrict(amp) {
        sum[idx] += a[idx] * b[idx];
      }
      );
      //sum.synchronize();
    }
  }

  double totalsum = 0;
  for (int idx = 0; idx < size; idx++) {
    totalsum += sum[idx];
  }
  printf("totalsum=%f\n", totalsum);
}
void CppAmpMethod2(const std::vector<float> &vec_a, const std::vector<float> &vec_b) {
  Timer timer("AMP2");
  const int size = vec_a.size();
  std::vector<float> vec_sum(size);
  // Create C++ AMP objects.
  array<float, 1> a(size, &vec_a[0]);
  array<float, 1> b(size, &vec_b[0]);
  //array_view<float, 1> sum(size, &vec_sum[0]);
  array<float, 1> sum(size, &vec_sum[0]);
  //sum.discard_data();
  {

    Timer timer("CppAmpMethod_runkernel2");
    for (int testCount = 0; testCount < nTestCount; testCount++) {

      parallel_for_each(
        // Define the compute domain, which is the set of threads that are created.
        sum.extent,
        // Define the code to run on each thread on the accelerator.
      [ =, &sum, &a, &b](index<1> idx) restrict(amp) {
        sum[idx] += a[idx] * b[idx];
      }
      );
      //sum.synchronize();
    }
  }
  std::vector<float> result = sum;
  double totalsum = 0;
  for (int idx = 0; idx < size; idx++) {
    totalsum += result[idx];
  }
  printf("totalsum=%f\n", totalsum);
}
#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>
const char *helloStr =
  "__kernel void "
  "hello() "
  "{ "
  "  "
  "} "
  "__kernel void "
  "add(__global float *a,__global float *b,__global float *c)"
  "{"
  "size_t gid=get_global_id(0);"
  "c[gid]=a[gid]*b[gid];"
  "}"
  ;

int OpenCLMethod_C(const std::vector<float> &va, const std::vector<float> &vb) {
  Timer timer("OpenCLMethod_C");
  Opencl_Instance opencl_inst;
  const char *str_kernel[] = { "hello", "add" };
  opencl_inst.BuildProgram(helloStr, str_kernel, 2);

  size_t global_worksize[1] = { va.size() };
  //size_t local_worksize[1] = { 128 };
  size_t *local_worksize = NULL;

  int Len = va.size();
  std::vector<float> vc(Len);//result vc=va+vb
  int data_len = va.size() * sizeof(float);

  cl_mem mem_A = clCreateBuffer(opencl_inst.GetContext(), CL_MEM_READ_ONLY, data_len, NULL, NULL);
  cl_mem mem_B = clCreateBuffer(opencl_inst.GetContext(), CL_MEM_READ_ONLY, data_len, NULL, NULL);
  cl_mem mem_C = clCreateBuffer(opencl_inst.GetContext(), CL_MEM_READ_WRITE, data_len, NULL, NULL);

  clEnqueueWriteBuffer(opencl_inst.GetCommandQueue(), mem_A, CL_FALSE, 0, data_len, &va[0], NULL, NULL, NULL);
  clEnqueueWriteBuffer(opencl_inst.GetCommandQueue(), mem_B, CL_FALSE, 0, data_len, &vb[0], NULL, NULL, NULL);

  Opencl_Instance::KernelArgs args[] = {
    { sizeof(mem_A), (void *) &mem_A },
    { sizeof(mem_B), (void *) &mem_B },
    { sizeof(mem_C), (void *) &mem_C },
  };
  for (int test = 0; test < nTestCount; test++) {
    Timer timer("OpenCLMethod_C_runkernel");
    opencl_inst.runKernel(opencl_inst.GetKernel(1), args, sizeof(args) / sizeof(args[0]), 1, global_worksize, local_worksize);
  }
  clEnqueueReadBuffer(opencl_inst.GetCommandQueue(), mem_C, CL_TRUE, 0, data_len, &vc[0], NULL, NULL, NULL);
  clFinish(opencl_inst.GetCommandQueue());

  double totalsum = 0;
  for (int idx = 0; idx < Len; idx++) {
    totalsum += vc[idx];
  }
  printf("\ntotalsum=%f\n", totalsum);

  clReleaseMemObject(mem_A);
  clReleaseMemObject(mem_B);
  clReleaseMemObject(mem_C);
  return 0;
}
int OpenCLMethod(const std::vector<float> &va, const std::vector<float> &vb) {
  Timer timer("OpenCL");
  cl_int err = CL_SUCCESS;
  try {
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    if (platforms.size() == 0) {
      std::cout << "Platform size 0\n";
      return -1;
    }

    cl_context_properties properties[] =
    { CL_CONTEXT_PLATFORM, (cl_context_properties)(platforms[0])(), 0 };
    cl::Context context(CL_DEVICE_TYPE_GPU, properties);

    std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();

    cl::Program::Sources source(1,
                                std::make_pair(helloStr, strlen(helloStr)));
    cl::Program program_ = cl::Program(context, source);
    program_.build(devices);

    //cl::Kernel kernel(program_, "hello", &err);
    cl::Kernel kernel_add(program_, "add", &err);

    cl::Event event;
    cl::CommandQueue queue(context, devices[0], 0, &err);
    //queue.enqueueNDRangeKernel(
    //  kernel,
    //  cl::NullRange,
    //  cl::NDRange(4, 4),
    //  cl::NullRange,
    //  NULL,
    //  &event);

    const int Len = va.size();
    //std::vector<float> va(Len);
    //std::vector<float> vb(Len);
    std::vector<float> vc(Len);
    int data_len = Len * sizeof(float);

    //for (int i = 0; i < Len; i++) {
    //  va[i] = 1;
    //  vb[i] = 1;
    //}
    cl::Buffer buffer_a(context, CL_MEM_READ_ONLY, data_len);
    cl::Buffer buffer_b(context, CL_MEM_READ_ONLY, data_len);
    cl::Buffer buffer_c(context, CL_MEM_READ_WRITE, data_len);

    queue.enqueueWriteBuffer(buffer_a, CL_FALSE, 0, data_len, &va[0]);
    queue.enqueueWriteBuffer(buffer_b, CL_FALSE, 0, data_len, &vb[0]);
    queue.enqueueWriteBuffer(buffer_c, CL_FALSE, 0, data_len, &vc[0]);


    kernel_add.setArg(0, buffer_a);
    kernel_add.setArg(1, buffer_b);
    kernel_add.setArg(2, buffer_c);

    std::cout << "nTestCount=" << nTestCount << std::endl;
    for (int test = 0; test < nTestCount; test++) {
      Timer timer("OpenCLMethod_runkernel");
      queue.enqueueNDRangeKernel(
        kernel_add,
        cl::NullRange,
        cl::NDRange(va.size()),
        cl::NullRange,
        NULL,
        &event);
      queue.flush();
      //event.wait();
    }
    //event.wait();
    queue.enqueueReadBuffer(buffer_c, CL_TRUE, 0, data_len, &vc[0]);
    //queue.finish();

    double totalsum = 0;
    for (int idx = 0; idx < Len; idx++) {
      totalsum += vc[idx];
    }
    printf("totalsum=%f\n", totalsum);
  } catch (cl::Error &err) {
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

#include <iostream>
#include <vector>
#include <CL/cl.hpp>

int OpenCLWraperest() {

  // Get the platforms
  std::vector<cl::Platform> platforms;
  cl::Platform::get(&platforms);

  // Loop over the number of platforms
  for (size_t i = 0; i < platforms.size(); ++i) {

    // Display the platform information
    std::cout << "Platform " << i + 1 << ": "
              << platforms[i].getInfo<CL_PLATFORM_NAME>()
              << "\n----------------------------------------------"
              << "\nVendor    : " << platforms[i].getInfo<CL_PLATFORM_VENDOR>()
              << "\nVersion   : " << platforms[i].getInfo<CL_PLATFORM_VERSION>();

    // Get the devices on the current platform
    std::vector <cl::Device> devices;
    platforms[i].getDevices(CL_DEVICE_TYPE_ALL, &devices);

    // Loop over the devices
    std::cout << "\n----------------------------------------------\n";
    for (size_t j = 0; j < devices.size(); ++j) {

      // Display the device information
      std::cout
          << "\n   Device " << j + 1 << ": "
          << devices[j].getInfo< CL_DEVICE_NAME >()
          << "\n\t Device Version     : "
          << devices[j].getInfo< CL_DEVICE_VERSION >()
          << "\n\t OpenCL C Version   : "
          << devices[j].getInfo< CL_DEVICE_OPENCL_C_VERSION >()
          << "\n\t Compute Units      : "
          << devices[j].getInfo< CL_DEVICE_MAX_COMPUTE_UNITS >()
          << "\n\t Max Work Group Size: "
          << devices[j].getInfo< CL_DEVICE_MAX_WORK_GROUP_SIZE >()
          << "\n\t Clock Frequency    : "
          << devices[j].getInfo< CL_DEVICE_MAX_CLOCK_FREQUENCY >()
          << "\n\t Local Memory Size  : "
          << devices[j].getInfo< CL_DEVICE_LOCAL_MEM_SIZE >()
          << "\n\t Global Memory Size : "
          << devices[j].getInfo< CL_DEVICE_GLOBAL_MEM_SIZE >();

      // Check if the device supports double precision
      std::string str = devices[j].getInfo<CL_DEVICE_EXTENSIONS>();
      size_t found = str.find("cl_khr_fp64");
      std::cout << "\n\t Double Precision   : ";
      if (found != std::string::npos) { std::cout << "yes\n"; }
      else { std::cout << "no\n"; }
    }
    std::cout << "\n----------------------------------------------\n";
  }
  //  std::cin.ignore();
  return 0;
}
int main(int argc, char *argv[]) {
  OpenCLWraperest();
  int size = 1024 * 1024;
  if (argc > 2) {
    size = atoi(argv[1]);
    nTestCount = atoi(argv[2]);
  }
  printf("vec_size=%d, nTestCount=%d\n", size, nTestCount);

  //{
  //  printf("pre compile\n");
  //  std::vector<float> a(1);
  //  std::vector<float> b(1);
  //  CppAmpMethod(a, b);

  //  std::cout << "\nOpenCLMethod\n";
  //  OpenCLMethod(a, b);
  //  printf("--------------\n");
  //}
  std::vector<float> a = generate_data(size);
  std::vector<float> b = generate_data(size);

  std::cout << "StandardMethod\n";
  StandardMethod(a, b);

  //std::cout << "\nOpenMPMethod\n";
  //OpenMPMethod(a, b);

  std::cout << "\nCppAmpMethod\n";
  CppAmpMethod(a, b);
  CppAmpMethod(a, b);

  std::cout << "\nCppAmpMethod2\n";
  CppAmpMethod2(a, b);
  CppAmpMethod2(a, b);
  //std::cout << "\nOpenCLMethod\n";
  //OpenCLMethod(a, b);
  //OpenCLMethod(a, b);
  std::cout << "\nOpenCLMethod_C\n";
  OpenCLMethod_C(a, b);
  OpenCLMethod_C(a, b);
  //a[0] = 0;

  //std::cout << "\nCppAmpMethod\n";
  //CppAmpMethod(a, b);

  //std::cout << "\nOpenCLMethod\n";
  //OpenCLMethod(a, b);
  return 0;
}