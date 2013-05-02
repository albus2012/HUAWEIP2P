#ifndef HUAWEIP2P_TIMER_H
#define HUAWEIP2P_TIMER_H

#include<boost/noncopyable.hpp>

#include"Atomic.h"
#include"Timestamp.h"
#include"Callbacks.h"
using namespace huaweip2p;

namespace huaweip2p
{

///
///Internal class for timer event
class Timer: boost::noncopyable
{
 public:
  Timer(const TimerCallback& cb, Timestamp when, double interval)
    : callback_(cb),
      expiration_(when),
      interval_(interval),
      repeat_(interval > 0.0),
      sequence_(s_numCreated_.incrementAndGet())
  {
  }
  
  void run() const { callback_(); }

  Timestamp expiration() const { return expiration_; }
  bool repeat() const { return repeat_; }
  int64_t sequence() const { return sequence_; }
  void restart(Timestamp now)
  {
    if(repeat_)
    {
      //FIXME, NO addTime()
      //expiration_ = addTime(now, interval_);
      ;
    }
    else
      expiration_ = Timestamp::invalid();
  }

  static int64_t numCreated() { return s_numCreated_.get(); }

 private:
  const TimerCallback callback_;
  Timestamp expiration_;
  const double interval_;
  const bool repeat_;
  const int64_t sequence_;

  static AtomicInt64 s_numCreated_;
};

AtomicInt64 Timer::s_numCreated_;

}
#endif 
