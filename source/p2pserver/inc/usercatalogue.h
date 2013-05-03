#ifndef HUAWEIP2P_USERCATALOGUE_H
#define HUAWEIP2P_USERCATALOGUE_H

#include <string>
#include <set>
#include <map>
#include <queue>

#define MAXQUE 3
using namespace std;


typedef struct UserInfo
{
  string userIP;
  int userFile;
  bool operator<(const UserInfo& t) const
  {
    return userIP.compare(t.userIP);
  }
}UserInfo;  
//class UserInfoSortCriterion
//{
// public:
//  bool operator() (const UserInfo& p1, const UserInfo& p2) const
//  {
//    return p1.userIP < p2.userIP;
//  }
//};
class UserCatalogue
{
 public:
 //typedef set<UserInfo, UserInfoSortCriterion> USET;
  explicit UserCatalogue(const string ip = "1111", int file = 100);
  set<UserInfo> getNeighborUsers(const string ip);
  void insertUser(const string ip, int file);
  int hasNeighbor(const string ip);
  
  

 private:
  string processIP(const string ip);
  bool isClientWorking(const string ip);
  //typedef set<UserInfo, UserInfoSortCriterion> USET;
  typedef map<string, set<UserInfo> > UserMap;
  typedef map<string, queue<string> > UserQue;
  UserQue userQue;
  UserMap users;
  const int kMaxCacheUsers;


};

#endif //HUAWEIP2P_USERCATALOGUE_H
