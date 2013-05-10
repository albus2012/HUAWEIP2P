#include <codec.h>
#include <p2pclient.h>
#include <Mutex.h>
#include <EventLoopThread.h>
#include <TcpClient.h>
#include <TcpServer.h>
#include <SocketsOps.h>

#include <boost/bind.hpp>
#include <EventLoop.h>
#include <errno.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
using namespace huaweip2p;
using namespace std;

//the local neighbor node info:<ip, updatafile>
map<string, int> P2PCServer::neiIP;

//Init the local updatafile
int P2PCServer::updateFile = 1;
string P2PCServer::IP;

void P2PCServer::setIP(string ip)
{
  IP = ip;
}

string P2PCServer::getIP()
{
  return IP;
}

//When there is no info of updatafile, just assume it's 101
void P2PCServer::insertNeiIP(string ip)
{
  P2PCServer::insertNeiIP(ip, 101);
}

void P2PCServer::insertNeiIP(string ip, int file)
{
  if(file == 1)
    file = 101;
  neiIP[ip] = file;
}

void P2PCServer::removeNeiIP(string ip)
{
  neiIP.erase(ip);
}

string P2PCServer::findNeiIP(int level)
{
  map<string, int>::iterator iter = neiIP.begin();
  for(; iter != neiIP.end(); iter++)
  {
    cout << endl << iter->first << endl;
    if(iter->second > level && iter->first != "1111")
      return iter->first;
  }
  //If there is no neighbor info, just return the server info
  return "1111";
}
  
  
int P2PCServer::getFile()
{
  return updateFile;
}

//If the source's > local's, then local's updatefile ++
//else return 0 
int P2PCServer::processUpdateFile(int level)
{
  //int temp = atoi(level.c_str()); 
  if(level > updateFile)
  {
    updateFile++;
    cout << endl << "updateFile" << updateFile << endl;
    return 1;
  }
  else
    return 0;
}
  
P2PClient::P2PClient(EventLoop* loop, const InetAddress& serverAddr)
    : loop_(loop),isFirst(1),
      client_(new TcpClient(loop, serverAddr, "P2PClient")),
      codec_(boost::bind(&P2PClient::onStringMessage, this, _1, _2, _3))
{
  client_->setConnectionCallback(
      boost::bind(&P2PClient::onConnection, this, _1));
  client_->setMessageCallback(
      boost::bind(&LengthHeaderCodec::onMessage, &codec_, _1, _2, _3));
  client_->enableRetry();
}

//Setup a new connection to newAddr
bool P2PClient::changeServer(const InetAddress& newAddr, EventLoop* loop)
{
  if(connection_)
    client_->disconnect();
  client_->stop();
  //delete client_;
  setState(kConnecting);
  //delete client_;  

  int sockfd = sockets::createOrDie();
  
  int ret = sockets::connect(sockfd, newAddr.getSockAddrInet());
  
  //Test whether the destination is working or not
  //If it is not working, return false
  //else setup a new connection
  if(ret < 0)
  {
    cout << "sockets::connect:" << errno << endl;
    return false;
  }
  else
  {
    cout << "ret ==0" <<ret << endl;
    sockets::close(sockfd);
 
  }
  delete client_;  
  client_ = new TcpClient(loop, newAddr, "P2PClient");
  
  client_->setConnectionCallback(
      boost::bind(&P2PClient::onConnection, this, _1));
  client_->setMessageCallback(
      boost::bind(&LengthHeaderCodec::onMessage, &codec_, _1, _2, _3));
  client_->enableRetry();
  connect();
  return true;
  

}
bool P2PClient::changeServer(const InetAddress& newAddr)
{
  return changeServer(newAddr, loop_);
}
  
void P2PClient::write(const StringPiece& message)
{
  //cout << endl << "write" << message.data() << endl;  
  if (connection_)
  {
    cout << endl << "p2pclient write" << endl;
    codec_.send(connection_, message);
  }
  else
    cout << endl << "Connection failed";
}

void P2PClient::onTestConnection(const TcpConnectionPtr& conn)
{
  cout << " onTestConnection" << endl;
}

//Callback funtion on the connection
void P2PClient::onConnection(const TcpConnectionPtr& conn)
{
  cout << conn->localAddress().toIpPort() << " -> "
       << conn->peerAddress().toIpPort() << " is "
       << (conn->connected() ? "UP" : "DOWN")
       << endl;

  if (conn->connected())
  {
    setState(kConnected);
    connection_ = conn;
    cout << " conn done" << endl;
    if(isFirst)
    {
      isFirst = 0;
      string mes("a0");
      mes.append(P2PCServer::getIP());
      mes.append(1, (char)1);
      write(mes);
    }
    else
    {
       string mes("c0");
       mes.append(P2PCServer::getIP());
       mes.append(1, (char)P2PCServer::getFile());
       write(mes);
    }
  }
  else
  {
    connection_.reset();
    cout << "is==" << (getState() == kConnecting) << endl;
    if(getState() == kConnected)
    {
      cout << "aaaaaaa" << endl;
      InetAddress dest(atoi(IP.c_str()));
      do{
        P2PCServer::removeNeiIP(IP);
        IP = P2PCServer::findNeiIP(P2PCServer::getFile());
        dest = InetAddress("127.0.0.1",atoi(IP.c_str()));
      }while(!changeServer(dest));
     
    }
  }
}
void P2PClient::onStringMessage(const TcpConnectionPtr&,
                       const string& message,
                       Timestamp)
{
  //printf("<<< %s\n", message.c_str());
  int res = processMessage(message);
  int upfile = P2PCServer::getFile();
  sleep(1);
  if(upfile < 100)
  {
    string mes = "c0";
    mes.append(P2PCServer::getIP());
    mes.append(1,(char)upfile);
    
    if(res) //still use the same conn
    {
      write(mes);
    }
    else // change the conn
    {
      IP = P2PCServer::findNeiIP(upfile);
      InetAddress dest = InetAddress("127.0.0.1", atoi(IP.c_str()));
      while(!changeServer(dest))
      {
        P2PCServer::removeNeiIP(IP); 
        IP = P2PCServer::findNeiIP(upfile);
        dest = InetAddress("127.0.0.1",atoi(IP.c_str()));
      }
      //write(mes);
      cout << "change conn" <<endl << mes <<  endl;
    }
  }
  else
    cout << endl << " Update File Done !! " << endl;
      
    
  //int type = atoi(message.substr(0,1).c_str());
  //int id   = atoi(message.substr(1,1).c_str()); 
  //if(message[0] == 'b')
  //{
  //  int num = message[2] - '0';
  //  for(int i = 0; i < num; i++)
  //  {
  //    P2PCServer::insertNeiIP(message.substr(3+4*i,4));
  //  }
  //}
  //client_.disconnect();
}

int P2PClient::processMessage(string message)
{
  char type = message[0];
  int  id   = message[1];
  cout << endl << "client processmes mes: " << message << endl;
  if(type == 'b')//message from the server
  {
    //message format: type id num IP IP IP...
    int num = message[2];//the number of neiIP
    for(int i = 0; i < num; i++)
    {
      //store the IP, which is neiIP(4chars) and file(1char)
      P2PCServer::insertNeiIP(message.substr(3+i*5, 4), (message[7+i*5]));
    }
    cout << "bbbbbbbbb" << endl;
    return 0;
  }
  else if(type == 'd')//message from other clients
  {
    //message format: type id IP level
    P2PCServer::insertNeiIP(message.substr(2,4), (int)message[6]);

    int res = P2PCServer::processUpdateFile(message[6]);
    cout << "client :: type :: d :: " << res << endl;
    return res ? 1 : 0;
  }
  else//error unknown
  {
    cout << "error" << endl;
    return -1;
  }
}
P2PCServer::P2PCServer(EventLoop* loop,
             const InetAddress& listenAddr)
  : loop_(loop), seq_id(0),
    server_(loop, listenAddr, "P2PCServer"),
    codec_(boost::bind(&P2PCServer::onStringMessage, this, _1, _2, _3))
{
  server_.setConnectionCallback(
      boost::bind(&P2PCServer::onConnection, this, _1));
  server_.setMessageCallback(
      boost::bind(&LengthHeaderCodec::onMessage, &codec_, _1, _2, _3));
  server_.setThreadNum(4);
}

void P2PCServer::onConnection(const TcpConnectionPtr& conn)
{
  cout << conn->localAddress().toIpPort() << " -> "
       << conn->peerAddress().toIpPort() << " is "
       << (conn->connected() ? "UP" : "DOWN")
       << endl;
}

void P2PCServer::onStringMessage(const TcpConnectionPtr& conn,
                     const string& message,
                     Timestamp)
{
  char type = message[0];
  int  id   = message[1]; 
  string ip = message.substr(2,4);
  int reqfile  = message[6]; 
  insertNeiIP(ip, reqfile);
  int localfile = getFile();
    
  string ret = "d0";
  ret.append(getIP());
  ret.append(1, (char)localfile);
  codec_.send(conn, ret);

    
}
    
