#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "arquivo.h"

#define TAMREG 84


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

void learquivo (){
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
        printf("nro=%s**nome=%s**carro=%s**\n", nro,nome,carro);
	}

	fclose (fp);
}

void menu() {
	int run = 1;

	while(run)
	{
		int op;
		printf("\n\nSelecione uma operacao:\n");
		printf("1. inserir\n2. remover\n3. alterar\n4. procurar\n5. compactar\n6. sair");
		printf("\nOperacao: ");
		scanf("%d%*c", &op);
		switch(op) {
		case 1:
			
			break;
		case 2:
			
			break;
		case 3:
			
			break;
		case 4:
			
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
	learquivo();
	menu();

}
 	