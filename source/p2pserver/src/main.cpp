#include "p2pserver.h"

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
       
int main(int argc, char* argv[])
{
  cout << "pid = " << getpid() << endl;
  if (argc > 1)
  {
    EventLoop loop;
    uint16_t port = static_cast<uint16_t>(atoi(argv[1]));
    InetAddress serverAddr(port);
    P2PServer server(&loop, serverAddr);
    server.start();
    loop.loop();
  }
  else
  {
    printf("Usage: %s port\n", argv[0]);
  }
}

