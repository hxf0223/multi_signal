#pragma once

#include <iostream>
#include <mutex>
#include <string>

namespace thread_safe_cout {
static std::mutex m_CoutMutex;
struct cout {
  std::unique_lock<std::mutex> m_Lock;
  cout() : m_Lock(std::unique_lock<std::mutex>(m_CoutMutex)) {}

  template <typename T> cout &operator<<(const T &message) {
    std::cout << message;
    return *this;
  }

  cout &operator<<(std::ostream &(*fp)(std::ostream &)) {
    std::cout << fp;
    return *this;
  }
};

} // namespace thread_safe_cout
