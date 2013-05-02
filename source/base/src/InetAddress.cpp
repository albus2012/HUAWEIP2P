#include"InetAddress.h"
#include"SocketsOps.h"

#include<string>
#include<strings.h> //bzero
#include<netinet/in.h>

#include<boost/static_assert.hpp>


static const in_addr_t kInaddrAny = INADDR_ANY;

using namespace huaweip2p;
using std::string;

BOOST_STATIC_ASSERT(sizeof(InetAddress) == sizeof(struct sockaddr_in));

InetAddress::InetAddress(uint16_t port)
{
  bzero(&addr_, sizeof(addr_));
  addr_.sin_family = AF_INET;
  addr_.sin_addr.s_addr = htobe32(kInaddrAny);
  addr_.sin_port = htobe16(port);
}

InetAddress::InetAddress(const string& ip, uint16_t port)
{
  bzero(&addr_, sizeof(addr_));
  sockets::fromIpPort(ip.data(), port, &addr_);
}

string InetAddress::toIpPort() const
{
  char buf[32];
  sockets::toIpPort(buf, sizeof(buf), addr_);
  return buf;
}

string InetAddress::toIp() const
{
  char buf[32];
  sockets::toIp(buf, sizeof(buf), addr_);
  return buf;
}

