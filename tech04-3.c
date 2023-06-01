#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h>

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
  char file_name[PATH_MAX];
  struct stat st;
  while (fgets(file_name, PATH_MAX, stdin)) {
    ExtractName(file_name);
    if (lstat(file_name, &st) == -1) {
      continue;
    }
    if (S_ISLNK(st.st_mode)) {
      char temp_file_name[PATH_MAX];
      char abs_path[PATH_MAX];
      char full_path[PATH_MAX];
      ssize_t length = readlink(file_name, temp_file_name, PATH_MAX);
      temp_file_name[length] = '\0';
      strcat(strcpy(full_path, dirname(file_name)), "/");
      strcat(full_path, temp_file_name);
      realpath(full_path, abs_path);
      printf("%s\n", abs_path);
      continue;
    }
    if (S_ISREG(st.st_mode)) {
      char new_name[PATH_MAX] = "link_to_";
      strcat(new_name, basename(file_name));
      symlink(file_name, new_name);
    }
  }
}