#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ArvoreBinaria.h"

#define TAMREG 84

int nReg;

//Altera chave(nro) para que possua sempre 3 digitos
void formata_chave (char *chave) {
	int n = atoi(chave);
	if (n < 10) {
		sprintf(chave, "00%d", n);
	}
	else if (n < 100) {
		sprintf(chave, "0%d", n);
	}
}

//Altera rrn para que possua sempre 5 digitos
void formata_rrn (char *rrn) {
	int n = atoi(rrn);
	if (n < 10) {
		sprintf(rrn, "0000%d", n);
	}
	else if (n < 100) {
		sprintf(rrn, "000%d", n);
	}
	else if (n < 1000) {
		sprintf(rrn, "00%d", n);
	}
	else if (n < 10000) {
		sprintf(rrn, "0%d", n);
	}
}

void carrega_memoria(Arvore *T){
	FILE *fp;
	fp = fopen("primario.ndx", "r");
	char str1[4], str2[6], c;
	fscanf(fp, "%d", &nReg);
	printf("nReg = %d\n", nReg);
	for (int i = 0; i < nReg; i++) {
		getc(fp);
		fgets(str1, sizeof(str1), fp);
		getc(fp);
		fgets(str2, sizeof(str2), fp);
		insere_abb(&(T->raiz), str1, str2);
	}
	fclose(fp);
}

void grava_indice(no *n, FILE *fp) {
	if (n == NULL) 
		return;

	grava_indice(n->esq, fp);
	fprintf(fp, "%s %s\n", n->chave, n->rrn);
	grava_indice(n->dir, fp);
}

void imprime_arvore(no *n) {
	if (n == NULL) 
		return;

	imprime_arvore(n->esq);
	printf("%s ", n->chave);
	printf("%s\n", n->rrn);
	imprime_arvore(n->dir);
}

void insere_arq(char *nro, char *nome, char *carro, Arvore *T){
	FILE *fp;
	fp = fopen("dados.txt", "a");
	formata_chave(nro);
	fprintf(fp, "%*s", 3, nro);
	fprintf(fp, "%*s", 40, nome);
	fprintf(fp, "%*s", 20, carro);
	fclose(fp);

	char rrn[6], str[6];
	sprintf(str, "%d", nReg);
	strcpy(rrn, str);
	formata_rrn(rrn);
	insere_abb(&(T->raiz), nro, rrn);
	nReg++;
}

void inserir (Arvore *T) {
	printf("[INSERIR]\n");
	char nro[4];
	char nome [41];
	char carro [21];
	printf("\n\nInsira os seguintes dados:\n");
	printf("NRO: ");
	scanf("%[^\n]s", nro);
	setbuf(stdin, NULL);
	printf("Nome: ");
	scanf("%[^\n]s", nome);
	setbuf(stdin, NULL);
	printf("Carro: ");
	scanf("%[^\n]s", carro);
	setbuf(stdin, NULL);
	insere_arq(nro, nome, carro, T);
	
}

void remove_arq (Arvore *T, int rrn) {
	FILE *fp;
	fp = fopen("dados.txt", "r+");
	fseek(fp, rrn*63, SEEK_SET);
	fputs("$", fp);
	fclose(fp);
	nReg--;
}

void remover (Arvore *T) {
	printf("[REMOVER]\n");
	char nro[3];
	printf("\n\nInsira os seguintes dados:\n");
	printf("NRO: ");
	scanf("%[^\n]s", nro);
	setbuf(stdin, NULL);
	int rrn = busca_rrn_abb(T->raiz, nro);
	remove_abb(&(T->raiz), nro);
	remove_arq(T, rrn);
}

void procurar (Arvore *T) {
	printf("[PROCURAR]\n");
	char nro[3];
	printf("\n\nInsira os seguintes dados:\n");
	printf("NRO: ");
	scanf("%[^\n]s", nro);
	setbuf(stdin, NULL);
	int rrn = busca_rrn_abb(T->raiz, nro);


}


/* 
   funcao que percorre a string de tras para frente eliminando os espacos em branco...
*/
char *trim (char *str) {
	int i;

	for (i = strlen(str)-1; str[i] == ' '; i--);

	str[i+1] = '\0';

	return str;
}

void flelinha(FILE *fp, char *nro, char *nome, char *carro){
	
	char c;
 	int i;
	// pula  "|" inicial
	c = fgetc(fp);
	// enquanto houver digitos vai lendo e armazena no nro
	i = 0;
	while ((c = fgetc(fp)) != ' ')
	    nro[i++] = c;
	nro[i] = '\0'; 

	// vai pulando até chegar no nome...
	while ((c = fgetc(fp)) != '|');

	// agora le o nome
	for (i=0; i<40;i++)
	   nome[i] = fgetc(fp);
        nome[i] = '\0';

	// elimina os brancos do lado direito...
        nome = trim(nome);

        // pula  "|" 
	c = fgetc(fp);

	// agora le o carro
	for (i=0; i<20;i++)
	   carro[i] = fgetc(fp);
        carro[i] = '\0';

	// elimina os brancos do lado direito...
        carro = trim(carro);

    // pula  "|" 
	c = fgetc(fp);
	// pula  "\n" 
	c = fgetc(fp);
}

void learquivo (Arvore *T){
	FILE *fp;
	char str[100];
 	char nro[4];
	char nome [41];
	char carro [21];
	
	int nReg; // numero de registros....
	
	fp = fopen("TabelaInicial.txt", "r");

	// le o nro de registros
	fscanf(fp, "nro de registros = %d\n", &nReg);

	// pula as outras duas linhas iniciais que sao comentarios...
	fgets(str,100,fp); fgets(str,100,fp);

	for (int i = 0; i < nReg; ++i) {
		flelinha(fp, nro, nome, carro);	
        //printf("nro=%s**nome=%s**carro=%s**\n", nro,nome,carro);
        insere_arq(nro, nome, carro, T);
	}

	fclose (fp);
}

void menu(Arvore *T) {
	int run = 1;

	while(run) {
		int op;
		printf("Selecione uma operacao:\n");
		printf("1. inserir\n2. remover\n3. alterar\n4. procurar\n5. compactar\n6. sair");
		printf("\nOperacao: ");
		scanf("%d%*c", &op);
		switch(op) {
		case 1:
			inserir(T);
			break;
		case 2:
			remover(T);
			break;
		case 3:
			//alterar();
			break;
		case 4:
			procurar(T);
			break;
		case 5:

			break;
		case 6:
			run = 0;
			break;
		default:
			printf("\n<Erro: selecione uma operacao valida>\n");
			break;
		}
	}
}

int main (){

	Arvore *T = malloc(sizeof(Arvore));
	cria_abb(T);
	//carrega_memoria(T);
	learquivo(T);
	menu(T);


	FILE *fp = fopen("primario.ndx", "w");
	fprintf(fp, "%d\n", nReg);
	grava_indice(T->raiz, fp);
	fclose(fp);


	//imprime_arvore(T->raiz);
}