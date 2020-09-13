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
    //confere se a barreira já foi usada anteriormente, se sim, "reseta barreira"
    if(b->counter == b->n){
        b->counter = 0;
    }

    b->counter++;

    while(b->counter < b->n) {
        // enquanto o número de threads na barreira nao chega ao máximo espera
        pthread_cond_wait(&(b->barreira_cond), &(b->mutex));
    }

    // quando todos estão nela, libera e destrava o mutex
    pthread_cond_broadcast(&(b->barreira_cond));
    pthread_mutex_unlock(&(b->mutex));
}
