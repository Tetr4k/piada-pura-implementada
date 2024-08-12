#include <stdlib.h>
#include <stdio.h>
#include<string.h>

#define MAX_STRING 255
#define NUM_CONTATOS 10
#define NUM_MSG 5
#define DELIMITER "|"

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

contato lista_contatos[NUM_CONTATOS] = {0};
// 1|002|matheus|ip|porta|oi



// Função para tratar pacote e redirecionar para o tipo de operação
void tratar_pacote(char* pacote){

	int ito = atoi(strtok(pacote, DELIMITER));
	int tam_pacote = atoi(strtok(NULL, DELIMITER));

	switch (ito)
	{
	case 0:
		// ERRO
		break;
	case 1:
		// SUCESSO
		break;
	case 2:
		// DEVOLVE LISTA
		break;
	case 3:
		// SOLICITAÇÃO DE REGISTRO
		break;
	case 4:
		// SOLICITAÇÃO DE LISTA
		solicita_lista_servidor();
		break;
	case 5:
		// MANDAR MENSAGEM
		break;
	case 6:
		// MANDAR MENSAGEM BROADCAST
		break;
	case 7:
		// SOLICITAÇÃO PARA DEDSCONECTAR
		break;
	default:
		break;
	}
}

// servidor recebendo mensagem de solicitação de lista
void solicita_lista_servidor(){
	char* nome = strtok(NULL, DELIMITER);
	char* IP = strtok(NULL, DELIMITER);
	char* PORTA = strtok(NULL, DELIMITER);
	criar_mensagem_lista_de_contato(); // mensagem de resposta do servidor
}

void m_print_contato(contato tmp){
    printf("nome: %s\n", tmp.nome);
    printf("ip: %s\n", tmp.ip);
    printf("porta: %d\n", tmp.porta);
}

// Função para criar contato