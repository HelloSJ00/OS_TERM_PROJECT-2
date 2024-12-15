// User.h
#ifndef USER_H
#define USER_H

#include <vector>
#include <sys/types.h>
#include "../PAGE/Page.h"

class User {
  pid_t pid;
  int numOfPage;
  std::vector<Page*> pages;
  size_t current_page_index = 0;

public:
  User(pid_t pid,int numOfPage);
  void receiveCommand();
};

#endif
