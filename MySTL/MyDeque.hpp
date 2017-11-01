template <class T, int N = 64>
class MyDeque {
 public:
  struct Data {
    T data[N];
  };
  bool empty() const { return m_c.empty(); }
  void push_back(const T &val) {
    if (!m_iter_back || m_back_index == N) {
      m_c.push_back(Data());
      m_iter_back = &(m_c.back());
      m_back_index = 0;
      if (!m_iter_front) m_iter_front = m_iter_back;
    }
    m_iter_back->data[m_back_index++] = val;
  }
  void pop_front() {
    m_front_index++;
    if (m_front_index == N) {
      m_front_index = 0;
      m_c.pop_front();
      if (m_c.empty()) {
        m_iter_front = nullptr;
        m_iter_front = nullptr;
      } else {
        m_iter_front = &m_c.front();
      }
    }
  }
  T front() {
    return m_iter_front->data[m_front_index];
  }
 private:
  typedef std::list<Data> Container;
  Container m_c;
  Data *m_iter_front = nullptr;
  Data *m_iter_back = nullptr;
  int m_front_index = 0;
  int m_back_index = 0;
};