#ifndef HUAWEIP2P_STRINGPIECE_H
#define HUAWEIP2P_STRINGPIECE_H

#include<string.h>
#include<iosfwd>

#ifndef HUAWEIP2P_STD_STRING
#include<string>
#endif

namespace huaweip2p
{

class StringPiece
{
 private:
  const char* ptr_;
  int length_;
  
 public:
  
  StringPiece()
    : ptr_(NULL), length_(0) { }
  StringPiece(const char* str)
    : ptr_(str), length_(static_cast<int>(strlen(ptr_))) { }
  
  StringPiece(const unsigned char* str)
    : ptr_(reinterpret_cast<const char*>(str)),
      length_(static_cast<int>(strlen(ptr_))) { }
  StringPiece(const string& str)
    : ptr_(str.data()),length_(static_cast<int>(str.size())){}
  const char* data() const { return ptr_;}
  int size() const { return length_;}


};
}
#endif
