#include <iostream>
#include <queue>
#include <unistd.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <csignal> 
#include "./CLASS/USER/User.h"
#include "./CLASS/MEMORY/Memory.h"
#include "./CLASS/DISK/Disk.h"
#include "./CLASS/MMU/MMU.h"
#include "./CLASS/KERNEL/Kernel.h"
#define NUM_OF_PROCESSES 10 
#define TIME_TICK 100 // 100ms
using namespace std;

int main(){
  Memory* memory = new Memory();
  Disk* disk = new Disk();
  MMU* mmu = new MMU();
  // 커널
  Kernel* kernel = new Kernel(memory,disk,mmu);

  vector<pid_t> child_pids;

  // 자식 프로세스 10개 생성
  for (int i = 0; i < NUM_OF_PROCESSES; ++i) {
    pid_t pid = fork(); // 자식 프로세스 생성
    int cpu_burst = (rand() % 1501 + 4500) * TIME_TICK; // 45초 ~ 60초
    if (pid == 0) { // 자식 프로세스
      cout << "Child Process Created: PID = " << getpid()
            << ", CPU Burst = " << cpu_burst;
      User user(getpid(), cpu_burst); // User 객체 생성
      user.receiveCommand();                  // 명령 수신 대기
      cout << "Child Process " << getpid() << " Terminating after receiveCommand.\n";

      exit(0);                                // 자식 프로세스 종료
    } else if (pid > 0) { // 부모 프로세스
        child_pids.push_back(pid); // 자식 PID 저장
        cout << "Kernel: add process to queue \n";

    } else {
      cerr << "Fork failed.\n";
      return 1;
    }
  }

  // 자식 프로세스 종료 대기
  for (pid_t child_pid : child_pids) {
      int status;
      waitpid(child_pid, &status, 0); // 자식 프로세스 종료 확인
      if (WIFEXITED(status)) {
          cout << "Child Process PID = " << child_pid << " exited with status " << WEXITSTATUS(status) << ".\n";
      }
  }

  cout << "All child processes have completed.\n";
}