#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct bloco {
	char chave[3];
	char rrn[5];
	struct bloco *prox;
} no;

typedef struct {
	no *inicio;
} lista;


void inicializa_lista(lista *);
void insere_lista(lista *, char *, char *);