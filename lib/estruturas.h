#include "defines.h"

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

// 1|002|matheus|ip|porta|oi