#include "ptree.h"
#include "barreira.h"

int main(int argc, char *argv[])
{
  struct timeval t; TipoNo *Dicionario;
  TipoRegistro x; TipoChave vetor[MAX];
  int i, j, k, n;
  TBarreira bar;
  initBarreira(&bar, 1);

  Inicializa(&Dicionario);
  /* Gera uma permuta��o aleatoria de chaves entre 1 e MAX */
  for (i = 0; i < MAX; i++) vetor[i] = i+1;
  gettimeofday(&t,NULL);
  srand((unsigned int)t.tv_usec);
  Permut(vetor,MAX-1);

  /* Insere cada chave na arvore e testa sua integridade apos cada insercao */
  for (i = 0; i < MAX; i++)
    { x.Chave = vetor[i];
      Insere(x, &Dicionario);
      printf("Inseriu chave: %ld\n", x.Chave);
     Testa(Dicionario);
    }

  barreira(&bar);

  /* Retira uma chave aleatoriamente e realiza varias pesquisas */
  for (i = 0; i <= MAX; i++)
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
  for (i = 0; i < MAX; i++)
    { x.Chave = Dicionario->Reg.Chave;
      Retira(x, &Dicionario);
      Testa(Dicionario);
      printf("Retirou chave: %ld\n", x.Chave);
    }
  return 0;
}