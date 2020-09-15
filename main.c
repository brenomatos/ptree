#include "ptree.h"
#include "barreira.h"

#define NUM_THREADS 2

int LEN = MAX/NUM_THREADS;

TBarreira bar;


void *tree_thread(void *parameters){//}, void* vetor){
  TipoRegistro x;
  int i, j, k, n;

  ThreadParams params = *((ThreadParams *)parameters);

  int id = params.id;
  TipoNo *Dicionario = params.Dicionario;
  TipoChave* vetor = params.vetor;

  /* Insere cada chave na arvore e testa sua integridade apos cada insercao */
  for (i = (id-1)*LEN; i < id*LEN; i++)
    { x.Chave = vetor[i];
      Insere(x, &Dicionario);
      printf("Inseriu chave: %ld\n", x.Chave);
     Testa(Dicionario);
    }

  barreira(&bar);

  /* Retira uma chave aleatoriamente e realiza varias pesquisas */
  for (i = (id-1)*LEN; i < id*LEN; i++)
    { k = (int) (10.0*rand()/(RAND_MAX+1.0));
      n = vetor[k];
      x.Chave = n;
      Retira(x, &Dicionario);
      Testa(Dicionario);
      printf("Retirou chave: %ld\n", x.Chave);
      for (j = 0; j < MAX; j++)
        { x.Chave = vetor[(int) (10.0*rand()/(RAND_MAX+1.0))];
          if (x.Chave != n)
          { printf("Pesquisando chave: %ld\n", x.Chave);
            Pesquisa(&x, &Dicionario);
          }
        }
      x.Chave = n;
      Insere(x, &Dicionario);
      printf("Inseriu chave: %ld\n", x.Chave);
      Testa(Dicionario);
    }

  barreira(&bar);
  /* Retira a raiz da arvore ate que ela fique vazia */
  for (i = (id-1)*LEN; i < id*LEN; i++){
      x.Chave = Dicionario->Reg.Chave;
      Retira(x, &Dicionario);
      Testa(Dicionario);
      printf("Retirou chave: %ld\n", x.Chave);
    }
  return NULL;
}


int main(int argc, char *argv[])
{
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
        printf("%d\n", i);
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
