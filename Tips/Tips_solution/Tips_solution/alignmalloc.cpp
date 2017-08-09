#include <memory>
#include <stdlib.h>
void *aligned_malloc(size_t size, size_t align) {
  // Align enough for the data, the alignment padding, and room to store a pointer to the actual start of the memory
  void  *mem = malloc(size + align + sizeof(void *));
  // The position at which we could potentially start addressing
  char *amem = ((char *)mem) + sizeof(void *);
  // Add align-1 to the start of the address and then zero out at most of the first align-1 bits.
  amem = (char *)(((size_t)(((char *)amem) + (align - 1))) & ~(align - 1));
  // Pre-write the actual address
  ((void **)amem)[-1] = mem;
  return amem;
}


int main() {
  void *p = aligned_malloc(17, 8);
  free(((int **)p)[-1]);

}