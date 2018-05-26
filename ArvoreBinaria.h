#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct bloco {
	char chave[4];
	char rrn[6];
	struct bloco *esq, *dir;
} no;

typedef struct {
	no *raiz;
} Arvore;

void cria_abb (Arvore*);
int IsEmpty (Arvore*);
void insere_abb(no **, char *, char *);
no *busca_abb(no *, char *);
int busca_rrn_abb(no *, char *);
no *busca_maior(no *);
int remove_abb (no **, char *);