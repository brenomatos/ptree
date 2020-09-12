#include "barreira.h"

void initBarreira(TBarreira* b, int n){
    b->n = n;
    b->counter = 0;
    b->barreira_cond = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
    b->mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
}

void barreira(TBarreira* b){
    // trava o mutex
    pthread_mutex_lock(&(b->mutex));
    b->counter++;
    if (b->counter <= b->n) {
        // enquanto o número de threads
        // na barreira nao chega ao máximo espera
        pthread_cond_wait(&(b->barreira_cond), &(b->mutex));
    } else {
        // quando todos estão nela, libera
        b->counter == 0;
        pthread_cond_signal(&(b->barreira_cond));
    }
    // destrava o mutex
    pthread_mutex_unlock(&(b->mutex));
}