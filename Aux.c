/*
TRABALHO DE ALGORITMOS E ESTRUTURAS DE DADOS II

Alunos:
Guilherme Brunassi Nogima (9771629)
Carlos Henrique de Oliveira Franco (9771608)
*/

#include "Aux.h"

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


