#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {
  char* file_name = argv[1];
  struct stat st;
  int file_descriptor = open(file_name, O_RDONLY);
  if (file_descriptor == -1) {
    return 0;
  }
  if (fstat(file_descriptor, &st) == -1) {
    close(file_descriptor);
    return 0;
  }
  size_t size_of_file = st.st_size;
  char* mem_ptr = mmap(NULL, size_of_file, PROT_READ, MAP_SHARED, file_descriptor, 0);
  if (MAP_FAILED == mem_ptr) {
    return 0;
  }
  uint32_t curr_ptr = 0;
  while (true) {
    if (curr_ptr >= size_of_file) {
      break;
    }
    int value = *(int*)(curr_ptr + mem_ptr);
    printf("%d\n", value);
    curr_ptr = *(uint32_t*)(4 + mem_ptr + curr_ptr);
    if (curr_ptr == 0) {
      break;
    }
  }
  munmap(mem_ptr, size_of_file);
  close(file_descriptor);
}