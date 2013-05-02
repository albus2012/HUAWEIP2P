#include "codec.h"
#include "usercatalogue.h"

#include <Mutex.h>
#include <EventLoop.h>
#include <TcpServer.h>

#include <boost/bind.hpp>
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <stdio.h>
#include <unistd.h>

using namespace std;
using namespace huaweip2p;

class P2PServer : boost::noncopyable
{
 public:
  P2PServer(EventLoop* loop, const InetAddress& listenAddr);
  void start()
  {
    server_.start();
  }

 private:
  void onConnection(const TcpConnectionPtr& conn);
  void onStringMessage(const TcpConnectionPtr& conn,
                       const string& message, Timestamp);

  string processMessage(string ip);
  string processMyself(string ip);
  string retNeighborIP(string ip);
    
  EventLoop* loop_;
  TcpServer server_;
  int seq_id;
  LengthHeaderCodec codec_;
  UserCatalogue* catalogue_;
};

