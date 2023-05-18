#include <sys/syscall.h>

long syscall(long number, ...);

void _start() {
  char data[10];
  while (syscall(SYS_read, 0, data, 1)) {
    //int temp_size = syscall(SYS_read, 0, data, size);
    syscall(SYS_write, 1, data, 1);
  }
  syscall(SYS_exit, 0);
}