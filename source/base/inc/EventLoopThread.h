#ifndef HUAWEIP2P_EVENTLOOPTHREAD_H
#define HUAWEIP2P_EVENTLOOPTHREAD_H

#include"Condition.h"
#include"Mutex.h"
#include"Thread.h"

#include<boost/noncopyable.hpp>

namespace huaweip2p
{

class EventLoop;

class EventLoopThread : boost::noncopyable
{
 public:
  typedef boost::function<void(EventLoop*)> ThreadInitCallback;

  EventLoopThread(const ThreadInitCallback& cb = ThreadInitCallback());
  ~EventLoopThread();
  EventLoop* startLoop();

 private:
  void threadFunc();

  EventLoop* loop_;
  bool exiting_;
  Thread thread_;
  MutexLock mutex_;
  Condition cond_;
  ThreadInitCallback callback_;
};

}


#endif  // HUAWEIP2P_EVENTLOOPTHREAD_H

