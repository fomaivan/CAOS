#include <stdio.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
  char *command = argv[1];
  char *file_name = argv[2];

  int in = open(file_name, O_RDONLY);
  dup2(in, 0);
  close(in);

  int pair[2];
  pipe(pair);

  pid_t pid = fork();
  if (0 == pid) {
    dup2(pair[1], 1);
    close(pair[1]);
    execlp(command, command, NULL);
  } else {
    close(pair[1]);
    long long unsigned int total_count = 0;
    ssize_t count;
    char buffer[4096];
    while ((count = read(pair[0], buffer, sizeof(buffer))) > 0) {
      total_count += count;
    }
    waitpid(pid, 0, 0);
    printf("%llu\n", total_count);
  }
}