#ifndef HUAWEIP2P_INETADDRESS_H
#define HUAWEIP2P_INETADDRESS_H

//#include<StringPiece.h>

#include<netinet/in.h>
#include<string>
using std::string;
namespace huaweip2p
{

class InetAddress
{
 public:
  //Construct an endpoint with given port number
  //mostly used in TcpServer listening
  explicit InetAddress(uint16_t port);

  //Construct an endpoint with given ip and port
  //@c ip should be "1.2.3.4"
  InetAddress(const string& ip, uint16_t port);

  //Construct an endpoint with given struct @c sockaddr_in
  //Mostly usedly when accpeting new connections
  InetAddress(const struct sockaddr_in &addr)
    : addr_(addr)
  { }
  string toIp() const;
  string toIpPort() const;
  string toHostPort() const __attribute__((deprecated))
  { return toIpPort(); }

  //default copy/assignment are okay
  
  const struct sockaddr_in& getSockAddrInet() const
  {
    return addr_;
  }
  
  void setSockAddrInet(const struct sockaddr_in& addr)
  {
    addr_ = addr;
  }

  uint32_t ipNetEndian() const
  {
    return addr_.sin_addr.s_addr;
  }

  uint16_t portNetEndian() const
  {
    return addr_.sin_port;
  }

 private:
  struct sockaddr_in addr_;
};

}


#endif //HUAWEIP2P_INETADDRESS_H
