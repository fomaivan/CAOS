#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>

int Algo(void* u) {
  int result = 0;
  int tmp;
  while (EOF != scanf("%d", &tmp)) {
    result += tmp;
  }
  return result;
}

int main(int argc, char* argv[]) {
  int result_sum = 0, num;
  sscanf(argv[1], "%d", &num);
  pthread_t threads[num];
  pthread_attr_t* p_attr = malloc(sizeof(pthread_attr_t));
  pthread_attr_init(p_attr);
  pthread_attr_setstacksize(p_attr, PTHREAD_STACK_MIN);
  pthread_attr_setguardsize(p_attr, 0);
  for (int i = 0; i < num; i++) {
    pthread_create(&threads[i], NULL, (void*) Algo, NULL);
  }
  pthread_attr_destroy(p_attr);
  for (int i = 0; i < num; i++) {
    int sum;
    pthread_join(threads[i], (void**)&sum);
    result_sum += sum;
  }
  free(p_attr);
  printf("%d\n", result_sum);
}