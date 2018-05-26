#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	int *n = malloc(5*sizeof(int));
	for (int i = 0; i < 5; i++){
		n[i] = i+100;
	}
	n = realloc(n, 7*sizeof(int));
	n[5] = 105;
	n[6] = 106;
	for(int i = 0; i < 7; i++){
		printf("%d\n", n[i]);
	}
	return 0;
}