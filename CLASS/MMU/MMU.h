#ifndef MMU_H
#define MMU_H

#include "../MEMORY/Memory.h"
class MMU{
  Memory* memory;
public:
  MMU(Memory* memory): memory(memory){}
  int getPhysicalAddress(pid_t pid,int pageNumber){
    return memory->getPhysicalMemory(pid,pageNumber);
  };
};
#endif