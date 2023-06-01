#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
  char buffer[5000];
  char program[5000];
  fgets(buffer, sizeof(buffer), stdin);
  char* slash = strchr(buffer, '\n');
  if (sizeof(buffer) == 0) {
    return 0;
  }
  if (slash) {
    *slash = '\0';
  }
  snprintf(program, sizeof(program), "ans = %s; print(ans)", buffer);
  execlp("python3", "python3", "-c", program, NULL);
  perror("failed to exec");
  return 1;
}