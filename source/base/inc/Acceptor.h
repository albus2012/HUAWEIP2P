#ifndef HUAWEIP2P_ACCEPTOR_H
#define HUAWEIP2P_ACCEPTOR_H

#include<boost/function.hpp>
#include<boost/noncopyable.hpp>

#include"Channel.h"
#include"Socket.h"

namespace huaweip2p
{


class EventLoop;
class InetAddress;

///
/// Acceptor of incoming TCP connections.
///
class Acceptor : boost::noncopyable
{
 public:
  typedef boost::function<void (int sockfd,
                                const InetAddress&)> NewConnectionCallback;

  Acceptor(EventLoop* loop, const InetAddress& listenAddr);
  ~Acceptor();

  void setNewConnectionCallback(const NewConnectionCallback& cb)
  { newConnectionCallback_ = cb; }

  bool listenning() const { return listenning_; }
  void listen();

 private:
  void handleRead();

  EventLoop* loop_;
  Socket acceptSocket_;
  Channel acceptChannel_;
  NewConnectionCallback newConnectionCallback_;
  bool listenning_;
  int idleFd_;
};


}

#endif  //HUAWEIP2P_ACCEPTOR_H
