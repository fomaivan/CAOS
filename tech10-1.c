#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

volatile sig_atomic_t end = 0;
volatile sig_atomic_t accepting = 0;
volatile sig_atomic_t loop = 0;

void signal_handler(int sig_num) {
  if (sig_num == SIGTERM || sig_num == SIGINT) {
    end = 1;
    if (!loop || accepting) {
      exit(0);
    }
  }
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    return 1;
  }

  struct sigaction sig_int_struct;
  sig_int_struct.sa_handler = signal_handler;
  sigemptyset(&sig_int_struct.sa_mask);
  sig_int_struct.sa_flags = 0;
  sigaction(SIGINT, &sig_int_struct, NULL);

  struct sigaction sig_term_struct;
  sig_term_struct.sa_handler = signal_handler;
  sigemptyset(&sig_term_struct.sa_mask);
  sig_term_struct.sa_flags = 0;
  sigaction(SIGTERM, &sig_term_struct, NULL);

  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in serv_sock_addr;
  serv_sock_addr.sin_family = AF_INET;
  uint16_t port;
  sscanf(argv[1], "%" SCNd16 "", &port);
  serv_sock_addr.sin_port = htons(port);
  serv_sock_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  if (bind(sock_fd, (struct sockaddr*)&serv_sock_addr, sizeof(serv_sock_addr)) < 0) {
    return 1;
  }

  if (listen(sock_fd, SOMAXCONN) < 0) {
    return 1;
  }

  loop = 1;

  const char query_format[] = "GET %s HTTP/1.1";
  const char ok_response[] = "HTTP/1.1 200 OK\n";
  const char not_found_response[] = "HTTP/1.1 404 Not Found\n";
  const char forbidden_response[] = "HTTP/1.1 403 Forbidden\n";

  while (end == 0) {
    accepting = 1;
    int cl_sock_fd = accept(sock_fd, NULL, NULL);
    accepting = 0;
    if (cl_sock_fd < 0) {
      continue;
    }

    FILE* client_file = fdopen(cl_sock_fd, "r");
    char file_name[4096];
    memset(file_name, 4096, '\0');
    if (fscanf(client_file, query_format, file_name) <= 0) {
      continue;
    }

    if (argv[2][strlen(argv[2]) - 1] != '/') {
      argv[2][strlen(argv[2]) + 1] = '\0';
      argv[2][strlen(argv[2])] = '/';
    }

    char full_file_name[4096];
    memset(full_file_name, '\0', 4096);
    memcpy(full_file_name, argv[2], strlen(argv[2]));
    strcat(full_file_name, file_name);

    if (access(full_file_name, F_OK) != 0) {
      write(cl_sock_fd, not_found_response, strlen(not_found_response));
      continue;
    }

    if (access(full_file_name, R_OK) != 0) {
      write(cl_sock_fd, forbidden_response, strlen(forbidden_response));
      continue;
    }

    FILE* input_file = fopen(full_file_name, "r");
    if (input_file == NULL) {
      write(cl_sock_fd, not_found_response, strlen(not_found_response));
      continue;
    }

    write(cl_sock_fd, ok_response, strlen(ok_response));

    /// writing size
    char size_str[40];
    fseek(input_file, 0, SEEK_END);
    size_t file_size = ftell(input_file);
    sprintf(size_str, "Content-Length: %ld\n\n", file_size);
    write(cl_sock_fd, size_str, strlen(size_str));

    /// writing file
    fseek(input_file, 0, SEEK_SET);
    char buff[4096];
    int temp_read;
    while ((temp_read = fread(buff, sizeof(char), 4096, input_file)) > 0) {
      write(cl_sock_fd, buff, temp_read);
    }

    /// closing
    fclose(input_file);
    fclose(client_file);
    shutdown(cl_sock_fd, SHUT_RDWR);
    close(cl_sock_fd);
  }
  return 0;
}