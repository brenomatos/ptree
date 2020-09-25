#include "ptree.h"

void Pesquisa(TipoRegistro *x, TipoApontador *p){
  if (*p == NULL){
    printf("Erro: Registro nao esta presente na arvore\n");
    return;
  }

  pthread_mutex_lock(&((*p)->mutex));
  if((*p)->is_locked){
    pthread_cond_wait(&((*p)->cond), &((*p)->mutex));
  }
  (*p)->reader_counter++;
  pthread_mutex_unlock(&((*p)->mutex));

  if (x->Chave < (*p)->Reg.Chave){
    Pesquisa(x, &(*p)->Esq);
    pthread_mutex_lock(&((*p)->mutex));
    (*p)->reader_counter--;
    if((*p)->reader_counter == 0){
      pthread_cond_broadcast(&((*p)->cond));
    }
    pthread_mutex_unlock(&((*p)->mutex));
    return;
  }

  if (x->Chave > (*p)->Reg.Chave){
    Pesquisa(x, &(*p)->Dir);
  }else{
    *x = (*p)->Reg;
  }
  pthread_mutex_lock(&((*p)->mutex));
  (*p)->reader_counter--;
  if((*p)->reader_counter == 0){
    pthread_cond_broadcast(&((*p)->cond));
  }
  pthread_mutex_unlock(&((*p)->mutex));
}

void Insere(TipoRegistro x, TipoApontador *p){
  if (*p == NULL){
    *p = (TipoApontador)malloc(sizeof(TipoNo));
    (*p)->Reg = x;
    (*p)->Esq = NULL;
    (*p)->Dir = NULL;
    (*p)->mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    (*p)->cond = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
    (*p)->is_locked = 0;
    (*p)->reader_counter = 0;
    return;
  }
  if (x.Chave < (*p)->Reg.Chave){
    if((*p)->Esq == NULL){
      pthread_mutex_lock(&((*p)->mutex));
      if((*p)->is_locked || (*p)->reader_counter > 0){
        pthread_cond_wait(&((*p)->cond), &((*p)->mutex));
      }
      (*p)->is_locked = 1;
      pthread_mutex_unlock(&((*p)->mutex));

      Insere(x, &(*p)->Esq);

      pthread_mutex_lock(&((*p)->mutex));
      (*p)->is_locked = 0;
      pthread_cond_broadcast(&((*p)->cond));
      pthread_mutex_unlock(&((*p)->mutex));

    }else{
      Insere(x, &(*p)->Esq);
    }
    return;
  }
  if (x.Chave > (*p)->Reg.Chave){
    if((*p)->Dir == NULL){
      pthread_mutex_lock(&((*p)->mutex));
      if((*p)->is_locked || (*p)->reader_counter > 0){
        pthread_cond_wait(&((*p)->cond), &((*p)->mutex));
      }
      (*p)->is_locked = 1;
      pthread_mutex_unlock(&((*p)->mutex));

      Insere(x, &(*p)->Dir);

      pthread_mutex_lock(&((*p)->mutex));
      (*p)->is_locked = 0;
      pthread_cond_broadcast(&((*p)->cond));
      pthread_mutex_unlock(&((*p)->mutex));

    }else{
      Insere(x, &(*p)->Dir);
    }
  }
  else printf("Erro : Registro ja existe na arvore\n");
}

void Inicializa(TipoApontador *Dicionario){
  *Dicionario = NULL;
}

void Antecessor(TipoApontador q, TipoApontador *r){
  pthread_mutex_lock(&((*r)->mutex));
  while((*r)->is_locked || (*r)->reader_counter > 0){
    pthread_cond_wait(&((*r)->cond), &((*r)->mutex));
  }
  (*r)->is_locked = 1;
  pthread_mutex_unlock(&((*r)->mutex));

  if ((*r)->Dir != NULL){
    pthread_mutex_lock(&((*r)->mutex));
    (*r)->is_locked = 0;
    pthread_cond_broadcast(&((*r)->cond));
    pthread_mutex_unlock(&((*r)->mutex));

    Antecessor(q, &(*r)->Dir);
    return;
  }
  q->Reg = (*r)->Reg;
  q = *r;
  *r = (*r)->Esq;
  free(q);
}

void Retira(TipoRegistro x, TipoApontador *p){
  TipoApontador Aux;
  if (*p == NULL){
    printf("Erro : Registro nao esta na arvore\n");
    return;
  }
  pthread_mutex_lock(&((*p)->mutex));
  while((*p)->is_locked || (*p)->reader_counter > 0){
    pthread_cond_wait(&((*p)->cond), &((*p)->mutex));
  }
  (*p)->is_locked = 1;
  pthread_mutex_unlock(&((*p)->mutex));

  if (x.Chave < (*p)->Reg.Chave) {
    pthread_mutex_lock(&((*p)->mutex));
    (*p)->is_locked = 0;
    pthread_cond_broadcast(&((*p)->cond));
    pthread_mutex_unlock(&((*p)->mutex));

    Retira(x, &(*p)->Esq);
    return;
  }
  if (x.Chave > (*p)->Reg.Chave) {
    pthread_mutex_lock(&((*p)->mutex));
    (*p)->is_locked = 0;
    pthread_cond_broadcast(&((*p)->cond));
    pthread_mutex_unlock(&((*p)->mutex));

    Retira(x, &(*p)->Dir);
    return;
  }

  if ((*p)->Dir == NULL){
    Aux = *p;
    *p = (*p)->Esq;
    free(Aux);
    return;
  }
  if ((*p)->Esq != NULL){
    Antecessor(*p, &(*p)->Esq);
    pthread_mutex_lock(&((*p)->mutex));
    (*p)->is_locked = 0;
    pthread_cond_broadcast(&((*p)->cond));
    pthread_mutex_unlock(&((*p)->mutex));
    return;
  }
  Aux = *p;
  *p = (*p)->Dir;
  free(Aux);
}

void Central(TipoApontador p)
{ if (p == NULL) return;
  Central(p->Esq);
  printf("%ld\n", p->Reg.Chave);
  Central(p->Dir);
}

void TestaI(TipoNo *p, int pai)
{ if (p == NULL) return;
  if (p->Esq != NULL)
  { if (p->Reg.Chave < p->Esq->Reg.Chave)
    { printf("Erro: Pai %ld menor que filho a esquerda %ld\n", p->Reg.Chave,
             p->Esq->Reg.Chave);
      exit(1);
    }
  }
  if (p->Dir != NULL)
  { if (p->Reg.Chave > p->Dir->Reg.Chave)
    { printf("Erro: Pai %ld maior que filho a direita %ld\n",  p->Reg.Chave,
             p->Dir->Reg.Chave);
    exit(1);
    }
  }
  TestaI(p->Esq, p->Reg.Chave);
  TestaI(p->Dir, p->Reg.Chave);
}


void Testa(TipoNo *p)
{ if (p != NULL)
  TestaI(p, p->Reg.Chave);
}

double rand0a1() {
  double resultado=  (double) rand()/ RAND_MAX; /* Dividir pelo maior inteiro */
  if(resultado>1.0) resultado = 1.0;
  return resultado;
}

void Permut( TipoChave A[], int n) {
  int i,j; TipoChave b;
  for(i = n; i>0; i --)
    { j = (i * rand0a1());
      b = A[i];
      A[i] = A[j];
      A[j] = b;
    }
}

void print(TipoApontador *p){
  if(*p==NULL){
    return;
  }
  printf("%ld ", (*p)->Reg.Chave);
  print(&(*p)->Esq);
  print(&(*p)->Dir);
}