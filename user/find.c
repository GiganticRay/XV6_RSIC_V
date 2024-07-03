#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "user/user.h"

const char* CURR_DIR = ".";
const char* PREV_DIR = "..";

void find(char* path, char* fileName) {
  char buf[512];
  char* p;
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(path, O_RDONLY)) < 0) {
    printf("find: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0) {
    printf("find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch (st.type) {
    case T_FILE:
      break;
    case T_DIR:
      if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
        printf("find: path too long\n");
        break;
      }

      strcpy(buf, path);
      p = buf + strlen(buf);
      *p++ = '/';

      while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (0 == de.inum || 0 == strcmp(de.name, CURR_DIR) ||
            0 == strcmp(de.name, PREV_DIR)) {
          continue;
        }

        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;

        if (strcmp(de.name, fileName) == 0) {
          printf("%s\n", buf);
        }

        find(buf, fileName);
      }
      break;
  }

  close(fd);
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    fprintf(2, "Usage: find <path> <name>\n");
    exit(-1);
  }

  char* path = argv[1];
  char* fileName = argv[2];

  find(path, fileName);

  exit(0);
}