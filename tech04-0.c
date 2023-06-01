#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char * argv[]) {
  int64_t sum = 0;
  char buffer[4096];
  while (fgets(buffer, 4096, stdin)) {
    struct stat st;
    buffer[strlen(buffer) - 1] = 0;
    if (lstat(buffer, &st) == -1) {
      continue;
    }
    if (S_ISREG(st.st_mode)) {
      sum += st.st_size;
    }
  }
  if (ferror(stdin)) {
    perror("Read fault");
    exit(1);
  }
  printf("%ld\n", sum);
}