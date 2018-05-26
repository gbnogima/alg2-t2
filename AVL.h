#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct bloco {
	char chave[4];
	char rrn[6];
	int fb;
	struct bloco *esq, *dir;
} no;

typedef struct {
	no *raiz;
} Arvore;


void cria_avl (Arvore *);

int IsEmpty (Arvore *);

int altura (no *);

void EE(no **);

void DD(no **);

void ED(no **);

void DE(no **);

int aux_insere(no **, char *, char *, int *);

int inserir_avl(no **, char *, char *);