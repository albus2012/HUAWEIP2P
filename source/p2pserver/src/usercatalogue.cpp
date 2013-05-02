#include"usercatalogue.h"
#include<iostream>
#include<set>
#include<string>
using namespace std;
UserCatalogue::UserCatalogue(const string ip, int file)
{
  UserInfo usr;
  usr.userIP = ip;
  usr.userFile = file;
  set<UserInfo> uset;
  uset.insert(usr);
  users[processIP(ip)] = uset;
}
void UserCatalogue::insertUser(const string ip, int file)
{
  //set<UserInfo> uset = users[processIP(ip)];
  set<UserInfo> uset;
  UserInfo usr;
  UserMap::iterator iter = users.find(processIP(ip));
  if(iter != users.end())
  {
    uset = iter->second;
  }
  usr.userIP = ip;
  usr.userFile = file;
  uset.erase(usr);
  uset.insert(usr);
  users[processIP(ip)] = uset;
}
int UserCatalogue::hasNeighbor(const string ip)
{
  UserMap::iterator iter = users.find(processIP(ip));
  return iter->second.size() - 1;//the num of users except the ipself
}   
set<UserInfo> UserCatalogue::getNeighborUsers(const string ip)
{
  UserMap::iterator iter = users.find(processIP(ip));
  return iter->second;
}
  
string UserCatalogue::processIP(const string ip)
{
  return ip.substr(0, 3);
}
