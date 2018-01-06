#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#include <CL/opencl.h>

#define KERNEL_CODE(...) #__VA_ARGS__

const char *kernelSource = KERNEL_CODE(
__kernel void vecAdd(__global double *a, __global double *b, __global double *c, const unsigned int n) {
  //Get our global thread ID
  int id = get_global_id(0);

  //Make sure we do not go out of bounds
  if (id < n) c[id] = a[id] + b[id];
}

__kernel void upSample(__global double *src, __global double *dst, int width, int height) {
  int id = get_global_id(0);

  for (int i = 0; i < height; i++) {
    double val = src[width * i + id];
    dst[2 * width * i + id] = val;
    dst[2 * width * i + width + id] = val;
  }
}

__kernel void upSample2(__global double *src, __global double *dst, int width, int height) {
  int id = get_global_id(0);
  __local float temp[128];
  int lid = get_local_id(0);
  for (int i = 0; i < height; i++) {
    temp[lid] = src[width * i + id];
    barrier(CLK_LOCAL_MEM_FENCE);
    dst[2 * width * i + id] = temp[lid];
    dst[2 * width * i + width + id] = temp[lid];
  }
}
__kernel void downsample(__global double *src, __global double *dst, int width, int height) {
  int id = get_global_id(0);

  for (int i = 0; i < height; i++) {
    dst[width * i + id] = src[2 * width * i + id];
  }
}

                           );

// OpenCL kernel. Each work item takes care of one element of


int main(int argc, char *argv[]) {
  // Length of vectors
  unsigned int n = 512;

  // Host input vectors
  double *h_a;
  double *h_b;
  // Host output vector
  double *h_c;

  // Device input buffers
  cl_mem d_a;
  cl_mem d_b;
  // Device output buffer
  cl_mem d_c;
  cl_mem d_d;
  cl_platform_id cpPlatform;        // OpenCL platform
  cl_device_id device_id;           // device ID
  cl_context context;               // context
  cl_command_queue queue;           // command queue
  cl_program program;               // program
  cl_kernel kernel;                 // kernel

  // Size, in bytes, of each vector
  size_t bytes = n * sizeof(double);

  // Allocate memory for each vector on host
  h_a = (double *)malloc(bytes);
  h_b = (double *)malloc(bytes);
  h_c = (double *)malloc(bytes);

  // Initialize vectors on host
  int i;
  for (i = 0; i < n; i++) {
//   h_a[i] = sinf(i) * sinf(i);
//   h_b[i] = cosf(i) * cosf(i);
    h_a[i] = 2;
    h_b[i] = 1;
  }
  {
    double sum = 0;
    for (i = 0; i < n; i++) {
      h_c[i] = h_a[i] + h_b[i];
      sum += h_c[i];
    }

    printf("cpu final result: %f\n", sum / n);
  }
  size_t globalSize, localSize;
  cl_int err;

  // Number of work items in each local work group
  localSize = 32;

  // Number of total work items - localSize must be devisor
  globalSize = ceil(n / (float)localSize) * localSize;

  // Bind to platform
  err = clGetPlatformIDs(1, &cpPlatform, NULL);

  // Get ID for the device
  err = clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);

  // Create a context
  context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);

  // Create a command queue
  queue = clCreateCommandQueue(context, device_id, 0, &err);

  // Create the compute program from the source buffer
  program = clCreateProgramWithSource(context, 1,
                                      (const char **)& kernelSource, NULL, &err);

  // Build the program executable
  clBuildProgram(program, 0, NULL, NULL, NULL, NULL);

  // Create the compute kernel in the program we wish to run
  kernel = clCreateKernel(program, "vecAdd", &err);

  // Create the input and output arrays in device memory for our calculation
  d_a = clCreateBuffer(context, CL_MEM_READ_ONLY, bytes, NULL, NULL);
  d_b = clCreateBuffer(context, CL_MEM_READ_ONLY, bytes, NULL, NULL);
  d_c = clCreateBuffer(context, CL_MEM_WRITE_ONLY, bytes, NULL, NULL);
  d_d = clCreateBuffer(context, CL_MEM_WRITE_ONLY, bytes, NULL, NULL);
  {
    int w = 64;
    int h = 3;
    size_t bytes = w * h * sizeof(double);

    //input
    for (int j = 0; j < h; j++) {
      for (int i = 0; i < w; i++) {
        h_a[j * w + i] = j * w + i;
      }
    }

    printf("input:\n");
    for (int j = 0; j < h; j++) {
      for (int i = 0; i < w; i++) {
        printf("%g ", h_a[j * w + i]);
      }
      printf("\n");
    }
    cl_kernel kernel = clCreateKernel(program, "upSample", &err);
    clEnqueueWriteBuffer(queue, d_a, CL_TRUE, 0,
                         bytes, h_a, 0, NULL, NULL);

    clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_a);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_c);
    clSetKernelArg(kernel, 2, sizeof(int), &w);
    clSetKernelArg(kernel, 3, sizeof(int), &h);

    // Execute the kernel over the entire range of the data set
    size_t globalSize = w, localSize = 32;
    err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, &localSize,
                                 0, NULL, NULL);
    // Wait for the command queue to get serviced before reading back results
    clFinish(queue);

    // Read the results from the device
    clEnqueueReadBuffer(queue, d_c, CL_TRUE, 0,
                        2 * bytes, h_c, 0, NULL, NULL);

    printf("output_upsample:\n");
    for (int j = 0; j < 2 * h; j++) {
      for (int i = 0; i < w; i++) {
        printf("%g ", h_c[j * w + i]);
      }
      printf("\n");
    }
    clReleaseKernel(kernel);

    {
      //downsample
      kernel = clCreateKernel(program, "downsample", &err);
      clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_c);
      clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_d);
      clSetKernelArg(kernel, 2, sizeof(int), &w);
      clSetKernelArg(kernel, 3, sizeof(int), &h);

      // Execute the kernel over the entire range of the data set
      size_t globalSize = w, localSize = 32;
      err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, &localSize,
                                   0, NULL, NULL);
      // Wait for the command queue to get serviced before reading back results
      clFinish(queue);
      clReleaseKernel(kernel);
      // Read the results from the device
      clEnqueueReadBuffer(queue, d_d, CL_TRUE, 0,
                          bytes, h_c, 0, NULL, NULL);

      printf("output_downsample:\n");
      for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
          printf("%g ", h_c[j * w + i]);
        }
        printf("\n");
      }
    }
  }
  // Write our data set into the input array in device memory
  err = clEnqueueWriteBuffer(queue, d_a, CL_TRUE, 0,
                             bytes, h_a, 0, NULL, NULL);
  err |= clEnqueueWriteBuffer(queue, d_b, CL_TRUE, 0,
                              bytes, h_b, 0, NULL, NULL);

  // Set the arguments to our compute kernel
  err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_a);
  err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_b);
  err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &d_c);
  err |= clSetKernelArg(kernel, 3, sizeof(unsigned int), &n);

  // Execute the kernel over the entire range of the data set
  err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, &localSize,
                               0, NULL, NULL);

  // Wait for the command queue to get serviced before reading back results
  clFinish(queue);

  // Read the results from the device
  clEnqueueReadBuffer(queue, d_c, CL_TRUE, 0,
                      bytes, h_c, 0, NULL, NULL);

  //Sum up vector c and print result divided by n, this should equal 1 within error
  double sum = 0;
  for (i = 0; i < n; i++)
    sum += h_c[i];
  printf("final result: %f\n", sum / n);

  // release OpenCL resources
  clReleaseMemObject(d_a);
  clReleaseMemObject(d_b);
  clReleaseMemObject(d_c);
  clReleaseProgram(program);
  clReleaseKernel(kernel);
  clReleaseCommandQueue(queue);
  clReleaseContext(context);

  //release host memory
  free(h_a);
  free(h_b);
  free(h_c);

  return 0;
}