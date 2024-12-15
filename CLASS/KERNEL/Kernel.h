#ifndef KERNEL_H
#define KERNEL_H

#include <vector>
#include <sys/types.h>
#include "./MEMORY/Memory.h"
#include "./MMU/MMU.h"
#include "./LRUCache/LRUCache.h"

using namespace std;

class Kernel{
  Memory* memory;
  MMU* mmu;
  LRUCache* cache;

  static Kernel* global_kernel;           // Singleton-like 인스턴스 포인터
  int time;
  // 자식 프로세스 PID 값들
  vector<pid_t> child_pids;
  size_t current_pid_index = 0; // 현재 접근 중인 PID 인덱스

  static void timer_handler(int signum);
  void start_timer();

  bool checkInMemory(pid_t pid,int pageNumber);
  // Page Fault
  void pageFault(pid_t pid,int pageNumber);
  int LRU_Algorithm();
public:
  // 생성자
  Kernel(Memory* memory,MMU* mmu,LRUCache* cache);

  void addChildPID(pid_t pid,int numOfPage);

  void tick();
};
#endif