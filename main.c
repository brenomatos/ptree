#include "ptree.h"
#include "barreira.h"
#include <time.h>

#define NUM_THREADS 20

int LEN = MAX/NUM_THREADS;

TBarreira bar;


void *tree_thread(void *parameters){//}, void* vetor){
  TipoRegistro x;
  int i, j, k, n;

  ThreadParams params = *((ThreadParams *)parameters);

  int id = params.id;
  TipoNo *Dicionario = params.Dicionario;
  TipoChave* vetor = params.vetor;
  barreira(&bar);

  /* Insere cada chave na arvore e testa sua integridade apos cada insercao */
  for(i = (id-1)*LEN; i < id*LEN; i++){
    x.Chave = vetor[i];
    Insere(x, &Dicionario);
  }

  barreira(&bar);

  /* Retira uma chave aleatoriamente e realiza varias pesquisas */
  for(i = 0; i < LEN; i++){
    k = (int) ((double)MAX*rand()/(RAND_MAX+1.0));
    n = vetor[k % LEN + (id-1)*LEN];
    x.Chave = n;
    Retira(x, &Dicionario);
    for (j = 0; j < MAX; j++)
      { x.Chave = vetor[((int) ((double)MAX*rand()/(RAND_MAX+1.0)))% LEN + (id-1)*LEN];
        // printf("%d %d\n",id, ((int)((double)MAX*rand()/(RAND_MAX+1.0)))% LEN + (id-1)*LEN);
        if (x.Chave != n)
        {
          Pesquisa(&x, &Dicionario);
        }
      }
    x.Chave = n;
    Insere(x, &Dicionario);
  }

  barreira(&bar);
  /* Retira a raiz da arvore ate que ela fique vazia */
  for (i = (id-1)*LEN; i < id*LEN; i++){
    x.Chave = Dicionario->Reg.Chave;
    Retira(x, &Dicionario);
  }

  return NULL;
}


int main(int argc, char *argv[]){
  pthread_t threads[NUM_THREADS];
  initBarreira(&bar, NUM_THREADS);
  struct timeval t; TipoNo *Dicionario;
  TipoChave vetor[MAX];
  int i;

  Inicializa(&Dicionario);
  /* Gera uma permuta��o aleatoria de chaves entre 1 e MAX */
  for (i = 0; i < MAX; i++) vetor[i] = i+1;
  gettimeofday(&t,NULL);
  srand((unsigned int)t.tv_usec);
  Permut(vetor,MAX-1);

  ThreadParams params[NUM_THREADS];
  for (i = 1; i < NUM_THREADS+1; i++)
  {
      params[i - 1].id = i;
      params[i - 1].Dicionario = Dicionario;
      params[i - 1].vetor = vetor;
      pthread_create(&threads[i - 1], NULL, tree_thread, (void *)&(params[i - 1]));
  }
  for (i = 0; i < NUM_THREADS; i++)
  {
      pthread_join(threads[i], NULL);
  }
}
