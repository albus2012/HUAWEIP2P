#ifndef MUDUO_EXAMPLES_ASIO_CHAT_CODEC_H
#define MUDUO_EXAMPLES_ASIO_CHAT_CODEC_H

#include "Buffer.h"
#include "TcpConnection.h"
#include "StringPiece.h"
#include <boost/function.hpp>

#include <stdint.h>
#include <endian.h>
#include <iostream>

using namespace huaweip2p;
class LengthHeaderCodec 
{
 public:
  typedef boost::function<void (const TcpConnectionPtr&,
                                const string& message,
                                Timestamp)> StringMessageCallback;

  explicit LengthHeaderCodec(const StringMessageCallback& cb)
    : messageCallback_(cb)
  {
  }

  void onMessage(const TcpConnectionPtr& conn,
                 Buffer* buf,
                 Timestamp receiveTime)
  {
    while (buf->readableBytes() >= kHeaderLen) // kHeaderLen == 4
    {
      // FIXME: use Buffer::peekInt32()
      const void* data = buf->peek();
      int32_t be32 = *static_cast<const int32_t*>(data); // SIGBUS
      const int32_t len = be32toh(be32);
      if (len > 65536 || len < 0)
      {
        std::cout << "can't read" << std::endl;
        conn->shutdown();  // FIXME: disable reading
        break;
      }
      else if (buf->readableBytes() >= len + kHeaderLen)
      {
        buf->retrieve(kHeaderLen);
        string message(buf->peek(), len);
        messageCallback_(conn, message, receiveTime);
        buf->retrieve(len);
      }
      else
      {
        break;
        //conn->shutdown();

      }
    }
  }

  // FIXME: TcpConnectionPtr
  void send(const huaweip2p::TcpConnectionPtr& conn,
            const huaweip2p::StringPiece& message)
  {
    sleep(1);
    Buffer buf;
    buf.append(message.data(), message.size());
    int32_t len = static_cast<int32_t>(message.size());
    int32_t be32 = htobe32(len);
    buf.prepend(&be32, sizeof be32);
    conn->send(&buf);
    std::cout << std::endl << "codec send" << std::endl;
    //conn->shutdown();
  }

 private:
  StringMessageCallback messageCallback_;
  const static size_t kHeaderLen = sizeof(int32_t);

};

#endif  // MUDUO_EXAMPLES_ASIO_CHAT_CODEC_H
