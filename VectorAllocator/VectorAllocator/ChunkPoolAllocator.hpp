#ifndef CHUNK_POOL_ALLOCATOR_H_
#define CHUNK_POOL_ALLOCATOR_H_

#include <cstddef>
#include <memory>

//namespace d3d {

template<typename _Ty, unsigned int FirstBlockSize = 32, unsigned int NextBlockSize = 512>
class ChunkPoolAllocator;

//! Memory pool for elements with fixed size providing allocate and deallocate
/**
 * @param _Ty Type of the structure to be returned for each call to allocate(1)
 * @param FirstBlockSize size of the first block
 * @param NextBlockSize size of ths following blocks
 */
template<typename _Ty, unsigned int FirstBlockSize, unsigned int NextBlockSize>
class ChunkPoolAllocator {
 public:
  typedef _Ty value_type;
  typedef value_type *pointer;
  typedef const value_type *const_pointer;
  typedef value_type &reference;
  typedef const value_type &const_reference;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;

  template<typename U>
  struct rebind {
    typedef ChunkPoolAllocator<U, FirstBlockSize, NextBlockSize> other;
  };
 private:
  typedef struct tagChunkNode {
    tagChunkNode *next;
  } ChunkNode, ElementNode;

  void init() {
    my_free_list = 0;
    chunklist = 0;
    nbChunkOccupied = 0;
    nElementSize = sizeof(_Ty) > sizeof(void *) ? sizeof(_Ty) : sizeof(void *);
  }
  ElementNode *getElementByStep(ElementNode *node, size_type step) {
    for (size_type i = 1; i < step; i++) {
      if (!node) return NULL;
      node = node->next;
    }
    return node;
  }
  ElementNode *getNextElementAddress(ElementNode *node) {
    return (ElementNode *)((char *)node + nElementSize);
  }
 public:
  //! Constructor
  ChunkPoolAllocator() { init(); }
  //! free all chunklist nodes
  ~ChunkPoolAllocator() { clear(); }

  ChunkPoolAllocator(ChunkPoolAllocator const &) { init(); }

  template<typename U>
  ChunkPoolAllocator(ChunkPoolAllocator<U, FirstBlockSize, NextBlockSize> const &) {
    init();
  }

  //! Free all blocks
  void clear() {
    while (chunklist) {
      ChunkNode *p = chunklist;
      chunklist = chunklist->next;
      free(p);
    }
    init();
  }

  pointer address(reference r) { return &r; }

  const_pointer address(const_reference r) { return &r; }

  //! allocate elements memory
  /**
   *if my_free_list is not null malloc a chunk,fill my_free_list
   * return the first element of of my_free_list
   * @param elementCount Element Count to be allocate, general is 1 for list<> allocator
   * @param const_pointer,
   */
  pointer allocate( size_type elementCount, typename std::allocator<void>::const_pointer = 0 ) {
    printf("allocate %d\n", elementCount);
    return (pointer)malloc(elementCount * sizeof(_Ty));//as std::allocator implementation

    ElementNode *pLastStepElement = getElementByStep(my_free_list, elementCount);
    //if my_free_list is empty, malloc a chunk and refill my_free_list
    if (NULL == pLastStepElement) {
      //first chunk contain FirstBlockSize elements,others chunk contain NextBlockSize elements
      unsigned int nBlockElementCount = chunklist ? NextBlockSize : FirstBlockSize;
      if (nBlockElementCount < elementCount) throw;

      //malloc a chunk, add to the front of chunk list
      ChunkNode *chunk = (ChunkNode *)malloc(sizeof(ChunkNode) + nBlockElementCount * nElementSize);
      if (!chunk) return (pointer)0; //malloc fail !!!
      updateChunklist(chunk);
      fillFreelist(chunk, nBlockElementCount);
      //move to the last
      pLastStepElement = getElementByStep(my_free_list, elementCount);
    }

    nbChunkOccupied += elementCount;
    pointer ret = (pointer)my_free_list;
    my_free_list = pLastStepElement->next;
    return ret;
  }

  void updateChunklist(ChunkNode *chunk) {
    chunk->next = chunklist;
    chunklist = chunk;
  }

  void fillFreelist(ChunkNode *chunk, unsigned int nBlockElementCount) {
    my_free_list = (ElementNode *)(chunk + 1);
    ElementNode *curElement = my_free_list;
    for (int i = 1; i < nBlockElementCount; i++) {
      curElement->next = getNextElementAddress(curElement);
      curElement = curElement->next;
    }
    curElement->next = NULL;
  }



  //! deallocate element memory,insert at the front of my_free_list
  /**
   * @param p element pointer
   * @param size,elements node count to deallocate,usually equal to 1
   */
  inline void deallocate( pointer p, size_type size ) {
    printf("~deallocate %d\n\n", size);
    free( p );//as std::allocator implementation
    //return;
    //inert p at the front of my_free_list.
    ElementNode *q = (ElementNode *)p;
    for (int i = 0; i < (int)(size) - 1; i++) {
      q->next = getNextElementAddress(q);
      q = q->next;
    }
    q->next = my_free_list;
    my_free_list = (ElementNode *)p;
    nbChunkOccupied -= size;
  }

  size_t max_size() const {
    // estimate maximum array size
    size_t _Count = (size_t)(-1) / nElementSize;
    return (0 < _Count ? _Count : 1);
  }
  void construct(pointer p, const _Ty &t) { new(p) _Ty(t); }
  void destroy(pointer p) { p->~_Ty(); }
  bool operator==(ChunkPoolAllocator const &a) const { return &a == this;}
  bool operator!=(ChunkPoolAllocator const &a) const { return &a != this;}

  //! Return index if the chunk. -1 if not from this pool.
  int index(const_pointer pos) const {
    int index = 0;

    ChunkNode *chunknode = chunklist;
    char *target = (char *) pos;
    while (chunknode) {
      char *block = (char *)chunknode;
      chunknode = chunknode->next;

      unsigned int element_count = !chunknode ? FirstBlockSize : NextBlockSize;
      size_t blocksize = element_count * nElementSize;

      if (target >= block && target < block + blocksize) {
        index += (target - block) / nElementSize;
        return index;
      }
      index += element_count;
    }

    return -1;
  }

  int GetNumChunkOccupied() const {
    return nbChunkOccupied;
  }

 private:
  ChunkNode *chunklist;
  ElementNode *my_free_list;
  int nbChunkOccupied;
  size_t nElementSize;//to support x64,minimum elememtSize is sizeof(void*)=8 byte
};

//} //namespace
#endif