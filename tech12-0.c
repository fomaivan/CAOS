#include <pthread.h>
#include <stdio.h>


struct Node {
  volatile double *arr_;
  pthread_mutex_t *mutex_;
  int n_;
  int k_;
  int i_;
};

void* thread(void* arg_) {
  int temp;
  struct Node *arg = (struct Node*) arg_;
  for (size_t i = 0; i < arg->n_; i++) {
    pthread_mutex_lock(arg->mutex_);
    temp = arg->i_;
    arg->arr_[temp] += 1;

    temp = ((arg->i_ + 1) % arg->k_);
    arg->arr_[temp] += 1.01;

    temp = (arg->i_ - 1) % arg->k_;
    if (temp < 0) {
      temp += arg->k_;
    }
    arg->arr_[temp] += 0.99;
    pthread_mutex_unlock(arg->mutex_);
  }
  return NULL;
}


int main(int argc, char* argv[]) {
  if (argc != 3) {
    return 1;
  }
  int num, k;
  sscanf(argv[1], "%d", &num);
  sscanf(argv[2], "%d", &k);

  struct Node args[k];
  volatile double arr[k];
  pthread_mutex_t mutex;
  pthread_mutex_init(&mutex, NULL);

  for (int i = 0; i < k; ++i) {
    arr[i] = 0;
    args[i].arr_ = arr;
    args[i].mutex_ = &mutex;
    args[i].k_ = k;
    args[i].n_ = num;
    args[i].i_ = i;
  }

  pthread_t threads[k];
  for (size_t i = 0; i < k; ++i) {
    pthread_create(&threads[i], NULL, (void*)thread, &args[i]);
  }
  for (size_t i = 0; i < k; i++) {
    pthread_join(threads[i], NULL);
  }
  for (size_t i = 0; i < k; ++i) {
    printf("%.10g\n", arr[i]);
  }
  pthread_mutex_destroy(&mutex);
  return 0;
}