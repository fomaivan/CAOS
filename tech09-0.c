#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>

volatile sig_atomic_t sigint = 0;
volatile sig_atomic_t sigterm = 0;

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

  printf("%d\n", getpid());
  fflush(stdout);

  sig_atomic_t counter = 0;
  while (true) {
    if(sigint) {
      counter++;
      sigint = 0;
    }
    if (sigterm) {
      printf("%u\n", counter);
      return 0;
    }
    pause();
  }
}