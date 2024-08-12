#include <stdlib.h>
#include <stdio.h>
#include<string.h>
#include <stdbool.h>
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

contato lista_contatos[NUM_CONTATOS] = {0};
// 1|002|matheus|ip|porta|oi

/// Funções para manipular dados
char* criar_mensagem_lista_de_contato();
bool contato_esta_vazio();
void solicita_lista_servidor();

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
// return char*?
void solicita_lista_servidor(){
	char* nome = strtok(NULL, DELIMITER);
	char* IP = strtok(NULL, DELIMITER);
	char* PORTA = strtok(NULL, DELIMITER);
	criar_mensagem_lista_de_contato(); // retunr pacote mas... como vai interagir com o servidor?
}

void m_print_contato(contato tmp){
    printf("nome: %s\n", tmp.nome);
    printf("ip: %s\n", tmp.ip);
    printf("porta: %d\n", tmp.porta);
}

bool contato_esta_vazio(contato tmp){
	bool result = (tmp.nome[0] == '\0') && (tmp.ip[0] == '\0') && (tmp.porta == 0);
	return result;
}
// what: concatena contato a mensagem
void m_concat_str(char** dest, contato contato){
    char tmp[40];
    sprintf(tmp, "|%s|%s|%d", contato.nome, contato.ip, contato.porta);
 	int tamanho_total = strlen(*dest) + strlen(tmp); // +1 for caractere +1 for '\0'
    *dest = realloc(*dest, tamanho_total);
    strcat(*dest,tmp);
}
// return: string de mensagem (type: char*)
char* criar_mensagem_lista_de_contato(){
	char* PORTA;
	char* msg = malloc(1);
	char* pacote = malloc(5);
	int pacote_tam;
	for(int i = 0; i < NUM_CONTATOS; i++){ // itera em vetor de contato até chegar em campo vazio ou fim de vetor
		if(contato_esta_vazio(lista_contatos[i])){
			printf("Saindo\n");
			break;
		}
		m_concat_str(&msg, lista_contatos[i]); // transforma contato em string
	}

	// montagem do pacote
	sprintf(pacote, "2|%03d", strlen(msg)-1);
	pacote_tam = strlen(pacote) + strlen(msg);
	pacote = realloc(pacote, pacote_tam);
	strcat(pacote, msg);
	return pacote;
}