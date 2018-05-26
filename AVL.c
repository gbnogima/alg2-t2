#include "AVL.h"

void cria_avl (Arvore *T) {
	T->raiz = NULL;
}

int IsEmpty (Arvore *T) {
	if (T->raiz == NULL)
		return 1;
	else return 0;
}


int altura (no *r) {
   if (r == NULL) 
      return -1; // altura da árvore vazia
   else {
      int he = altura (r->esq);
      int hd = altura (r->dir);
      if (he < hd) return hd + 1;
      else return he + 1;
   }
}


void EE(no **r) {
     no *pai=*r;
     no *filho=pai->esq;
     pai->esq=filho->dir;
     filho->dir=pai;
     pai->fb=0;
     filho->fb=0;
     *r=filho;
}

void DD(no **r) {
     no *pai=*r;
     no *filho=pai->dir;
     pai->dir=filho->esq;
     filho->esq=pai;
     pai->fb=0;
     filho->fb=0;
     *r=filho;
}

void ED(no **r) {
     no *pai=*r;
     no *filho=pai->esq;
     no *neto=filho->dir;
     pai->esq=neto->dir;
     filho->dir=neto->esq;
     neto->esq=filho;
     neto->dir=pai;
     switch (neto->fb) {
            case -1:
                 pai->fb=1;
                 filho->fb=0;
                 break;
            case 0:
                 pai->fb=0;
                 filho->fb=0;
                 break;
            case 1:
                 pai->fb=0;
                 filho->fb=-1;
                 break;
     }
     neto->fb=0;
     *r=neto;
}

void DE(no **r) {
     no *pai=*r;
     no *filho=pai->dir;
     no *neto=filho->esq;
     filho->esq=neto->dir;
     pai->dir=neto->esq;
     neto->esq=pai;
     neto->dir=filho;
     switch (neto->fb) {
            case -1:
                 pai->fb=0;
                 filho->fb=1;
                 break;
            case 0:
                 pai->fb=0;
                 filho->fb=0;
                 break;
            case 1:
                 pai->fb=-1;
                 filho->fb=0;
                 break;
     }
     neto->fb=0;
     *r=neto;
}

int aux_insere(no **p, char *chave, char *rrn, int *cresceu) {
    if (*p==NULL) {
       *p=(no*) malloc(sizeof(no));
       strcpy((*p)->chave, chave);
       strcpy((*p)->rrn, rrn);
       (*p)->fb=0;
       (*p)->esq=NULL;
       (*p)->dir=NULL;
       *cresceu=1;
       return 1;
    }
    else if (atoi(chave)==atoi((*p)->chave))
         return 0;
    else if (atoi(chave)<atoi((*p)->chave)) {
         if (aux_insere(&(*p)->esq,chave,rrn,cresceu)) {
            if (*cresceu) {
	           switch ((*p)->fb) {
	                  case -1:
	                       if ((*p)->esq->fb==-1)
	                          EE(p);
                           else ED(p);
	                       *cresceu=0;
	                       break;
	                  case 0:
	                       (*p)->fb=-1;
	                       *cresceu=1;
	                       break;
                      case 1:
	                       (*p)->fb=0;
	                       *cresceu=0;
	                       break;
               }
            }
            return 1;
         }
         else return 0;
    }
    else {
         if (aux_insere(&(*p)->dir,chave,rrn,cresceu)) {
            if (*cresceu) {
	           switch ((*p)->fb) {
	                  case -1:
	                       (*p)->fb=0;
                           *cresceu=0;
	                       break;
                      case 0:
	                       (*p)->fb=1;
	                       *cresceu=1;
	                       break;
                      case 1:
	                       if ((*p)->dir->fb==1)
	                          DD(p);
                           else DE(p);
	                       *cresceu=0;
	                       break;
               }
            }
            return 1;
         }
         else return 0;
    }
}

int inserir_avl(no **p, char *chave, char *rrn) {
  int cresceu;
  return aux_insere(p, chave, rrn, &cresceu);
}