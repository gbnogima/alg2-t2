/*
TRABALHO DE ALGORITMOS E ESTRUTURAS DE DADOS II

Alunos:
Guilherme Brunassi Nogima (9771629)
Carlos Henrique de Oliveira Franco (9771608)
*/

#include "ArvoreBinaria.h"
#include "Aux.h"

#define TAMREG 84
#define REG_LEN 63

int nRegistro;//Quantidade de registro
int nRRN;//Quantidade de registros no arquivo de dados, incluindo os removidos

/*
Reconstrói o índice primário a partir do arquivo de dados caso seja detectado que ele está desatualizado
*/
void reconstroi_indice(Arvore *T){
	nRRN = 0;
	nRegistro = 0;
	FILE *fr;
	fr = fopen("dados.txt", "r");
	char nro[4];
	char nome [41];
	char carro [21];
	char rrn[6];
	int j = 0;
	while(fgets(nro, sizeof(nro), fr) != NULL){
	 	fgets(nome, sizeof(nome), fr);
	 	fgets(carro, sizeof(carro), fr);
	 	nRRN++;
		if (nro[0] != '$'){
			sprintf(rrn, "%d", nRegistro++);
			formata_rrn(rrn);
			insere_abb(&(T->raiz), nro, rrn);
		}
	}

	fclose(fr);
}

/*
Verifica se o indice está atualizado
*/
int verifica_indice(Arvore *T) {
	FILE *fp;

	fp = fopen("flag.txt", "r+");

	int flag;
	fscanf(fp, "%d", &flag);
	if (flag == 1){
		reconstroi_indice(T);
	}
	fseek(fp, 0, SEEK_SET);
	fprintf(fp, "1");//Arquivo flag é marcado com "1", indicando que o índice será alterado
	fclose(fp);
	return flag;
}


/*
Carrega o arquivo de índices, no início do programa, do disco para a memória principal
*/
void carrega_memoria(Arvore *T){
	if(verifica_indice(T))
		return;

	FILE *fp;

	fp = fopen("primario.ndx", "r");

	if (fp == NULL){
		nRRN = 0;
		nRegistro = 0;
		return;
	}
	char str1[4], str2[6], c;
	fscanf(fp, "%d", &nRegistro);
	fscanf(fp, "%d", &nRRN);
	for (int i = 0; i < nRegistro; i++) {
		getc(fp);
		fgets(str1, sizeof(str1), fp);
		getc(fp);
		fgets(str2, sizeof(str2), fp);
		insere_abb(&(T->raiz), str1, str2);
	}
	fclose(fp);
}

/*
Grava o índice primário, armazenado em memória primária, no arquivo de índices no disco
*/
void grava_indice(no *n, FILE *fp) {
	if (n == NULL) 
		return;

	grava_indice(n->esq, fp);
	fprintf(fp, "%s %s\n", n->chave, n->rrn);
	grava_indice(n->dir, fp);
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
	sprintf(str, "%d", nRRN);
	strcpy(rrn, str);
	formata_rrn(rrn);
	insere_abb(&(T->raiz), nro, rrn);
	nRegistro++;
	nRRN++;
}

void inserir (Arvore *T) {
	printf("[INSERIR]\n");
	char nro[4];
	char nome [41];
	char carro [21];
	printf("Insira os seguintes dados:\n");
	printf("NRO: ");
	scanf("%s", nro);
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
	fseek(fp, rrn*REG_LEN, SEEK_SET);
	fputs("$", fp);
	fclose(fp);
	nRegistro--;
}

void remover (Arvore *T) {
	printf("[REMOVER]\n");
	char nro[3];
	printf("Insira os seguintes dados:\n");
	printf("NRO: ");
	scanf("%[^\n]s", nro);
	setbuf(stdin, NULL);
	int rrn = busca_rrn_abb(T->raiz, nro);
	remove_abb(&(T->raiz), nro);
	remove_arq(T, rrn);
}

void alterar (Arvore *T) {
	printf("[ALTERAR]\n");
	char nro[4];
	printf("Insira os seguintes dados:\n");
	printf("NRO: ");
	scanf("%s", nro);
	setbuf(stdin, NULL);
	int rrn = busca_rrn_abb(T->raiz, nro);
	FILE *fp;
	fp = fopen("dados.txt", "r");
	fseek(fp, rrn*REG_LEN, SEEK_SET);
 	char nome [41], carro [21];
 	fgets(nro, sizeof(nro), fp);
 	fgets(nome, sizeof(nome), fp);
 	fgets(carro, sizeof(carro), fp);
 	printf("NRO: %s\n", nro);	
 	printf("NOME: %s\n", formata_string(nome));
 	printf("CARRO: %s\n", formata_string(carro));
 	fclose(fp);


 	remove_abb(&(T->raiz), nro);
	remove_arq(T, rrn);


 	printf("\nSelecione um campo para alterar: \n1. NRO\n2. NOME\n3. CARRO\n");
 	int op;
 	printf("Operacao: ");
	scanf("%d%*c", &op);
	switch(op) {
		case 1:
			printf("\nNovo NRO: ");
			scanf("%s", nro);
			insere_arq(nro, nome, carro, T);
			break;
		case 2:
			printf("\nNovo Nome: ");
			scanf("%[^\n]s", nome);
			setbuf(stdin, NULL);
			insere_arq(nro, nome, carro, T);
			break;
		case 3:
			printf("\nNovo Carro: ");
			scanf("%[^\n]s", carro);
			setbuf(stdin, NULL);
			insere_arq(nro, nome, carro, T);
			break;
	}
}

void procurar (Arvore *T) {
	printf("[PROCURAR]\n");
	char nro[4];
	printf("Insira os seguintes dados:\n");
	printf("NRO: ");
	scanf("%s", nro);
	setbuf(stdin, NULL);
	int rrn = busca_rrn_abb(T->raiz, nro);
	FILE *fp;
	fp = fopen("dados.txt", "r");
	fseek(fp, rrn*REG_LEN, SEEK_SET);
 	char nome [41], carro [21];
 	fgets(nro, sizeof(nro), fp);
 	fgets(nome, sizeof(nome), fp);
 	fgets(carro, sizeof(carro), fp);
 	printf("NRO: %s\n", nro);	
 	printf("NOME: %s\n", formata_string(nome));
 	printf("CARRO: %s\n", formata_string(carro));
 	fclose(fp);
}

void compactar (Arvore *T) {	
	destroi_no(&(T->raiz));
	FILE *fr, *fw;
	fr = fopen("dados.txt", "r");
	fw = fopen("dados_aux.txt", "w");
	char nro[4];
	char nome [41];
	char carro [21];
	char rrn[6];
	int j = 0;
	for(int i = 0; i < nRRN; i++){
		fgets(nro, sizeof(nro), fr);
	 	fgets(nome, sizeof(nome), fr);
	 	fgets(carro, sizeof(carro), fr);
		if (nro[0] != '$'){
			fputs(nro, fw);
			fputs(nome, fw);
			fputs(carro, fw);
			sprintf(rrn, "%d", j++);
			formata_rrn(rrn);
			insere_abb(&(T->raiz), nro, rrn);
		}
	}

	fclose(fr);
	fclose(fw);
	remove("dados.txt");
	rename("dados_aux.txt", "dados.txt");
	nRRN = nRegistro;
}

void le_arquivo (Arvore *T){
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
        insere_arq(nro, nome, carro, T);
	}

	fclose (fp);
}

void menu(Arvore *T) {
	int run = 1;
	printf("\e[H\e[2J");
	printf("TRABALHO DE ALG II\nAlunos: \n\tGuilherme Brunassi Nogima (9771629)\n\tCarlos Henrique de Oliveira Franco (9771608)\n");
	while(run) {
		int op;
		printf("\n------------------------------------------------------\nSelecione uma operacao:\n");
		printf("1. inserir\n2. remover\n3. alterar\n4. procurar\n5. compactar\n6. sair");
		printf("\nOperacao: ");
		scanf("%d%*c", &op);
		printf("\e[H\e[2J");
		switch(op) {
		case 1:
			inserir(T);
			break;
		case 2:
			remover(T);
			break;
		case 3:
			alterar(T);
			break;
		case 4:
			procurar(T);
			break;
		case 5:
			compactar(T);
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
	carrega_memoria(T);
	FILE *f = fopen("dados.txt", "r");

	if (f == NULL) //Verifica se há um arquivo de dados e se a tabela já foi lida
		le_arquivo(T); //Função de leitura fornecida
	menu(T);

	FILE *fp1, *fp2;
	fp1 = fopen("primario.ndx", "w");
	fprintf(fp1, "%d ", nRegistro);
	fprintf(fp1, "%d\n", nRRN);
	grava_indice(T->raiz, fp1);
	fclose(fp1);

	fp2 = fopen("flag.txt", "w");
	fprintf(fp2, "0");//Marca o arquivo flag com 0, indicando que está devidamente atualizado
	fclose(fp2);

	destroi_abb(T);

	return 0;
}