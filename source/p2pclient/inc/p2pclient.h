#include <codec.h>
#include <Mutex.h>
#include <EventLoopThread.h>
#include <TcpClient.h>
#include <TcpServer.h>

#include <boost/bind.hpp>
#include <EventLoop.h>
#include <string>
#include <map>
using namespace huaweip2p;
using namespace std;
class P2PClient
{
 public:
  P2PClient(EventLoop* loop, const InetAddress& serverAddr);

  void connect()
  {
    client_->connect();
  }

  void disconnect()
  {
    client_->disconnect();
  }
  bool changeServer(const InetAddress& , EventLoop*);
  bool changeServer(const InetAddress&);
  void write(const StringPiece& message);
  enum ConnState { kConnecting, kConnected, kDisconnected, kDisconnecting};
  void setState(ConnState s) { state_ = s;}
  ConnState getState() { return state_;}

 private:

  void onTestConnection(const TcpConnectionPtr& conn);
  
  void onConnection(const TcpConnectionPtr& conn);

  void onStringMessage(const TcpConnectionPtr&,
                       const string& message,
                       Timestamp);
  int processMessage(string message);

  ConnState state_;
  EventLoop* loop_;
  TcpClient* client_;
  LengthHeaderCodec codec_;
  int isFirst;
  string IP; 
  MutexLock mutex_;
  TcpConnectionPtr connection_;
 
};


class P2PCServer 
{
 public:
  P2PCServer(EventLoop* loop, const InetAddress& listenAddr);

  void start()
  {
    server_.start();
  }
  static void setIP(string ip);
  static string getIP();
  static void insertNeiIP(string ip);
  static void insertNeiIP(string ip, int file);
  static void removeNeiIP(string ip);
  static int getFile();
  static int processUpdateFile(int level);
  static string findNeiIP(int level);
  
 private:
  void onConnection(const TcpConnectionPtr& conn);

  void onStringMessage(const TcpConnectionPtr&, const string&, Timestamp);
    
  EventLoop* loop_;
  TcpServer server_;
  int seq_id;
  LengthHeaderCodec codec_;
  static map<string, int> neiIP;
  static int updateFile;
  static string IP;
};
