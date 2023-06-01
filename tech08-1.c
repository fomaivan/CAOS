#define _GNU_SOURCE
#include <stdio.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
  int fds_in[2];
  int fds_out[2];

  int num = 2;

  pipe2(fds_in, O_NONBLOCK);
  pipe2(fds_out, O_NONBLOCK);

  for (int i = 0; i < num; i++) {
    pid_t pid = fork();
    if (pid == 0) {
      if (i > 0) {
        dup2(fds_out[0], 0);
        close(fds_out[0]);
      }
      if (i < num - 1) {
        dup2(fds_in[1], 1);
        close(fds_in[1]);
      }
      execlp(argv[i + 1], argv[i + 1], NULL);
    } else {
      wait(0);
      if (i < num - 1) {
        char buff[4096];
        int size;
        while((size = read(fds_in[0], buff, sizeof(buff))) > 0) {
          write(fds_out[1], buff, size);
        }
      }
    }
  }
  close(fds_in[0]);
  close(fds_in[1]);
  close(fds_out[0]);
  close(fds_out[1]);
}