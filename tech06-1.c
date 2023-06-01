#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  int num = atoi(argv[1]);
  pid_t id;
  int i = 0;
  while (i < num) {
    id = fork();
    if (id != 0) {
      int temp = 0;
      waitpid(id, &temp, 0);
      if (i != 0) {
        printf("%d ", num - i);
      } else {
        printf("%d\n", num - i);
      }
      break;
    }
    i++;
  }
}