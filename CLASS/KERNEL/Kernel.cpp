#include "./Kernel.h"

#include <sys/ipc.h>
#include <sys/msg.h>
#include <iostream>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <cstring>
#include <fstream>  // 파일 출력을 위한 헤더 추가
#include <filesystem>
#include "../IPCMESSAGE/IPCMessage.h"
using namespace std;

// IPC 키 설정
#define IPC_KEY_TO_USER 1234
#define IPC_KEY_TO_KERNEL 5678
#define MIN_EXECUTION_TIME 60000  // 최소 실행 시간 (600초)

Kernel* Kernel::global_kernel = nullptr;
// 생성자
Kernel::Kernel(Memory* memory,MMU* mmu,LRUCache* cache)
    :memory(memory) , mmu(mmu), cache(cache) ,time(0){
    global_kernel = this; // 현재 kernel 객체를 전역으로 설정
    start_timer();
}

// 타이머 핸들러
void Kernel::timer_handler(int signum) {
    if (global_kernel) {
        global_kernel->tick(); // 전역 kernel 객체의 tick 호출
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

void Kernel::tick() {
    time += 100; // Tick 시간 증가 100ms
    cout << time << "ms" << endl;
    if (time > MIN_EXECUTION_TIME) {
        return;
    }
    // 현재 child_pid 접근
    pid_t current_pid = child_pids[current_pid_index];
    cout << "Tick: " << time << ", Processing PID: " << current_pid << endl;

    // IPC 메시지 작성
    int msgid_to_user = msgget(IPC_KEY_TO_USER, 0666 | IPC_CREAT);
    IPCMessageFromKernel msg_to_user;
    msg_to_user.mtype = child_pids[this->current_pid_index];

    // 메시지 전송
    if (msgsnd(msgid_to_user, &msg_to_user, sizeof(msg_to_user) - sizeof(long), 0) == -1) {
        cerr << "KERNEL: Failed to send message to User process " << this->current_pid_index << "\n";
        return;
    }

    // 자식으로부터 메시지 수신
    int msgid_from_user = msgget(IPC_KEY_TO_KERNEL, 0666 | IPC_CREAT); // 메시지 큐 ID
    IPCMessageFromUser msg_from_user;
    while (true) {
        if (msgrcv(msgid_from_user, &msg_from_user, sizeof(msg_from_user) - sizeof(long), 0, 0) == -1) {
            cerr << "[ERROR] Failed to receive message from User process" << endl;
            continue;
        } else {
            cout << "[DEBUG] Message received from user process PID: " << msg_from_user.mtype << endl;
            cout << "Page Number: " << msg_from_user.current_pageNumber << endl;
            break;
        }
    }

    // 받은 메시지의 PID와 pageNum으로 메모리에서 찾기
    if (!this->checkInMemory(msg_from_user.mtype, msg_from_user.current_pageNumber)) {
        cout << "[DEBUG] Page fault is triggered: " << msg_from_user.mtype << endl;
        cout << "Page Number: " << msg_from_user.current_pageNumber << endl;
        pageFault(msg_from_user.mtype, msg_from_user.current_pageNumber);
    } else {
        cout << "[DEBUG] Page found in memory for PID: " << msg_from_user.mtype << endl;
        cout << "Page Number: " << msg_from_user.current_pageNumber << endl;
    }

    // 가상 주소 -> 물리 주소 변환
    int physicalAddress = mmu->getPhysicalAddress(msg_from_user.mtype, msg_from_user.current_pageNumber);
    cout << "Translated virtual address to physical address: " << physicalAddress << endl;

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
  using namespace std;
  using namespace std::filesystem;

  // 덤프 파일이 저장될 폴더 경로
  string folder_path = "./dump";

  // 덤프 파일 경로 설정
  string file_path = folder_path + "/page_fault_dump.txt";

  // 폴더가 존재하지 않으면 생성
  if (!exists(folder_path)) {
    create_directories(folder_path);
  }

  // 페이지 객체 생성
  Page* page = new Page(pid,pageNumber);

  // swapOut 필요하면 수행
  int swappedOutAddress = -1;
  if(memory->isFull()){
    int physicalAddress = LRU_Algorithm();
    swappedOutAddress = physicalAddress;

    // swapOut 전후로 로그 작성
    {
      ofstream out(file_path, ios::app);
      if (out.is_open()) {
        out << "[PAGE FAULT] PID: " << pid << ", PageNumber: " << pageNumber 
            << " - Memory Full. Performing swapOut on PhysicalAddress: " << physicalAddress << "\n";
      } else {
        cerr << "Error: Unable to open page_fault_dump.txt for writing (swapOut).\n";
      }
    }

    // 실제 swapOut 수행
    memory->swapOut(physicalAddress);

    {
      ofstream out(file_path, ios::app);
      if (out.is_open()) {
        out << "[PAGE FAULT] PID: " << pid << ", PageNumber: " << pageNumber 
            << " - swapOut Complete for PhysicalAddress: " << physicalAddress << "\n";
      }
    }
  }

  // swapIn 수행
  int pa = memory->swapIn(pid,page);
  cout << "Logical Address : " << pid <<", " << pageNumber 
       << " ->> PhysicalAddress >> " << pa << " SWAP IN" << endl;

  {
    ofstream out(file_path, ios::app);
    if (out.is_open()) {
      out << "[PAGE FAULT] PID: " << pid << ", PageNumber: " << pageNumber 
          << " - swapIn Complete at PhysicalAddress: " << pa << "\n";
    } else {
      cerr << "Error: Unable to open page_fault_dump.txt for writing (swapIn).\n";
    }
  }

  // LRU 캐시 업데이트
  cache->add(pa);
  cout << "LRU CACHE UPDATE " << endl;

  {
    ofstream out(file_path, ios::app);
    if (out.is_open()) {
      out << "[LRU UPDATE] PhysicalAddress: " << pa << " added to LRU Cache.\n";
    } else {
      cerr << "Error: Unable to open page_fault_dump.txt for writing (LRU Update).\n";
    }
  }
}


void Kernel::run(){
  while(time < MIN_EXECUTION_TIME){
    pause();
  }
}