
#ifndef HUAWEIP2P_POLLPOLLER_H
#define HUAWEIP2P_POLLPOLLER_H

#include"Poller.h"

#include<map>
#include<vector>
#include<poll.h>


namespace huaweip2p
{
typedef std::vector<Channel*> ChannelList;
///
/// IO Multiplexing with poll(2).
///
class PollPoller : public Poller
{
 public:

  PollPoller(EventLoop* loop);
  virtual ~PollPoller();

  virtual Timestamp poll(int timeoutMs, ChannelList* activeChannels);
  virtual void updateChannel(Channel* channel);
  virtual void removeChannel(Channel* channel);

 private:
  void fillActiveChannels(int numEvents,
                          ChannelList* activeChannels) const;

  typedef std::vector<struct pollfd> PollFdList;
  typedef std::map<int, Channel*> ChannelMap;
  
  PollFdList pollfds_;
  ChannelMap channels_;
};

Poller* Poller::newDefaultPoller(EventLoop* loop)
{
  return new PollPoller(loop);
}
}
#endif  // HUAWEIP2P_POLLPOLLER_H
