#ifndef MEMORY_H
#define MEMORY_H

#include "./PAGETABLE/PageTable.h"
#include "./PAGE/Page.h"
class Memory{
  vector<PageTable*> pageTables;
  Page* physicalMemory[8];
public:
  // 프로세스가 생성되면 페이지테이블 메모리에 페이지 테이블 추가
  void addPageTable(PageTable* pageTable);

  // swap out
  void swapOut(int physicalAddress);

  // swap in
  int swapIn(pid_t pid,Page* page);

  // 메모리에 적재되어있는지 
  bool checkInMemory(pid_t pid,int pageNumber);
  
  PageTable* findPageTable(pid_t);

  // 메모리가 찼는지
  bool isFull();

  int getPhysicalMemory(pid_t pid,int pageNumber);
};
#endif