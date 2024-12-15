#include "./PTE.h"

// 기본 인자를 제거한 생성자 정의
PTE::PTE(int addr, int num, bool dirty, bool valid)
        : physicalAddress(addr), pageNumber(num), dirtyBit(dirty), validBit(valid) {}

bool PTE::checkInMemory(int pageNumber) {
    return this->pageNumber == pageNumber && this->physicalAddress != -1;
}
