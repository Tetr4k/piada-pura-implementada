#include "./defines.h"

// Talvez necessário
typedef struct{
	char nome[MAX_NAME];
	char mensagem[MAX_STRING];
	int broadcast; // se 1 é broadcast
} mensagem;

typedef struct {
	char nome[MAX_NAME];
	char ip[16];
	int porta;
} contato;

// Lista de contatos
int qtdContatos = 0;
contato lista_contatos[NUM_CONTATOS] = {0};

contato meu_contato;

mensagem buffer_msg[NUM_MSG] = {0};
int buffer_ult = 0;
