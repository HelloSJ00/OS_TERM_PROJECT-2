#include "./Memory.h"
#include <iostream>
#include <cstring>
#include <fstream>  // 파일 출력을 위한 헤더 추가
Memory::Memory()noexcept {
    for (int i = 0; i < 8; ++i) {
        physicalMemory[i] = nullptr; // nullptr로 초기화
    }
}

bool Memory::checkInMemory(pid_t pid,int numOfPage){
  // 모든 PageTable을 순회
  PageTable* tmp = nullptr; // 초기화
  for (PageTable* pageTable : pageTables){
    if (pageTable->isEqualPid(pid)){
      tmp = pageTable;
    }
  }
  if (tmp == nullptr) {
        return false; // 해당 pid가 없으면 false 반환
    }
  return tmp->checkInMemory(pid,numOfPage);
}

void Memory::addPageTable(PageTable* pageTable){
  pageTables.push_back(pageTable);
}

PageTable* Memory::findPageTable(pid_t pid){
  for(PageTable* pageTable : pageTables){
    if(pageTable->pid == pid){
      return pageTable;
    }
  }
  return nullptr; // 해당 pid가 없으면 nullptr 반환
}

int Memory::swapIn(pid_t pid,Page* page){
  for(int i = 0; i< 8 ; i++){
    if (physicalMemory[i] == nullptr){
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
  return -1;
}

void Memory::swapOut(int physicalAddress) {
    Page* page = this->physicalMemory[physicalAddress];
    if (page == nullptr) {
        cerr << "Error: Attempt to swap out a null page at physical address " << physicalAddress << endl;
        return;
    }

    // 페이지 테이블 수정
    PageTable* fPageTable = findPageTable(page->pid);
    if (fPageTable == nullptr) {
        cerr << "Error: Page table not found for PID " << page->pid << endl;
        return;
    }

    PTE* fPTE = fPageTable->findPTE(page->pageNumber);
    if (fPTE == nullptr) {
        cerr << "Error: PTE not found for page number " << page->pageNumber << endl;
        return;
    }

    fPTE->isInValid();
    fPTE->physicalAddress = -1;

    // 물리 메모리 수정
    physicalMemory[physicalAddress] = nullptr;

    cout << "Swapped out Physical Address " << physicalAddress << " and pid ,age number " << page->pid <<"," << page->pageNumber << endl;
}

bool Memory::isFull(){
  for(int i = 0; i< 8 ; i++){
    if (physicalMemory[i] == nullptr) return false;
  }
  return true;
}

int Memory::getPhysicalMemory(pid_t pid,int pageNumber){
  PageTable* fPageTable = findPageTable(pid);
  PTE* fPTE = fPageTable->findPTE(pageNumber);
  return fPTE->physicalAddress;
}