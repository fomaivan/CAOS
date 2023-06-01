#define _GNU_SOURCE
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>

int main(int argc, char* argv[]) {
  char* name_of_file = argv[1];
  char* string_to_find = argv[2];
  int size_of_string = strlen(string_to_find);
  int file_descriptor = open(name_of_file, O_RDONLY);
  if (file_descriptor == -1) {
    return 0;
  }
  struct stat st;
  if (fstat(file_descriptor, &st) == -1) {
    close(file_descriptor);
    return 0;
  }
  int size_of_file = st.st_size;
  char* file_ptr = mmap(NULL, size_of_file, PROT_READ, MAP_SHARED, file_descriptor, 0);
  char* curr = memmem(file_ptr, size_of_file, string_to_find, size_of_string);
  while (curr != NULL) {
    printf("%ld ", curr - file_ptr);
    curr++;
    curr = memmem(curr, (size_of_file + file_ptr - curr), string_to_find, size_of_string);
  }
  printf("\n");
  munmap(file_ptr, size_of_file);
  close(file_descriptor);
}