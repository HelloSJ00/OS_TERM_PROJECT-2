#ifndef USER_H
#define USER_H

class User{
  pid_t pid;           // 프로세스 ID
  int cpu_burst;       // CPU 버스트 시간
public:
  User(pid_t pid,int cpu_burst);

  // 부모로부터 명령 수신
  void receiveCommand();
};
#endif