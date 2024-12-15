#include "./Kernel.h"

#include <sys/ipc.h>
#include <sys/msg.h>
#include <iostream>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include "./IPCMESSAGE/IPCMessage.h"
using namespace std;

// IPC 키 설정
#define IPC_KEY_TO_USER 1234
#define IPC_KEY_TO_KERNEL 5678

Kernel* Kernel::global_kernel = nullptr;
// 생성자
Kernel::Kernel(Memory* memory,MMU* mmu,LRUCache* cache)
    :memory(memory) , mmu(mmu), cache(cache) {
    global_kernel = this; // 현재 Scheduler 객체를 전역으로 설정
    start_timer();
}

// 타이머 핸들러
void Kernel::timer_handler(int signum) {
    if (global_kernel) {
        global_kernel->tick(); // 전역 Scheduler 객체의 tick 호출
    } else {
        cerr << "Error: global_kernel is null!" << endl;
    }
}

// 타이머 시작
void Kernel::start_timer() {
    struct sigaction sa;
    struct itimerval timer;

    // 타이머 핸들러 설정
    sa.sa_handler = &Kernel::timer_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGALRM, &sa, nullptr);

    // 타이머 값 설정 (10ms)
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 100000; // 100ms
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 100000; // 100ms
    setitimer(ITIMER_REAL, &timer, nullptr);
}
int Kernel::LRU_Algorithm(){
  return cache->getLeastRecentlyUsed();
};

void Kernel::tick(){
  time+= 100; // Tick 시간 증가 100ms

  // IPC 메시지 작성
  int msgid_to_user = msgget(IPC_KEY_TO_USER, 0666 | IPC_CREAT);
  IPCMessageFromKernel msg_to_user;
  msg_to_user.mtype = child_pids[this->current_pid_index];
  
  // 메시지 전송
  if (msgsnd(msgid_to_user, &msg_to_user, sizeof(msg_to_user) - sizeof(long), 0) == -1) {
    cerr << "KERNEL: Failed to send message to User process " << this->current_pid_index<< "\n";
  }

  // 자식으로 부터 메세지 수신
  int msgid_from_user = msgget(IPC_KEY_TO_KERNEL, 0666 | IPC_CREAT); // 메시지 큐 ID
  IPCMessageFromUser msg_from_user;

  // 받은 메세지의 pid와 pageNum으로 메모리에 찾기
  if(!this->checkInMemory(msg_from_user.mtype,msg_from_user.current_pageNumber)){
    pageFault(msg_from_user.mtype,msg_from_user.current_pageNumber);
  }

  // 가상 주소 -> 물리 주소 변환
  mmu->getPhysicalAddress(msg_from_user.mtype,msg_from_user.current_pageNumber);

  // 현재 child_pid 접근
  pid_t current_pid = child_pids[current_pid_index];
  cout << "Tick: " << time << ", Processing PID: " << current_pid << endl;

  // 다음 PID로 이동 (순환)
  current_pid_index = (current_pid_index + 1) % child_pids.size();
}

void Kernel::addChildPID(pid_t pid,int numOfPage){
  child_pids.push_back(pid);
  PageTable* pageTable = new PageTable(pid,numOfPage);
  memory->addPageTable(pageTable);
}

bool Kernel::checkInMemory(pid_t pid,int pageNumber){
  return memory->checkInMemory(pid,pageNumber);
}

void Kernel::pageFault(pid_t pid,int pageNumber){
  Page* page = new Page(pid,pageNumber);

  if(memory->isFull()){
    int physicalAddress = LRU_Algorithm();
    memory->swapOut(physicalAddress);
  }
  int pa = memory->swapIn(pid,page);

  // LRU 캐시 업데이트
  cache->add(pa);
}