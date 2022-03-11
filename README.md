## 定制化的线程同步通知

1. 使用C++11中的condition_variable，实现
- 主线程等待多个后台线程到达同步点：class groupSignals；
- 后台线程在同一个同步点等待主线程发出信号（模拟windows的waitForMultipleEvents）：semaphore.h class semaphore;

2. 功能限制
在等待线程没有到达wait之前（condition_variable::wait）发出信号，等待线程本次接收不到；

参考实现：
- [Thread_Mutex_Semaphore](https://github.com/ayowin/Thread_Mutex_Semaphore)

