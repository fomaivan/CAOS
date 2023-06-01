#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
  char prog[5000];
  char buff[5000];
  fgets(buff, sizeof(buff), stdin);
  char* slash_pos = strchr(buff, '\n');
  if (sizeof(buff) == 0) {
    return 0;
  }
  if (slash_pos) {
    *slash_pos = '\0';
  }
  int fd = open("tmp.c", O_CREAT | O_WRONLY | O_TRUNC, 0644);
  snprintf(prog, sizeof(prog), "#include <stdio.h>\n int main() { int a = (%s); printf(\"%%d\", a); return 0; }", buff);
  write(fd, prog, strnlen(prog, sizeof(prog)));
  close(fd);
  pid_t pid = fork();
  if (pid == 0) {
    execlp("gcc", "gcc", "-o", "tmp", "tmp.c", NULL);
    return 0;
  } else {
    int temp;
    waitpid(pid, &temp, 0);
  }
  pid = fork();
  if (pid == 0) {
    execlp("./tmp", "./tmp", NULL);
    return 0;
  }
  int temp;
  waitpid(pid, &temp, 0);
}