#include "barreira.h"

void initBarreira(TBarreira* b, int n){
    //preenche os campos do struct barreira, nesse struck há o contador, 
    //o número total de threads e o mutex e sua variável de condição
    b->n = n;
    b->counter = 0;
    b->barreira_cond = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
    b->mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
}

void barreira(TBarreira* b){
    // trava o mutex
    pthread_mutex_lock(&(b->mutex));
    //incrementa o número de threads que chegaram na barreira
    b->counter++;
    if(b->counter < b->n) {
        //enquanto o número de threads na barreira nao chega ao máximo espera
        pthread_cond_wait(&(b->barreira_cond), &(b->mutex));
    }else{
        //quando todos estão nela, libera e destrava o mutex
        pthread_cond_broadcast(&(b->barreira_cond));
        //reseta o número de threads na barreira
        b->counter = 0;
    }
    //libera o mutex
    pthread_mutex_unlock(&(b->mutex));
}
