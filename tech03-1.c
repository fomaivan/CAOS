#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
  int input_file = open(argv[1], O_RDONLY);
  if (input_file == -1) {
    return 0;
  }
  u_int32_t next_position = 1;
  int num = 0;
  int i = 0;
  while (1) {
    read(input_file, &num, sizeof(int));
    if (num == 0 && i == 0) {
      close(input_file);
      return 0;
    }
    read(input_file, &next_position, sizeof(u_int32_t));
    printf("%d\n", num);
    i++;
    if (next_position == 0) {
      close(input_file);
      return 0;
    }
    lseek(input_file, next_position, SEEK_SET);
  }
}