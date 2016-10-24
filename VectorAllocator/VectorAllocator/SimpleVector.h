#pragma once
#include <map>
#include <vector>
#include <assert.h>
#include <algorithm>

//#include "SimpleAllocator.hpp"
const int s_table_len = 8;
static int s_table[s_table_len] = { 1, 2, 4, 8, 16, 32, 64, 128 };
template<class T>
struct BuddyAllocator {
  enum { kMaxCount = 4,kFreeTableLen=8 };
  //T *m_freelist[8];
  std::vector<T *> m_blocks[kFreeTableLen];
  T *m_freelist[kFreeTableLen];
  BuddyAllocator() {
    Init();
  }
  ~BuddyAllocator() {
    for (int i = 0; i < kFreeTableLen; i++) {
      for (int j = 0; j < m_blocks[i].size(); j++) {
        free(m_blocks[i][j]);
      }
      m_blocks[i].clear();
    }
  }

  T *allocate(size_t n) {
    //if (n < 1) throw;
    int pos = index(n);
    if (pos < kFreeTableLen) {
      if (!m_freelist[pos]) {
        NewBlock(n, pos);
      }

      T *p = m_freelist[pos];
      m_freelist[pos] = nextof(p);

      return p;
    } else {
      return (T *)malloc(n*sizeof(T));
    }
  }
  void deallocate(T *p, size_t n) {
    if (!p) return;
    if (n < kFreeTableLen) {
      int pos = index(n);
      nextof(p)= m_freelist[pos];
      m_freelist[pos] = p;
    } else {
      free(p);
    }
  }

  static int index(int n) {
    int *pos = std::upper_bound(s_table, s_table + s_table_len, n - 1);
    return pos - s_table;
  }

  static void test() {
    //test index()
    assert(index(1)==0);
    assert(index(2) == 1);
    assert(index(3) == 2);
    assert(index(4) == 2);
    assert(index(5) == 3);
    assert(index(6) == 3);
    assert(index(7) == 3);
    assert(index(8) == 3);
    assert(index(9) == 4);
    assert(index(127) == 7);
    assert(index(128) == 7);
    assert(index(129) == 8);
    assert(index(512) == 8);

    BuddyAllocator ba;
    //test BuddyAllocator
    for (int i = 0; i < kFreeTableLen; i++) {
      T *p = ba.m_freelist[i];
      for (int t = 0; t < kMaxCount; t++) p = nextof(p);
      assert(p == nullptr);
    }

    //test allocator
    T *p = ba.m_freelist[0];
    vector<T *> arr;
    arr.resize(2*kMaxCount);
    int n = 2;
    arr[0] = ba.allocate(n);
    assert(ba.m_freelist[index(n)] == nextof(arr[0]));
    arr[1] = ba.allocate(n);
    assert(ba.m_freelist[index(n)] == nextof(arr[1]));
    assert(arr[1] == nextof(arr[0]));

    ba.deallocate(arr[0],n);
    assert(ba.m_freelist[index(n)] == arr[0]);
    ba.deallocate(arr[1],n);
    assert(ba.m_freelist[index(n)] == arr[1]);
  }
 private:

  static T *&nextof(T *p) {
    return *(T **)p;
  }
  void Init() {
    for (int i = 0; i < kFreeTableLen; i++) {
      int len = kMaxCount;
      NewBlock(len, i);
    }
  }

  void NewBlock(int len, int pos) {
    T *p = (T *)malloc(len * sizeof(T)*s_table[pos]);

    //make list.
    T *cur = p;
    size_t step = s_table[pos];
    for (int j = 0; j < len - 1; j++) {
      T *next = cur + step;
      nextof(cur) = next;
      cur = next;
    }
    nextof(cur) = nullptr;
    m_blocks[pos].push_back(p);
    m_freelist[pos] = p;
  }
};
template<class T>
class  SimpleVector {
 public: //iterator
  typedef       T *iterator;
  typedef const T *const_iterator;

  inline iterator begin() {
    return m_array;
  }
  inline const_iterator begin() const {
    return m_array;
  }

  inline iterator end() {
    return m_array + m_last;
  }
  inline const_iterator end() const {
    return m_array + m_last;
  }
  //iterator erase(iterator position)
  //{
  //	if(position>=begin() && position <end())
  //	{
  //		int pos=position-begin();
  //		for (size_t i=pos+1; i<endpos; i++)
  //		{
  //			array[i-1]=array[i];
  //		}
  //		endpos--;
  //	}
  //	return position;
  //}

 public:
  inline size_t size() const { return m_last; }
  inline size_t capacity()  const { return m_capacity;}
  bool empty() const { return 0 == m_last; }
  void clear() {
    Destruct(begin(), end());
    m_last = 0;
  }
  T operator[] (int i) const { /*checkRange(i, m_last);*/ return m_array[i]; }


  SimpleVector() :m_array(nullptr), m_last(0), m_capacity(0) {  }
  ~SimpleVector() {
    deallocate();
  }
  void operator=(const T &rhs) = delete;  //disallowed for security
  SimpleVector(const SimpleVector &) = delete; //disallowed for security

  //void reserve(T *p,size_t len);
  //void resize(T *p,size_t len, T c=T());
  void reserve(size_t len);
  void resize(size_t len, const T &c = T());
  T &operator[] (int i) { /*checkRange(i, m_last);*/ return m_array[i]; }

  void push_back(const T &val) {
    if (m_last == m_capacity) {
      size_t len = m_capacity;
      Grow(len);
      reserve(len);
    }
    Construct(m_array + m_last, val);
    m_last++;
    //insert(end(), val);//slow
  }
  iterator insert(const_iterator iter, const T &val) {
    size_t pos = iter - begin();
    if (size() == capacity()) {
      Grow(m_capacity);
      T *p = allocate(m_capacity);
      Copy(begin(), begin() + pos, p);
      Construct(p + pos, val);
      Copy(begin() + pos, end(), p + pos + 1);

      //deallocate old memory
      deallocate();

      m_last += 1;
      m_array = p;
    } else {
      if (iter==end()) {
        Construct(end(), val);
      } else {
        Copy_Backward(begin() + pos, end(), end() + 1);
        *(begin() + pos) = val;
      }
      m_last++;
    }
    return begin() + pos;
  }
 private:
  void Grow(size_t &capacity) {
    if (capacity == 0) {
      capacity= 1;
    } else {
      capacity *= 2;
    }
  }
  void Construct(T *p,const T &val) {
    //std::_Construct(p, val);
    new(p)T(val);
  }
  void Destruct(T *first, T *last) {
    for (T *p = first; p != last; ++p) {
      //std::_Destroy(p);
      p->~T();
    }
  }
  void Copy(T *first, T *last, T *d_last) {
    for (T *p = first; p != last; p++, d_last++) {
      Construct(d_last, *p);
    }
  }
  T *Copy_Backward(T *first, T *last, T *d_last) {
    while (first != last) {
      *(--d_last) = *(--last);
    }
    return d_last;
  }

  T *allocate(size_t len) {
    //T *p = (T *)malloc(len * sizeof(T));
    T *p=s_ba.allocate(len);
    return p;
  }
  void deallocate() {
    Destruct(begin(), end());
    //free(m_array);
    s_ba.deallocate(m_array, s_ba.index(size()));
  }
  void checkRange(size_t pos, size_t len) const {
#if 0
    if (pos >= len) {
      printf("FixedLenVector::checkRange failed,LINE=%d", __LINE__);
      throw ("FixedLenVector::checkRange failed");
    }
#else
#endif
  }
 private:
  T *m_array;
  size_t m_last;
  size_t m_capacity;
  static BuddyAllocator<T> s_ba;
};
BuddyAllocator<int> SimpleVector<int>::s_ba;

template<class T>
void SimpleVector<T>::resize(size_t len, const T &c) {
  if (len == m_last) return;

  if (len < m_last) {
    Destruct(begin() + len, end());
    m_last = len;
  }
  if (len > m_last) {
    if (len < m_capacity) {
      for (iterator it = end(); it < end() + len - m_last; it++) {
        Construct(it, c);
      }
    } else {
      size_t length_new = m_capacity;
      while (length_new < len) {
        Grow(length_new);
      }
      T *p = allocate(length_new);

      Copy(begin(), end(), p);
      std::uninitialized_fill_n(p + size(), len - size(), c);

      deallocate();
      m_last = len;
      m_array = p;
      m_capacity = length_new;
    }
  }
}

template<class T>
void SimpleVector<T>::reserve(size_t len ) {
  if (len <= m_capacity) return;

  T *p = allocate(len);
  Copy(begin(), end(), p);
  deallocate();

  m_array = p;
  m_capacity = len;
}
