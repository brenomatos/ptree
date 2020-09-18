#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <sys/time.h>
#include <pthread.h>
#define MAX  10000


typedef long TipoChave;

typedef struct TipoRegistro {
  TipoChave Chave;
  /* outros componentes */
} TipoRegistro;



typedef struct TipoNo * TipoApontador;

typedef struct TipoNo {
  TipoRegistro Reg;
  TipoApontador Esq, Dir;
  pthread_cond_t cond;
  pthread_mutex_t mutex;
  int reader_counter;
  int is_locked;
} TipoNo;

typedef TipoApontador TipoDicionario;

typedef struct ThreadParams {
  int id;
  TipoNo * Dicionario;
  TipoChave* vetor;
} ThreadParams;


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

