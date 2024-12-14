#ifndef KERNEL_H
#define KERNEL_H

class Kernel{
  Memory* memory;
  Disk* disk;
  MMU* mmu;
public:
  Kernel(Memory* memory,Disk* disk,MMU* mmu);
};
#endif