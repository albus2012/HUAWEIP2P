#ifndef HUAWEIP2P_BUFFER_H
#define HUAWEIP2P_BUFFER_H

#include<algorithm>
#include<vector>
#include<assert.h>
#include<string>
using std::string;

namespace huaweip2p
{

class Buffer
{
 public:
  static const size_t kCheapPrepend = 8;
  static const size_t kInitialSize = 1024;

  Buffer()
    : buffer_(kCheapPrepend + kInitialSize),
      readerIndex_(kCheapPrepend),
      writerIndex_(kCheapPrepend)
  {
    assert(readableBytes() == 0);
  }

  //default copy-ctor, dtor and assignment are fine

  size_t readableBytes() const
  {
    return writerIndex_ - readerIndex_; 
  }
  
  size_t writableBytes() const
  {
    return buffer_.size() - writerIndex_;
  }
  
  size_t prependableBytes() const
  {
    return readerIndex_;
  }
  
  const char* peek() const
  {
    return begin() + readerIndex_;
  }
  
  const char* findCRLF() const
  {
    const char* crlf = std::search(peek(), beginWrite(), kCRLF, kCRLF+2);
    return crlf == beginWrite() ? NULL: crlf;
  }
  
  //retrieve returns void
  void retrieve(size_t len)
  {
    assert(len <= readableBytes());
    if(len < readableBytes())
    {
      readerIndex_ += len;
    }
    else
    {
      retrieveAll();
    }
  }
  void retrieveUntil(const char* end)
  {
    assert(peek() <= end);
    assert(end <= beginWrite());
    retrieve(end - peek());
  }
  
  void retrieveAll()
  {
    readerIndex_ = kCheapPrepend;
    writerIndex_ = kCheapPrepend;
  }
  
  string retrieveAllAsString()
  {
    return retrieveAsString(readableBytes());
  }

  string retrieveAsString(size_t len)
  {
    assert(len <= readableBytes());
    string result(peek(), len);
    retrieve(len);
    return result;
  }

  void append(const char* data, size_t len)
  {
    ensureWritableBytes(len);
    std::copy(data, data+len, beginWrite());
    hasWritten(len);
  }
  void append(const void* data, size_t len)
  {
    append(static_cast<const char*>(data), len);
  }
  
  void ensureWritableBytes(size_t len)
  {
    if(writableBytes() < len)
    {
      makeSpace(len);
    }
    assert(writableBytes() >= len);
  }
   
  char* beginWrite()
  {
    return begin() + writerIndex_;
  }
  
  const char* beginWrite() const
  {
    return begin() + writerIndex_;
  }

  void hasWritten(size_t len)
  { writerIndex_ += len; }

  void prepend(const void* data, size_t len)
  {
    assert(len <= prependableBytes());
    readerIndex_ -= len;
    const char* d = static_cast<const char*>(data);
    std::copy(d, d+len, begin()+readerIndex_);
  }

//  void shrink(size_t reserve)
//  {
//    Buffer other;
//    other.ensureWritableBytes(readableBytes()+reserve);
//    //todo
//    other.append(toStringPiece());
//    swap(other);
//  }

  ssize_t readFd(int fd, int* savedErrno);
 private:
  char* begin()
  { return &*buffer_.begin(); }
  
  const char* begin() const
  { return &*buffer_.begin(); }

  void makeSpace(size_t len)
  {
    if(writableBytes() + prependableBytes() < len + kCheapPrepend)
    {
      buffer_.resize(writerIndex_ + len);
    }
    else
    {
      assert(kCheapPrepend < readerIndex_);
      size_t readable = readableBytes();
      std::copy(begin() + readerIndex_, 
                begin() + writerIndex_, 
                begin() + kCheapPrepend);
      readerIndex_ = kCheapPrepend;
      writerIndex_ = readerIndex_ + readable;
      assert(readable == readableBytes());
    }
  }

 private:
  std::vector<char> buffer_;
  size_t readerIndex_;
  size_t writerIndex_;
  static const char kCRLF[];
};
}
#endif 
