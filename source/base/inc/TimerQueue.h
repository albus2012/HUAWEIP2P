#ifndef HUAWEIP2P_TIMERQUEUE_H
#define HUAWEIP2P_TIMERQUEUE_H

#include<set>
#include<vector>
#include<string>
#include<boost/noncopyable.hpp>

#include"Mutex.h"
#include"Timestamp.h"
#include"Callbacks.h"
#include"Channel.h"
using std::string;
namespace huaweip2p
{

class EventLoop;
class Timer;
class TimerId;

///
/// A best efforts timer queue
///no guarantee that callback will be on time
///
class TimerQueue: boost::noncopyable
{
 public:
  TimerQueue(EventLoop* loop);
  ~TimerQueue();

///
///schedules the callback to be run at given time
///repeats if @c interval > 0.0
///
///must be thread safe. Usually be called from other threads
TimerId addTimer(const TimerCallback& cb,
                 Timestamp when,
                 double interval);
void cancel(TimerId timerId);

 private:

  typedef std::pair<Timestamp, Timer*> Entry;
  typedef std::set<Entry> TimerList;
  typedef std::pair<Timer*, int64_t> ActiveTimer;
  typedef std::set<ActiveTimer> ActiveTimerSet;

  void addTimerInLoop(Timer* timer);
  void cancelInLoop(TimerId timerId);
  //called when timerfd alarms
  void handleRead();
  //move out all expired timers
  std::vector<Entry> getExpired(Timestamp now);
  void reset(const std::vector<Entry>& expired, Timestamp now);

  bool insert(Timer* timer);

  EventLoop* loop_;
  const int timerfd_;
  Channel timerfdChannel_;
  //Timer list sorted by expiration
  TimerList timers_;

  //for cancel()
  ActiveTimerSet activeTimers_;
  bool callingExpiredTimers_;
  ActiveTimerSet cancelingTimers_;
};
}
#endif //
