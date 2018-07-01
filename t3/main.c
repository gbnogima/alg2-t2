/*
TRABALHO DE ALGORITMOS E ESTRUTURAS DE DADOS II

Alunos:
Guilherme Brunassi Nogima (9771629)
Carlos Henrique de Oliveira Franco (9771608)
*/

#include "btree.h"
#include "Aux.h"

#define TAMREG 84
#define REG_LEN 63

int nRegistro;//Quantidade de registro
int nRRN;//Quantidade de registros no arquivo de dados, incluindo os removidos



/*
Reconstrói o índice primário a partir do arquivo de dados caso seja detectado que ele está desatualizado
*/
void reconstroi_indice(bTree *T){
	nRRN = 0;
	nRegistro = 0;
	FILE *fr;
	fr = fopen("dados.txt", "r");
	char nro[4];
	char nome [41];
	char carro [21];
	char rrn[6];
	int j = 0;
	
	remove(BTFILE);

	initBT(T);

	while(fgets(nro, sizeof(nro), fr) != NULL){
	 	fgets(nome, sizeof(nome), fr);
	 	fgets(carro, sizeof(carro), fr);
	 	sprintf(rrn, "%d", nRRN++);
		formata_rrn(rrn);
		insert(T, atoi(nro), atoi(rrn)*REG_LEN);
		if (nro[0] != '$'){
			nRegistro++;
		}
	}

	fclose(fr);
}


/*
Carrega o arquivo de índices, no início do programa, do disco para a memória principal
*/
void carrega_memoria(bTree *T){
	FILE *fp;

	fp = fopen("info.txt", "r");

	if (fp == NULL){
		nRRN = 0;
		nRegistro = 0;
		return;
	}

	fscanf(fp, "%d", &nRegistro);
	fscanf(fp, "%d", &nRRN);

	fclose(fp);
}

int insere_arq(char *nro, char *nome, char *carro, bTree *T){
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
	if(!insert(T, atoi(nro), atoi(rrn)*REG_LEN)){
		printf("\n[Dado já cadastrado]\n");
		return 0;
	}
	nRegistro++;
	nRRN++;
	return 1;
}

void inserir (bTree *T) {
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
	if (insere_arq(nro, nome, carro, T))
		printf("\n[Dado inserido]\n");
}

void remove_arq (bTree *T, int offset) {
	FILE *fp;
	fp = fopen("dados.txt", "r+");
	fseek(fp, offset, SEEK_SET);
	fputs("$", fp);
	fclose(fp);
	nRegistro--;
	reconstroi_indice(T);
}

void remover (bTree *T) {
	printf("[REMOVER]\n");
	char nro[3];
	printf("Insira os seguintes dados:\n");
	printf("NRO: ");
	scanf("%[^\n]s", nro);
	setbuf(stdin, NULL);

	int offset = search(T, atoi(nro));
	if (offset == -1){
		printf("\n[Dado não encontrado]\n");
		return;
	}

	remove_arq(T, offset);
	printf("\n[Dado removido]\n");

}

void alterar (bTree *T) {
	printf("[ALTERAR]\n");
	char nro[4];
	printf("Insira os seguintes dados:\n");
	printf("NRO: ");
	scanf("%s", nro);
	setbuf(stdin, NULL);

	int offset = search(T, atoi(nro));
	if (offset == -1){
		printf("\n[Dado não encontrado]\n");
		return;
	}

	FILE *fp;
	fp = fopen("dados.txt", "r");
	fseek(fp, offset, SEEK_SET);
 	char nome [41], carro [21];
 	fgets(nro, sizeof(nro), fp);
 	fgets(nome, sizeof(nome), fp);
 	fgets(carro, sizeof(carro), fp);
 	printf("NRO: %s\n", nro);	
 	printf("NOME: %s\n", formata_string(nome));
 	printf("CARRO: %s\n", formata_string(carro));
 	fclose(fp);

 	remove_arq(T, offset);

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
	printf("\n[Dado alterado]\n");
}


void procurar (bTree *T) {
	printf("[PROCURAR]\n");
	char nro[4];
	printf("Insira os seguintes dados:\n");
	printf("NRO: ");
	scanf("%s", nro);
	setbuf(stdin, NULL);

	int offset = search(T, atoi(nro)) ;
	
	if (offset == -1){
		printf("\n[Dado não encontrado]\n");
		return;
	}
	FILE *fp;
	fp = fopen("dados.txt", "r");
	fseek(fp, offset, SEEK_SET);
 	char nome [41], carro [21];
 	fgets(nro, sizeof(nro), fp);

 	fgets(nome, sizeof(nome), fp);
 	fgets(carro, sizeof(carro), fp);
 	printf("NRO: %s\n", nro);	
 	printf("NOME: %s\n", formata_string(nome));
 	printf("CARRO: %s\n", formata_string(carro));
 	fclose(fp);
}


void compactar (bTree *T) {	
	remove(BTFILE);
	initBT(T);

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
			insert(T, atoi(nro), atoi(rrn)*REG_LEN);
		}
	}

	fclose(fr);
	fclose(fw);
	remove("dados.txt");
	rename("dados_aux.txt", "dados.txt");
	nRRN = nRegistro;

	printf("[Compactação concluída com sucesso]\n");
}

void le_arquivo (bTree *T){
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

void menu(bTree *T) {
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
	bTree T ;
	initBT(&T) ;

	carrega_memoria(&T);

	FILE *f = fopen("dados.txt", "r");

	if (f == NULL) //Verifica se há um arquivo de dados e se a tabela já foi lida
		le_arquivo(&T); //Função de leitura fornecida
	menu(&T);

	FILE *fp1;
	fp1 = fopen("info.txt", "w");
	fprintf(fp1, "%d ", nRegistro);
	fprintf(fp1, "%d\n", nRRN);
	fclose(fp1);


	return 0;
}