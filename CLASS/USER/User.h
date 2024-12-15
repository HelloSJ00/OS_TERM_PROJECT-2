#ifndef USER_H
#define USER_H
#include <vector>     
#include <sys/types.h> 

#include "./PAGE/Page.h"
using namespace std;
class User{
  pid_t pid;           // 프로세스 ID
  int numOfPage;
  vector<Page> pages;
  size_t current_page_index = 0; // 현재 접근 중인 Page 인덱스

public:
  User(pid_t pid,int numOfPage);
  
  // 부모로부터 명령 수신
  void receiveCommand();
};
#endif