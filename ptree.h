#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <sys/time.h>
#include <pthread.h>
#define MAX  2000


typedef long TipoChave;

typedef struct TipoRegistro {
  TipoChave Chave;
  /* outros componentes */
} TipoRegistro;



typedef struct TipoNo * TipoApontador;


typedef struct TipoNo {
  TipoRegistro Reg;
  TipoApontador Esq, Dir;
  //mutex e variáveis de condição para cada um dos nós da árvore
  pthread_cond_t cond;
  pthread_mutex_t mutex;
  //número de leitores
  int reader_counter;
  //indica se o nó pode ser alterado (insere/remove)
  int is_locked;
} TipoNo;

typedef TipoApontador TipoDicionario;

typedef struct ThreadParams {
  int id;
  TipoChave* vetor;
} ThreadParams;

//métodos da árvore
//assinatura original mantida
void Pesquisa(TipoRegistro *x, TipoApontador *p);
void Insere(TipoRegistro x, TipoApontador *p);
void Inicializa(TipoApontador *Dicionario);
void Antecessor(TipoApontador q, TipoApontador *r);
void Retira(TipoRegistro x, TipoApontador *p);
void Central(TipoApontador p);
void TestaI(TipoNo *p, int pai);
void Testa(TipoNo *p);
double rand0a1();
void Permut( TipoChave A[], int n);
void print(TipoApontador *p);
