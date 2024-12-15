#include "./PTE.h"

// 기본 생성자 정의
PTE::PTE(int addr = -1, int num = -1, bool dirty = false, bool valid = false)
        : physicalAddress(addr), pageNumber(num), dirtyBit(dirty), validBit(valid) {}

bool PTE::checkInMemory(int pageNumber) {
        return this->pageNumber == pageNumber && this->physicalAddress != -1;
}
