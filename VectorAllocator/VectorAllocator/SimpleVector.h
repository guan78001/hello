#pragma once
#include "SimpleAllocator.hpp"



static int s_table[] = { 1, 2, 4, 8, 16, 32, 64, 128 };
int s_table_len = sizeof(s_table) / sizeof(s_table[0]);
template<class T>
struct BuddyAllocator {
  std::map<int, T *> m_freelist;
  BuddyAllocator()
  {}

  struct Node {
    Node *next;
  };
  void Init() {
    for (int i = 0; i < 8; i++) {
      int len = 1024;
      T *p = malloc(len * sizeof(T)*s_table[i]);

      //make list.
      T *cur = p;
      for (int i = 0; i < len-1; i++) {
        T *next = cur + s_table;
        *cur = next;
        cur = next;
      }
      *cur = 0;
    }
  }
  ~BuddyAllocator()
  {}
  T *allocate(size_t n) {
    auto it = m_freelist.find(n);
    if (it==m_freelist.end()) {
    }
  }
  void deallocate(T *p, size_t n) {

  }
  int lkup(int n) {

    auto it = std::lower_bound(table, table + 8, n);

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
    T *p = (T *)malloc(len * sizeof(T));
    return p;
  }
  void deallocate() {
    Destruct(begin(), end());
    free(m_array);
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
};

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
