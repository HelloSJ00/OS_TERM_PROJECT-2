#ifndef IPC_MESSAGES_H
#define IPC_MESSAGES_H

#include <sys/types.h>
// Kernel -> User 메시지 구조체
struct IPCMessageFromKernel {
  long mtype;  // 메시지를 받을 프로세스의 PID 
};

// User -> Kernel 메시지 구조체
struct IPCMessageFromUser {
  long mtype;
  int current_pageNumber;
};

#endif  // IPC_MESSAGES_H