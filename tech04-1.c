#include <unistd.h>
#include <limits.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

bool IsExec(struct stat* st) {
  if (!S_ISREG(st->st_mode) || !(st->st_mode & S_IXUSR)) {
    return false;
  }
  return true;
}

void ExtractName(char* str) {
  size_t i = 0;
  while (i < PATH_MAX && str[i] != '\n') {
    ++i;
  }
  if (i != PATH_MAX) {
    str[i] = '\0';
  }
}

int main() {
  char f_in[PATH_MAX];
  char f_interp[PATH_MAX];
  char check_exec[2];
  char check_elf[4];
  int file;
  struct stat st;
  struct stat interp_st;

  while (fgets(f_in, sizeof(f_in), stdin)) {
    ExtractName(f_in);
    file = open(f_in, O_RDONLY);
    if (file == -1 || fstat(file, &st) == -1 || !IsExec(&st)) {
      continue;
    }
    if (read(file, &check_exec, 2) >= 2 && check_exec[0] == '#' && check_exec[1] == '!') {
      read(file, f_interp, PATH_MAX);
      ExtractName(f_interp);
      if ((stat(f_interp, &interp_st) == -1) || !IsExec(&interp_st)) {
        printf("%s\n", f_in);
      }
    } else {
      lseek(file, 0, 0);
      if (read(file, &check_elf, 4) < 4 ||
          ((check_elf[0] != 0x7f) || (check_elf[1] != 0x45) || (check_elf[2] != 0x4c) || (check_elf[3] != 0x46))) {
        printf("%s\n", f_in);
      }
    }
  }
}