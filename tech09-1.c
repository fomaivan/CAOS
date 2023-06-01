#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>

volatile sig_atomic_t sigint = 0;
volatile sig_atomic_t sigterm = 0;
volatile sig_atomic_t sigusr_first = 0;
volatile sig_atomic_t sigusr_second = 0;

void process_sigusr_first(int signum) {
  sigusr_first = 1;
}

void process_sigusr_second(int signum) {
  sigusr_second = 1;
}

void process_sigint(int signum) {
  sigint = 1;
}

void process_sigterm(int signum) {
  sigterm = 1;
}

int main() {
  struct sigaction sigint_action = {};
  sigint_action.sa_handler = process_sigint;
  sigaction(SIGINT, &sigint_action, NULL);

  struct sigaction sigterm_action = {};
  sigterm_action.sa_handler = process_sigterm;
  sigaction(SIGTERM, &sigterm_action, NULL);

  struct sigaction sigusr_first_action = {};
  sigusr_first_action.sa_handler = process_sigusr_first;
  sigaction(SIGUSR1, &sigusr_first_action, NULL);


  struct sigaction sigusr_second_action = {};
  sigusr_second_action.sa_handler = process_sigusr_second;
  sigaction(SIGUSR2, &sigusr_second_action, NULL);


  printf("%d\n", getpid());
  fflush(stdout);

  sig_atomic_t num;
  scanf("%d", &num);

  while (true) {
    if (sigterm || sigint) {
      break;
    }
    if (sigusr_first) {
      printf("%d\n", ++num);
      sigusr_first = 0;
      fflush(stdout);
    }
    if (sigusr_second) {
      printf("%d\n", num *= -1);
      sigusr_second = 0;
      fflush(stdout);
    }
    pause();
  }
  return 0;
}