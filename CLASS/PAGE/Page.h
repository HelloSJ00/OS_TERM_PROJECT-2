#ifndef PAGE_H
#define PAGE_H

#include <sys/types.h> 
class Page{
public:
  pid_t pid;
  int pageNumber;
  Page(pid_t pid = -1, int pageNumber = -1)
    : pid(pid), pageNumber(pageNumber) {} // 멤버 초기화 리스트 사용
  

};
#endif