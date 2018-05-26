#include "ArvoreBinaria.h"

void cria_abb (Arvore *T) {
	T->raiz = NULL;
}

int IsEmpty (Arvore *T) {
	if (T->raiz == NULL)
		return 1;
	else return 0;
}

void insere_abb(no **n, char *chave, char *rrn) {
	while ((*n) != NULL){
		if(atoi(chave) < atoi((*n)->chave))
			n = &((*n)->esq);
		else 
			n = &((*n)->dir);
	}
	*n = (no *) malloc(sizeof(no));
    (*n)->esq = NULL;
    (*n)->dir = NULL;
    strcpy((*n)->chave, chave);
    strcpy((*n)->rrn, rrn);
}

no *busca_abb(no *n, char *nro) {
	int x = atoi(nro);
	while (x != atoi(n->chave)){
		if (x < atoi(n->chave))
			n = n->esq;
		else
			n = n->dir;
	}
	return n;
}

int busca_rrn_abb(no *n, char *nro) {
	int x = atoi(nro);
	while (x != atoi(n->chave)){
		if (x < atoi(n->chave))
			n = n->esq;
		else
			n = n->dir;
	}
	return atoi(n->rrn);	
}

no *busca_maior(no *n) {
	while (n->dir != NULL)
		n = n->dir;
	return n;
}

int remove_abb (no **n, char *nro) {
	no *aux;
	int rrn;
	if(*n == NULL)
		return 0;

	else if (atoi(nro) < atoi((*n)->chave))
		return (remove_abb(&(*n)->esq, nro));

	else if (atoi(nro) > atoi((*n)->chave))
		return (remove_abb(&(*n)->dir, nro));

	else {
		if(((*n)->esq == NULL) && ((*n)->dir == NULL)) {
			rrn = atoi((*n)->rrn);
			free(*n);
			*n = NULL;
			return 1;
		}
		else if ((*n)->esq == NULL) {
			rrn = atoi((*n)->rrn);
			aux = *n;
			*n = (*n)->dir;
			free(aux);
			return 1;
		}
		else if ((*n)->dir == NULL) {
			rrn = atoi((*n)->rrn);
			aux = *n;
			*n = (*n)->esq;
			free(aux);
			return 1;
		}
		else {
			aux = busca_maior((*n)->esq);
			strcpy((*n)->chave, aux->chave);
    		strcpy((*n)->rrn, aux->rrn);
			return (remove_abb(&(*n)->esq, (*n)->chave));
		}
	}
}