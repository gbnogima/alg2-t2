/*
TRABALHO DE ALGORITMOS E ESTRUTURAS DE DADOS II

Alunos:
Guilherme Brunassi Nogima (9771629)
Carlos Henrique de Oliveira Franco (9771608)
*/

#include "ArvoreBinaria.h"

#define TAMREG 84
#define REG_LEN 63

int nRegistro;//Quantidade de registro
int nRRN;//Quantidade de registros no arquivo de dados, incluindo os removidos

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

//Remove espaços em branco das strings
char *formata_string (char *str) {
	int i = 0, j = 0;
	while(str[i] == ' '){
		i++;
	}
	while(i < strlen(str)){
		str[j++] = str[i++];
	}
	while(j < strlen(str)){
		str[j++] = ' ';
	}
	return str;
}

/*
Carrega o arquivo de índices, no início do programa, do disco para a memória principal
*/
void carrega_memoria(Arvore *T){
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

void compactar () {	
	FILE *fr, *fw;
	fr = fopen("dados.txt", "r");
	fw = fopen("dados_aux.txt", "w");
	char input[REG_LEN+1];
	for(int i = 0; i < nRRN; i++){
		fgets(input, sizeof(input), fr);
		if (input[0] != '$'){
			fputs(input, fw);
		}
	}

	fclose(fr);
	fclose(fw);
	remove("dados.txt");
	rename("dados_aux.txt", "dados.txt");
	nRRN = nRegistro;
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
        //printf("nro=%s**nome=%s**carro=%s**\n", nro,nome,carro);
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
			compactar();
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
	FILE *fp = fopen("primario.ndx", "w");
	fprintf(fp, "%d ", nRegistro);
	fprintf(fp, "%d\n", nRRN);
	grava_indice(T->raiz, fp);
	fclose(fp);

	return 0;
}