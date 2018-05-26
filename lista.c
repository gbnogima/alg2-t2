#include "lista.h"

void inicializa_lista(lista *L) {
	L->inicio = NULL;
}

void insere_lista(lista *L, char *nro, char *rrn) {
	no *atual, *ant, *p;
	p = (no *) malloc(sizeof(no));

	strcpy(p->chave, nro);
	strcpy(p->rrn, rrn);

	if ((L->inicio == NULL) || (atoi(nro) < atoi(L->inicio->chave))) {
		p->prox = L->inicio;
		L->inicio = p;
	}
	else {
		ant = NULL;
		atual = L->inicio;
		while((atual != NULL) && (atoi(atual->chave) < atoi(nro))) {
			ant = atual;
			atual = atual->prox;
		}
		p->prox = atual;
		ant->prox = p;
	}
}