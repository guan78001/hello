
#define __CL_ENABLE_EXCEPTIONS

#if defined(__APPLE__) || defined(__MACOSX)
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif
#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;

const char*  helloStr = "__kernel void "
                        "hello(void) "
                        "{ "
                        "  "
                        "} ";
const char*  helloStr2 = "__kernel void "
                         "hello2(void) "
                         "{ "
                         "  "
                         "} ";


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

    cl::Program::Sources source(1,
                                std::make_pair(helloStr, strlen(helloStr)));
    source.push_back(std::make_pair(helloStr2, strlen(helloStr2)));

    cl::Program program_ = cl::Program(context, source);
    program_.build(devices);

    cl::Kernel kernel(program_, "hello", &err);

    cl::Event event;
    cl::CommandQueue queue(context, devices[0], 0, &err);
    queue.enqueueNDRangeKernel(
      kernel,
      cl::NullRange,
      cl::NDRange(4, 4),
      cl::NullRange,
      NULL,
      &event);

    event.wait();

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


