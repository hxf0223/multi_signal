#include "semaphore.h"
#include "thread_safe_cout.h"
#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iostream>
#include <mutex>
#include <ostream>
#include <stdio.h>
#include <string>
#include <thread>
#include <vector>

class groupSignals {
public:
  groupSignals(size_t signalNum) : cvs_(signalNum) {}
  groupSignals() = delete;
  groupSignals(const groupSignals &) = delete;
  groupSignals &operator=(const groupSignals &) = delete;
  groupSignals(groupSignals &&) = delete;
  groupSignals &operator=(groupSignals &&) = delete;

  void notifyAll() {
    for (size_t i = 0; i < cvs_.size(); i++) {
      cvs_.at(i).notify_all();
    }
  }

  void clientWait(size_t id) {
    if (id < cvs_.size()) {
      std::unique_lock<std::mutex> lk(m_);
      cvs_[id].wait(lk);
    }
  }

private:
  std::vector<std::condition_variable> cvs_;
  // std::condition_variable cv_;
  std::mutex m_;
};

int main(int argc, char *argv[]) {
  constexpr size_t thd_num = 4;
  groupSignals signal_sender(thd_num);
  std::semaphore init_sem(-(int32_t)thd_num);
  std::semaphore run_sem(-(int32_t)thd_num);
  using safe_cout = thread_safe_cout::cout;

  auto thread_proc = [&init_sem, &run_sem](groupSignals *waitSignal,
                                           size_t waitIndex,
                                           size_t loopNum) -> void {
    // auto tid = std::this_thread::get_id();
    safe_cout() << "enter thread " << waitIndex << std::endl;
    init_sem.release();

    for (size_t i = 0; i < loopNum; i++) {
      waitSignal->clientWait(waitIndex); // wait at start of loop
      safe_cout() << "wait done: " << waitIndex << ", " << i << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
      run_sem.release();
    }

    safe_cout() << "exit thread " << waitIndex << std::endl;
  };

  std::vector<std::thread> thds;
  constexpr size_t loop_num = 16;
  for (size_t i = 0; i < thd_num; i++) {
    thds.emplace_back(thread_proc, &signal_sender, i, loop_num);
  }

  init_sem.wait();
  for (size_t i = 0; i < loop_num; i++) {
    safe_cout() << "=====>> notify all: " << i << std::endl;
    signal_sender.notifyAll();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    run_sem.waitReset(-(int32_t)thd_num);
  }

  safe_cout() << "wait threads end." << std::endl;
  for (size_t i = 0; i < thd_num; i++) {
    thds.at(i).join();
  }

  return 0;
}
