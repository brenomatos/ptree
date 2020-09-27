#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

//struct que define a barreira e seus campos
struct TBarreira {
    int n;
    int counter;
    pthread_cond_t barreira_cond;
    pthread_mutex_t mutex;
};

typedef struct TBarreira TBarreira;

//métodos da barreira
void initBarreira(TBarreira* b, int n);
void barreira(TBarreira* b);