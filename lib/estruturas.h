#include "./defines.h"

// Talvez necessário
typedef struct{
	int ito;
	int tam_pacote;
	char mensagem[MAX_STRING];
} pacote;

typedef struct {
	char nome[MAX_NAME];
	char ip[16];
	int porta;
} contato;
