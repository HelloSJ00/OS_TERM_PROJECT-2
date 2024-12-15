#include "./User.h"
#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>  // pid_t 타입을 위해 필요
#include <unistd.h> // getpid()
#include "./IPCMESSAGE/IPCMessage.h"
// IPC 키 설정
#define IPC_KEY_TO_USER 1234
#define IPC_KEY_TO_KERNEL 5678

using namespace std;

User::User(pid_t pid, int numOfPage) : pid(pid), numOfPage(numOfPage) {
    // numOfPage 만큼 Page 객체 생성
    for (int i = 0; i < numOfPage; i++) {
        Page page(pid,i);  // 페이지 번호를 순서대로 설정 (1부터 시작)
        pages.push_back(page);     // pages 벡터에 추가
    }
    cout << "User created with PID: " << pid << " and " << numOfPage << " pages.\n";
}

void User::receiveCommand() {
  int msgid_to_kernel = msgget(IPC_KEY_TO_USER, 0666 | IPC_CREAT);
  int msgid_from_kernel = msgget(IPC_KEY_TO_KERNEL, 0666 | IPC_CREAT); // 메시지 큐 ID

  if (msgid_to_kernel == -1 || msgid_from_kernel == -1) {
    perror("msgget failed");
    return; // 비정상 종료 방지
  }
  IPCMessageFromUser msg_to_kernel;
  IPCMessageFromKernel msg_from_kernel;

  while (true){
    // 메시지 수신 및 오류 확인
    ssize_t result = msgrcv(msgid_from_kernel, &msg_from_kernel, sizeof(msg_from_kernel) - sizeof(long), getpid(), 0);
    if (result == -1) {  // 메시지 수신 실패
      perror("msgrcv failed");
      continue;  // 루프 계속
    }

    msg_to_kernel.mtype = getpid();
    msg_to_kernel.current_pageNumber = current_page_index;

    // 메시지 전송
    if (msgsnd(msgid_to_kernel, &msg_to_kernel, sizeof(msg_to_kernel) - sizeof(long), 0) == -1) {
      cerr << "USER: Failed to send message to KERNEL " << this->current_page_index<< "\n";
    }

    current_page_index = (current_page_index +1)%numOfPage;
  }
}
