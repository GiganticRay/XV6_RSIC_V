#include "types.h"
#include "param.h"
#include "spinlock.h"
#include "riscv.h"
#include "proc.h" // depends on spinlock & pagetable_t in riscv.h
#include "syscall.h"
#include "defs.h"

uint64 sys_trace(void) {
  int traceMask;
  if (argint(0, &traceMask) < 0) {
    printf("sys_trace: invalid argument\n");
    return -1;
  }

  myproc()->traceMask = traceMask;
  return 0;
}