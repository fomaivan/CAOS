#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>


int main() {
  int num = 1;
  pid_t id = fork();
  while (!id) {
    num++;
    id = fork();
  }
  if (id > 0) {
    int temp = 0;
    waitpid(id, &temp, 0);
  }
  if (-1 == id) {
    printf("%d\n", num);
  }
}