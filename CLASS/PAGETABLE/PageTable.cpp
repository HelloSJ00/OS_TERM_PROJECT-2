#include "./PageTable.h"

PageTable::PageTable(pid_t pid,int NumOfPage){
  this->pid = pid;
  for (int i = 0; i < NumOfPage; ++i) {
    PTE* pte = new PTE(-1,i,false,false); // PTE 객체 동적 생성
    PTEs.push_back(pte); // vector에 추가
  }
}

bool PageTable::checkInMemory(pid_t pid,int pageNumber){
  for (PTE* pte : PTEs){
    if(pte->checkInMemory(pageNumber)) return true;
  }
  //Page fault
  return false;
}

PTE* PageTable::findPTE(int pageNumber){
  for (PTE* pte : PTEs){
    if(pte->pageNumber == pageNumber){
      return pte;
    }
  }
  return nullptr; // 해당 pageNumber가 없으면 nullptr 반환
}