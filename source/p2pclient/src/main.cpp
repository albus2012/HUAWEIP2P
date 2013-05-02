#include <codec.h>
#include <p2pclient.h>
#include <Mutex.h>
#include <EventLoopThread.h>
#include <TcpClient.h>
#include <TcpServer.h>

#include <boost/bind.hpp>
#include <EventLoop.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
using namespace huaweip2p;
using namespace std;

//void runclient(const char* addr, const char* port)
//{
//  
//  EventLoopThread clientloop;
//  uint16_t clientport = static_cast<uint16_t>(atoi(port));
//  InetAddress destAddr(addr, clientport);
//  P2PClient client(clientloop.startLoop(), destAddr);
//  client.connect();
//  sleep(3);
//  //client.write(line);
//}
  
    
int main(int argc, char* argv[])
{
  std::cout << "pid = " << getpid() << endl;
//  ifstream configfile("config.txt",ifstream::in);
//  string line;
//  getline(configfile, line);
//  while( line.substr(0,12) != "Server port:")
//  {
//    getline(configfile, line);
//    if(configfile.eof())
//    {
//      cout << "config.txt wrong";
//      //return 1;
//    }
//  }
//  configfile.close();
  //uint16_t port = static_cast<uint16_t>(atoi(line.substr(12, 5).c_str()));

  if (argc > 2)
  {
    EventLoop loop; 
    //client address port
    uint16_t cserverport = static_cast<uint16_t>(atoi(argv[1]));
    InetAddress cserverAddr(cserverport);
    P2PCServer cserver(&loop, cserverAddr);
    P2PCServer::setIP((string)argv[1]);
    
    //server address port
    uint16_t port = static_cast<uint16_t>(atoi(argv[2]));
    InetAddress serverAddr("127.0.0.1", port);
    P2PClient client(&loop, serverAddr);
    
    client.connect();
    cserver.start();
    loop.loop();
  }
  else
  {
    printf("Usage: %s client_ip host_ip \n", argv[0]);
  }
}

