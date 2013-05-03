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

P2PServer::P2PServer(EventLoop* loop, const InetAddress& listenAddr)
  :loop_(loop), 
   seq_id(0),
   server_(loop, listenAddr, "P2PServer"),
   codec_(boost::bind(&P2PServer::onStringMessage, this, _1, _2, _3))
{
  catalogue_ = new UserCatalogue();
  server_.setConnectionCallback(
             boost::bind(&P2PServer::onConnection, this, _1));
  server_.setMessageCallback(
             boost::bind(&LengthHeaderCodec::onMessage, &codec_, _1, _2, _3));
  //server_.setThreadNum(4);
 }


void P2PServer::onConnection(const TcpConnectionPtr& conn)
{
  cout << conn->localAddress().toIpPort() << " -> "
       << conn->peerAddress().toIpPort() << " is "
       << (conn->connected() ? "UP" : "DOWN") 
       << endl;

}

void P2PServer::onStringMessage(const TcpConnectionPtr& conn,
                      const string& message,
                      Timestamp)
{
  char   type = message[0];
  int      id = message[1]; 
  string   ip = message.substr(2,4);
  int reqfile = message[6];
  //catalogue_->insertUser(ip, reqfile); 
    cout << endl 
       << "test" << catalogue_->hasNeighbor(ip) 
       << message 
       << endl;
  string ret;
  if(type == 'a')
  {
    catalogue_->insertUser(ip, reqfile);

    ret.append("b0");
    ret.append(processMessage(ip));
    cout << endl << ret << endl;
    codec_.send(conn, ret);
    conn->shutdown();
  }
  else if(type == 'c')
  {
    ret.append("d01111");
    ret.append(1, (char)100);
    cout << endl << "222test" << endl << ret;
    codec_.send(conn, ret);
  }
  else
  { 
    cout << "error 123" << endl;
  } 
}

string P2PServer::processMessage(string ip)
{
  string retMessage;
  if(catalogue_->hasNeighbor(ip))
  {
    //retMessage[0] = 'b';
    //retMessage[1] = char(seq_id);
    //seq_id = (seq_id == 255) ? 0 : seq_id + 1;
    //retMessage.append(retNeighborIP(ip));
    retMessage = retNeighborIP(ip);
  }  
  else
  {
    retMessage = processMyself(ip);
  }
  return retMessage;
  //codec_.send(conn, retMessage);
}
string P2PServer::processMyself(string ip)
{
  string ret("11111");
  ret[0] = char(1);
  return ret;
}
string P2PServer::retNeighborIP(string ip)
{
  set<UserInfo> neiset = catalogue_->getNeighborUsers(ip);
  string ret = "0";
  
  int num = 0;
  for(set<UserInfo>::iterator iter = neiset.begin();
      iter != neiset.end(); ++iter)
  {
    if(iter->userIP != ip)
    {
      ret.append(iter->userIP);
      ret.append(1,char(iter->userFile));
      num++;
    }
  }
  ret[0] = char(num);
  return ret;
}
    
