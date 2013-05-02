#ifndef HUAWEIP2P_POLLER_H
#define HUAWEIP2P_POLLER_H

#include<vector>
#include<boost/noncopyable.hpp>

#include"Timestamp.h"
#include"EventLoop.h"

namespace huaweip2p
{

class Channel;

///Base class for IO multiplexing
///
///this class doesn't own the Channel objects

class Poller: boost::noncopyable
{
 public:
  typedef std::vector<Channel*> ChannelList;
    
  Poller(EventLoop* loop)
    : ownerLoop_(loop) { }
  virtual ~Poller() { }

  ///Polls the I/O events
  ///must be called in the loop thread
  virtual Timestamp poll(int timeoutMs, ChannelList* activeChannel) = 0;
 
  ///Changes the interested I/O events
  ///must be called in the loop thread
  virtual void updateChannel(Channel* channel) = 0;
  virtual void removeChannel(Channel* channel) = 0;

  static Poller* newDefaultPoller(EventLoop* loop);
  void assertInLoopThread()
  { ownerLoop_->assertInLoopThread(); }

 private:
  EventLoop* ownerLoop_;
};


}
#endif //HUAWEIP2P_POLLER_H  
