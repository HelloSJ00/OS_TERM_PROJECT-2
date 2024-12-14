#ifndef PAGETABLE_H
#define PAGETABLE_H

class PageTable{
  int physicalAddress;
  bool dirtyBit;
  bool validBit;
public:
  PageTable(int physicalAddress);
};

#endif