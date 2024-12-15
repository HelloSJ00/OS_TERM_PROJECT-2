#include <iostream>
#include <queue>
#include <unistd.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <csignal> 
#include "./CLASS/USER/User.h"
#include "./CLASS/MEMORY/Memory.h"
#include "./CLASS/MMU/MMU.h"
#include "./CLASS/KERNEL/Kernel.h"
#include "./CLASS/LRUCache/LRUCache.h"
#define NUM_OF_PROCESSES 10 
#define TIME_TICK 100 // 100ms
#define MIN_EXECUTION_TIME 60000  // 최소 실행 시간 (60초)

using namespace std;

int main(){
  // 커널 필드 생성
  Memory* memory = new Memory();
  MMU* mmu = new MMU(memory);
  LRUCache* Cache = new LRUCache();

  // 커널 생성
  Kernel kernel(memory,mmu,Cache);

  // 자식 프로세스 10개 생성
  for (int i = 0; i < NUM_OF_PROCESSES; ++i) {
    pid_t pid = fork(); // 자식 프로세스 생성
    int numOfPage = (rand()%8 + 3 ); // numOfPage 는 3~ 10 사이
    if (pid == 0) { // 자식 프로세스
      cout << "Child Process Created: PID = " << getpid()
            << ",Num Of Page = " << numOfPage;
      User user(getpid() , numOfPage ); // User 객체 생성
      user.receiveCommand();                  // 명령 수신 대기
      cout << "Child Process " << getpid() << " Terminating after receiveCommand.\n";

      exit(0);                                // 자식 프로세스 종료
    } else if (pid > 0) { // 부모 프로세스
        kernel.addChildPID(pid,numOfPage); // 자식 PID 저장
        cout << "Kernel: add process \n";

    } else {
      cerr << "Fork failed.\n";
      return 1;
    }
  }
}