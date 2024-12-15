#ifndef PTE_H
#define PTE_H
class PTE {
    bool validBit;
    bool dirtyBit;
public:
    int physicalAddress;
    int pageNumber;
    // 기본 인자를 선언부에 정의
    PTE(int addr = -1, int num = -1, bool dirty = false, bool valid = false);

    bool checkInMemory(int pageNumber);

    void isValid() {
        this->validBit = 1;
    }

    void isInValid() {
        this->validBit = 0;
    }
};
#endif
