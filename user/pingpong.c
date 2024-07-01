#include "kernel/types.h"
#include "user/user.h"

const int READ_END = 0;
const int WRITE_END = 1;

int main(int argc, char* argvp[]) {
  int pPaWrite[2];
  int pSonWrite[2];
  char buf[1];

  pipe(pPaWrite);
  pipe(pSonWrite);

  int pid = fork();
  if (0 == pid) {
    close(pPaWrite[WRITE_END]);
    read(pPaWrite[READ_END], (void*)buf, 1);
    fprintf(READ_END, "%d: received ping\n", getpid());

    write(pSonWrite[WRITE_END], (void*)buf, 1);
    close(pSonWrite[WRITE_END]);
    exit(0);
  }
  // else if (pid > 0) {
  close(pSonWrite[WRITE_END]);
  write(pPaWrite[WRITE_END], (void*)buf, 1);
  close(pPaWrite[WRITE_END]);

  read(pSonWrite[READ_END], (void*)buf, 1);
  fprintf(READ_END, "%d: received pong\n", getpid());
  exit(0);
}