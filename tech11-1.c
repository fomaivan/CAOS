#include <string.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <inttypes.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

enum { MAX_CONNECTIONS = 65536, MAX_EVENTS = 65536 };

volatile sig_atomic_t end = 0;

void signal_handler(int sig_num) {
  if (sig_num == SIGTERM) {
    end = 1;
  }
}

typedef struct {
  int fd;
  int closed;
  int size;
} data_t;

ssize_t Write(int fd, void* buf, size_t sz) {
  while (sz > 0) {
    int tmp = write(fd, buf, sz);
    if (tmp < 0) {
      return tmp;
    }
    buf += tmp;
    sz -= tmp;
  }
  return sz;
}

void ProcessClnt(struct epoll_event *event) {
  data_t* data_ptr = (data_t*)(event->data.ptr);
  if (event->events & EPOLLIN) {
    char buff[4096];
    ssize_t tmp = read(data_ptr->fd, buff, 4096);
    if (tmp == 0) {
      shutdown(data_ptr->fd, SHUT_RDWR);
      close(data_ptr->fd);
      data_ptr->closed = 1;
    }
    for (int i = 0; i < tmp; ++i) {
      buff[i] = toupper(buff[i]);
    }
    Write(data_ptr->fd, buff, tmp);
  }
}

data_t* CreateEventData(int epoll_fd, int fd) {
  fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
  data_t* data = malloc(sizeof(data_t));
  data->fd = fd;
  data->closed = 0;

  struct epoll_event event_ready_read;
  event_ready_read.events = EPOLLIN;
  event_ready_read.data.ptr = data;
  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event_ready_read) == -1) {
    exit(1);
  }
  return data;
}

data_t* LstnPort(const char* port_str, int* lstng_sock, int* epoll_fd) {
  *lstng_sock = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in serv_sock_addr;
  serv_sock_addr.sin_family = AF_INET;
  uint16_t port;
  sscanf(port_str, "%" SCNd16 "", &port);
  serv_sock_addr.sin_port = htons(port);
  serv_sock_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  if (bind(*lstng_sock, (struct sockaddr*)&serv_sock_addr, sizeof(serv_sock_addr)) < 0) {
    exit(1);
  }
  if (listen(*lstng_sock, SOMAXCONN) < 0) {
    exit(1);
  }
  if ((*epoll_fd = epoll_create1(0)) < 0) {
    exit(1);
  }
  return CreateEventData(*epoll_fd, *lstng_sock);
}

data_t* ProcessLstnSock(int epoll_fd, int lstng_sock, struct epoll_event* event) {
  int clnt_sock = accept(lstng_sock, NULL, NULL);
  if (clnt_sock < 0) {
    return NULL;
  }
  return CreateEventData(epoll_fd, clnt_sock);
}

int main(int argc, char** argv) {
  if (argc != 2) {
    return 1;
  }
  struct sigaction sig_term_struct = {};
  sig_term_struct.sa_handler = signal_handler;
  sigemptyset(&sig_term_struct.sa_mask);
  sigaction(SIGTERM, &sig_term_struct, NULL);

  int listenning_sock;
  int epoll_fd;
  data_t* listen_event = LstnPort(argv[1], &listenning_sock, &epoll_fd);

  data_t* conn[MAX_CONNECTIONS];
  int conn_num = 0;
  struct epoll_event pending[MAX_EVENTS];

  while (!end) {
    int num = epoll_wait(epoll_fd, pending, MAX_EVENTS, -1);
    for (int i = 0; i < num; i++) {
      data_t* tmp_data = (data_t*)(pending[i].data.ptr);
      if (tmp_data->fd == listenning_sock) {
        data_t* tmp = ProcessLstnSock(epoll_fd, listenning_sock, &(pending[i]));
        if (tmp != NULL) {
          conn[conn_num++] = tmp;
        }
      } else {
        ProcessClnt(&(pending[i]));
      }
    }
  }
  close(listenning_sock);
  close(epoll_fd);

  for (int i = 0; i < conn_num; i++) {
    if (!conn[i]->closed) {
      shutdown(conn[i]->fd, SHUT_RDWR);
      close(conn[i]->fd);
    }
    free(conn[i]);
  }
}