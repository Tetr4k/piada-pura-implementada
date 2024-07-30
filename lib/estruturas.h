#include <stdlib.h>
#include <stdio.h>

#define MAX_STRING 255
#define NUM_CONTATOS 10
#define NUM_MSG 5

// Talvez necessário
typedef struct{
	int ito;
	int tam_pacote;
	char mensagem[MAX_STRING];
} pacote;

typedef struct {
	char nome[MAX_STRING];
	char ip[16];
	int porta;
} contato;

contato lista_contatos[NUM_CONTATOS];
// 1|002|matheus|ip|porta|oi