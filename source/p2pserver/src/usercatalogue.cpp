#include"usercatalogue.h"
#include<iostream>
#include<stdlib.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;
UserCatalogue::UserCatalogue(const string ip, int file)
  :kMaxCacheUsers(MAXQUE)
{
  UserInfo usr;
  usr.userIP = ip;
  usr.userFile = file;
  set<UserInfo> uset;
  uset.insert(usr);
  users[processIP(ip)] = uset;
  userQue[processIP(ip)].push(ip);
}
void UserCatalogue::insertUser(const string ip, int file)
{
  //set<UserInfo> uset = users[processIP(ip)];
  //if(kMaxCacheUsers > userQue[processIP(ip)].size())
  
  set<UserInfo> uset;
  UserInfo usr;
  usr.userIP = ip;
  usr.userFile = file;
  //uset.erase(usr);

  UserMap::iterator iter = users.find(processIP(ip));
  if(iter != users.end()) // already have a neighbour record 
  {
    uset = iter->second;
    for(set<UserInfo>::iterator it =uset.begin();
      it != uset.end(); ++it)
      cout <<it->userIP;
    cout << "  my" << ip << endl;

    queue<string> q = userQue[processIP(ip)];
    while(!q.empty())
    {
      cout <<q.front();
      q.pop();
    }
    cout << endl <<"  my" << ip << endl;


    if(uset.erase(usr) == 0)
    {      
      
      if(kMaxCacheUsers > userQue[processIP(ip)].size())
      {
         userQue[processIP(ip)].push(ip);
      }
      else
      {
        string firstIP = userQue[processIP(ip)].front();
        userQue[processIP(ip)].pop();
        if(isClientWorking(firstIP))
        {
          //userQue[processIP(ip)].pop();
          userQue[processIP(ip)].push(firstIP);
          return;          
        }
        else //the front ip is not working
        {
          UserInfo usrtemp;
          usrtemp.userIP = firstIP;
          uset.erase(usrtemp);
          userQue[processIP(ip)].push(ip);
        }
      }
    }
  }
  else
    userQue[processIP(ip)].push(ip);
   
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

bool UserCatalogue::isClientWorking(const string ip)
{
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  const char* kIP = "127.0.0.1";
  struct sockaddr_in addr_in;
  addr_in.sin_family = AF_INET;
  if(inet_pton(AF_INET, kIP, &addr_in.sin_addr) != 1)
  {
    return false;
  }
  addr_in.sin_port = htons(atoi(ip.c_str()));
  if(connect(sockfd, (struct sockaddr *)&addr_in, sizeof(addr_in)) != 0)
  {
    cout << 222;
    return false;
  }
  close(sockfd);
  cout << "size :" << userQue[processIP(ip)].size() << endl;
    queue<string> q = userQue[processIP(ip)];
    while(!q.empty())
    {
      cout <<q.front();
      q.pop();
    }
    cout << endl <<"  my" << ip << endl;
  cout << "new feature 000000" << endl;
  return true;
}
