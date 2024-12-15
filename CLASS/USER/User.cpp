// User.cpp
#include "User.h"
#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include "../IPCMESSAGE/IPCMessage.h"

#define IPC_KEY_TO_USER 1234
#define IPC_KEY_TO_KERNEL 5678

User::User(pid_t pid, int numOfPage) : pid(pid), numOfPage(numOfPage) {
    for (int i = 0; i < numOfPage; i++) {
        Page* page= new Page(pid,i);
        pages.push_back(page);
    }
    std::cout << "User created with PID: " << pid << " and " << numOfPage << " pages.\n";
}

void User::receiveCommand() {
  int msgid_from_kernel = msgget(IPC_KEY_TO_USER, 0666 | IPC_CREAT);
  int msgid_to_kernel = msgget(IPC_KEY_TO_KERNEL, 0666 | IPC_CREAT);

  if (msgid_from_kernel == -1 || msgid_to_kernel == -1) {
    perror("msgget failed");
    return;
  }

  IPCMessageFromUser msg_to_kernel;
  IPCMessageFromKernel msg_from_kernel;

  while (true){
    ssize_t result = msgrcv(msgid_from_kernel, &msg_from_kernel, sizeof(msg_from_kernel) - sizeof(long), getpid(), 0);
    if (result == -1) {
      perror("msgrcv failed");
      continue;
    }

    msg_to_kernel.mtype = getpid();
    msg_to_kernel.current_pageNumber = current_page_index;

    if (msgsnd(msgid_to_kernel, &msg_to_kernel, sizeof(msg_to_kernel) - sizeof(long), 0) == -1) {
      std::cerr << "USER: Failed to send message to KERNEL " << this->current_page_index<< "\n";
    }

    current_page_index = (current_page_index + 1) % numOfPage;
  }
}
