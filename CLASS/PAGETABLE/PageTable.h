#ifndef PAGETABLE_H
#define PAGETABLE_H

#include <vector> // vector 사용을 위해 필요
#include <sys/types.h>  // pid_t 타입을 위해 필요
#include "./PTE/PTE.h"
using namespace std;
class PageTable{
  vector<PTE*> PTEs;
public:
  pid_t pid;
  PageTable(pid_t pid,int NumOfPage);

  int getPhysicalAddress(int NumOfPage);

  bool isEqualPid(pid_t pid){
    return this->pid == pid;
  }
  
  bool checkInMemory(pid_t pid,int pageNumber);

  PTE* findPTE(int pageNumber);
};

#endif