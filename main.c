#include "ptree.h"
#include "barreira.h"
#include <time.h>

#define NUM_THREADS 2

int LEN = MAX/NUM_THREADS;

TBarreira bar;
TipoNo *Dicionario;
pthread_mutex_t global_mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t time_mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;

void *tree_thread(void *parameters){
  //esta função será executada por todas as threads e inclui as 3 fases
  TipoRegistro x;
  int i, j, k, n;
  ThreadParams params = *((ThreadParams *)parameters);
  int id = params.id;
  TipoChave* vetor = params.vetor;

  //Insere cada chave na arvore
  //Cada thread irá inserir um subconjunto dos valores
  //essa divisão de conjunto é feita no limite desse for
  for(i = (id-1)*LEN; i < id*LEN; i++){
    x.Chave = vetor[i];
    /*
      no início, a árvore é vazia, então, foi usado um mutex global
      responsável por esperar até que o primeiro nó seja 
    */
    if(Dicionario == NULL) pthread_mutex_lock(&global_mutex);
    Insere(x, &Dicionario);
    pthread_mutex_unlock(&global_mutex);
  }

  barreira(&bar);
  //Retira uma chave aleatoriamente e realiza varias pesquisas
  for(i = 0; i < LEN; i++){
    //Cada thread irá considerar um subconjunto dos valores
    //essa divisão de conjunto é feita nessa seleção aleatória de valores
    k = (int) ((double)MAX*rand()/(RAND_MAX+1.0));
    n = vetor[k % LEN + (id-1)*LEN];
    x.Chave = n;
    Retira(x, &Dicionario);
    for (j = 0; j < LEN; j++){
      x.Chave = vetor[((int) ((double)MAX*rand()/(RAND_MAX+1.0)))% LEN + (id-1)*LEN];
      if (x.Chave != n)
      {
        Pesquisa(&x, &Dicionario);
      }
    }
    x.Chave = n;
    Insere(x, &Dicionario);
  }

  barreira(&bar);

  //Insere remove elementos da árvore
  //Cada thread irá remover um subconjunto dos valores
  //essa divisão de conjunto é feita no limite desse for
  for (i = (id-1)*LEN; i < id*LEN; i++){
    x.Chave = vetor[i % LEN + (id-1)*LEN];
    Retira(x, &Dicionario);
  }
  return NULL;
}

int main(int argc, char *argv[]){
  pthread_t threads[NUM_THREADS];
  initBarreira(&bar, NUM_THREADS);
  struct timeval t;
  TipoChave vetor[MAX];
  int i;

  Inicializa(&Dicionario);
  //Gera uma permutação aleatoria de chaves entre 1 e MAX
  for (i = 0; i < MAX; i++) vetor[i] = i+1;
  gettimeofday(&t,NULL);
  srand((unsigned int)t.tv_usec);
  Permut(vetor,MAX-1);

  ThreadParams params[NUM_THREADS];
  for (i = 1; i < NUM_THREADS+1; i++)
  {
      //preenche os parametros e as threads
      params[i - 1].id = i;
      params[i - 1].vetor = vetor;
      pthread_create(&threads[i - 1], NULL, tree_thread, (void *)&(params[i - 1]));
  }
  for (i = 0; i < NUM_THREADS; i++)
  {
      //espera que todas terminem
      pthread_join(threads[i], NULL);
  }
  //função que imprime a arvore. Nesse caso, como os nós foram removidos, não irá imprimir nada
  //caso aparecam os valores, significa que não foi removido corretamente
  Central(Dicionario);
  //função que testa a integridade da árvore
  Testa(Dicionario);
}
