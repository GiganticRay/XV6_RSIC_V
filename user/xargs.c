#include "kernel/param.h"
#include "kernel/types.h"
#include "user/user.h"

const int STDIN = 0;
const int STDOUT = 1;
const int MAXLEN = 100;

void doCommand(char* cmd, int argc, char** argv) {
  if (fork() == 0) {
    exec(cmd, argv);
    exit(0);
  } else {
    wait((int*)0);
  }
}

int main(int argc, char* argv[]) {
  char xArgsArgv[MAXARG][MAXLEN];
  char* xArgsArgv_pointers[MAXARG];  // Array of pointers

  // Copy the command line arguments to the xArgsArgv array
  for (int i = 1; i < argc; i++) {
    strcpy(xArgsArgv[i - 1], argv[i]);
    xArgsArgv_pointers[i - 1] = xArgsArgv[i - 1];  // Initialize pointers
  }

  // Read the input from the standard input and append it into the xArgsArgv
  // array
  char c;
  int argsIdx = argc - 1;
  int charOffset = 0;

  while (read(STDIN, &c, 1) > 0) {
    if (c == '\n') {
      if (charOffset != 0) {
        xArgsArgv[argsIdx][charOffset] = '\0';  // Null-terminate the string
        xArgsArgv_pointers[argsIdx] = xArgsArgv[argsIdx];  // Initialize pointer
        argsIdx++;
      }

      doCommand(xArgsArgv[0], argsIdx - 1, xArgsArgv_pointers);

      memset(xArgsArgv + argc - 1, 0,
             (MAXARG - argc + 1) * MAXLEN * sizeof(char));
      argsIdx = argc - 1;
      charOffset = 0;
      continue;
    }

    if (c == ' ') {
      if (charOffset != 0) {
        xArgsArgv[argsIdx][charOffset] = '\0';  // Null-terminate the string
        xArgsArgv_pointers[argsIdx] = xArgsArgv[argsIdx];  // Initialize pointer
        argsIdx++;
        charOffset = 0;
      }
      continue;
    }

    xArgsArgv[argsIdx][charOffset++] = c;
  }

  if (charOffset != 0) {
    xArgsArgv[argsIdx][charOffset] = '\0';  // Null-terminate the last argument
    xArgsArgv_pointers[argsIdx] = xArgsArgv[argsIdx];  // Initialize pointer
    doCommand(xArgsArgv[0], argsIdx, xArgsArgv_pointers + 1);
  }

  exit(0);
}
