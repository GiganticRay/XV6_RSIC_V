#include "types.h"
#include "kernel/param.h"
#include "kernel/riscv.h"
#include "kernel/defs.h"
#include "kernel/sysinfo.h"
#include "kernel/spinlock.h"
#include "kernel/proc.h"

uint64 sys_sysinfo(void) {
  // get the sysinfo struct from user space. us(user space)
  uint64 usInfoPtr;
  if (argaddr(0, &usInfoPtr) < 0) {
    printf("sys_sysinfo: invalid pointer\n");
  }

  // ks(kernel space)
  struct sysinfo ksInfo;

  ksInfo.freemem = getFreeMemSize();
  ksInfo.nproc = getActiveProcCount();

  // using copyout to send the sysinfo to user space
  int ret =
      copyout(myproc()->pagetable, usInfoPtr, (char*)&ksInfo, sizeof(ksInfo));
  if (ret < 0) {
    printf("sys_sysinfo: copyout failed\n");
    return -1;
  }

  return 0;
}