#ifndef HUAWEIP2P_TIMERID_H
#define HUAWEIP2P_TIMERID_H
#include<stdint.h>
namespace huaweip2p
{

class Timer;

class TimerId
{
 public:
  TimerId()
    : timer_(0),sequence_(0) { }
  
  TimerId(Timer* timer, int64_t seq)
    : timer_(timer), sequence_(seq) { }

  friend class TimerQueue;
 
 private:
  Timer* timer_;
  int64_t sequence_;
};
}
#endif //HUAWEIP2P_TIMERID_H

