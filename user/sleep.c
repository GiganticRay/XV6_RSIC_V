#include "kernel/types.h"
#include "user/user.h"

const int STDIN = 0;

int main(int argc, char* argv[]) {
  int sleepTime;
  if (argc != 2) {
    fprintf(2, "Usage: sleep <seconds>\n");
  }

  sleepTime = atoi(argv[1]);

  sleep(sleepTime);

  exit(0);
}