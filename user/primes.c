
#include "kernel/types.h"
#include "user/user.h"

const int READ_END = 0;
const int WRITE_END = 1;

void filterAndFork(int readEnd) {
  int base = -1; 
  if (0 == read(readEnd, (void*)&base, sizeof(base))) {
    return;
  }
  printf("prime %d\n", base);

  int p[2];
  pipe(p);

  int num;
  if (fork() == 0) {
    while(read(readEnd, (void*)&num, sizeof(num))) {
      if (num % base != 0) {
        write(p[WRITE_END], (void*)& num, sizeof(num));
      }
    }

    close(p[READ_END]);
    close(p[WRITE_END]);
    close(readEnd);
  } else {
    close(p[WRITE_END]);
    close(readEnd);
    filterAndFork(p[READ_END]);
  }

}

int main(int argc, char *argv[]) {
  int p[2];
  pipe(p);

  int i = 2; 
  while (i <= 35) {
    write(p[WRITE_END], (void*)&i, sizeof(i));
    i++;
  }
  close(p[WRITE_END]);

  filterAndFork(p[READ_END]);

  exit(0);
}