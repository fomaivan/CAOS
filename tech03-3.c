#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include <ctype.h>
#include <stdint.h>

int main(int argc, char* argv[]) {
  HANDLE input_file = CreateFileA(argv[1], GENERIC_READ, FILE_SHARE_READ, NULL,
                                  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (input_file == -1) {
    return 0;
  }
  int input_size = 1;
  uint32_t next_position = 1;
  int num = 0;
  while (1) {
    ReadFile(input_file, &num , sizeof(int), (LPDWORD) & input_size, NULL);
    if (input_size < sizeof(int)) {
      CloseHandle(input_file);
      return 0;
    }
    ReadFile(input_file, &next_position , sizeof(uint32_t), (LPDWORD) & input_size, NULL);
    if (input_size < sizeof(int)) {
      CloseHandle(input_file);
      return 0;
    }
    printf("%d\n", num);
    if (next_position == 0) {
      CloseHandle(input_file);
      return 0;
    }
    LARGE_INTEGER move_to;
    move_to.HighPart = 0;
    move_to.LowPart = next_position;
    SetFilePointerEx(input_file, move_to, NULL, FILE_BEGIN);
  }
}