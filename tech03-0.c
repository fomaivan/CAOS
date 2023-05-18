#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>


int main(int argc, char* argv[]) {
  int exit_code = 0;
  int in = open(argv[1], O_RDONLY);
  if (in == -1) {
    exit_code = 1;
    goto exit_program;
  }
  int out_digits = open(argv[2], O_WRONLY | O_CREAT, 0640);
  int out_other = open(argv[3], O_WRONLY | O_CREAT, 0640);
  if (out_digits == -1 || out_other == -1) {
    exit_code = 2;
    goto exit_program;
  }
  ssize_t input_read;
  char input_byte;
  int out;
  while((input_read = read(in, &input_byte, sizeof(input_byte))) > 0) {
    out = ('0' <= input_byte && input_byte <= '9') ? out_digits : out_other;
    if (write(out, &input_byte, sizeof(input_byte)) == -1) {
      exit_code = 3;
      goto exit_program;
    }
  }
  if (input_read == -1) {
    exit_code = 3;
    goto exit_program;
  }

exit_program:
  close(in);
  close(out_digits);
  close(out_other);
  return exit_code;
}