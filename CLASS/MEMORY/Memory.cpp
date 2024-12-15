#include "./Memory.h"

bool Memory::checkInMemory(pid_t pid,int numOfPage){
  // 모든 PageTable을 순회
  PageTable* tmp;
  for (PageTable* pageTable : pageTables){
    if (pageTable->isEqualPid(pid)){
      tmp = pageTable;
    }
  }
  return tmp->checkInMemory(pid,numOfPage);
}

PageTable* Memory::findPageTable(pid_t pid){
  for(PageTable* pageTable : pageTables){
    if(pageTable->pid == pid){
      return pageTable;
    }
  }
}

int Memory::swapIn(pid_t pid,Page* page){
  for(int i = 0; i< 8 ; i++){
    if (physicalMemory[i]->pid == -1){
      //메모리에 적재 
      physicalMemory[i] = page;

      //페이지 테이블 수정 
      PageTable* fPageTable = findPageTable(pid);
      PTE* fPTE = fPageTable->findPTE(page->pageNumber);
      fPTE->isValid();
      fPTE->physicalAddress = i;
      return fPTE->physicalAddress;
    }
  }
}

void Memory::swapOut(int physicalAddress){
  Page* page = this->physicalMemory[physicalAddress];

  //페이지 테이블 수정 
  PageTable* fPageTable = findPageTable(page->pid);
  PTE* fPTE = fPageTable->findPTE(page->pageNumber);
  fPTE->isInValid();
  fPTE->physicalAddress = -1;

  // 물리 메모리 수정
  page->pid = -1;
  page->pageNumber = -1;
}

bool Memory::isFull(){
  for(int i = 0; i< 8 ; i++){
    if (physicalMemory[i]->pid == -1) return true;
  }
  return false;
}

int Memory::getPhysicalMemory(pid_t pid,int pageNumber){
  PageTable* fPageTable = findPageTable(pid);
  PTE* fPTE = fPageTable->findPTE(pageNumber);
  return fPTE->physicalAddress;
}