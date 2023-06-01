#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <unistd.h>
#include <inttypes.h>

int main(int argc, char *argv[]) {
  uint16_t port = 0;
  sscanf(argv[2], "%" SCNd16 "", &port);
  struct sockaddr_in sockaddr;
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_port = htons(port);
  sockaddr.sin_addr.s_addr = inet_addr(argv[1]);

  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  connect(sock_fd, (const struct sockaddr*)&sockaddr, sizeof(sockaddr));

  int num;
  while(true) {
    if (scanf("%d", &num) <= 0) {
      break;
    }
    if (write(sock_fd, &num, sizeof(num)) <= 0) {
      break;
    }
    if (read(sock_fd, &num, sizeof(num)) <= 0) {
      break;
    }
    printf("%d\n", num);
  }
  shutdown(sock_fd, SHUT_RDWR);
  close(sock_fd);
  return 0;
}