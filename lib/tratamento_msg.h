#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "./estruturas.h"
#include "./defines.h"

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
		// MANDAR MENSAGEM
		receber_mensagem_cliente();
		break;
	case 5:
		// MANDAR MENSAGEM BROADCAST
		break;
	case 6:
		// SOLICITAÇÃO PARA DEDSCONECTAR
		break;
	default:
		break;
	}
}

void print_contatos(){
	for(int i = 0; i < NUM_CONTATOS; i++){ // itera em vetor de contato até chegar em campo vazio ou fim de vetor
		contato_esta_vazio(lista_contatos[i])){
			printf("Saindo\n");
			break;
		}
		printf("________________________");
		printf("Nome: %s\n", lista_contatos[i].nome);
		printf("IP: %s\n", lista_contatos[i].ip);
		printf("Porta: %d\n", lista_contatos[i].porta);
		printf("________________________\n");
	}
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
char* devolver_lista_servidor(){ // ""
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

	return pacote; // retorna ou copia para o pacote passado em parâmetro?
}


void mandar_resposta(int ito, char* msg){
	char* pacote = malloc(5);
	int pacote_tam;

	sprintf(pacote, "%s|%03d",ito, strlen(msg)-1); // testar
	pacote_tam = strlen(pacote) + strlen(msg);
	pacote = realloc(pacote, pacote_tam);
	strcat(pacote, msg);

}

void receber_resposta(){
	char* msg = strtok(NULL, DELIMITER);
	printf("%s!\n",msg);
}
void receber_mensagem_cliente(){
	char* nome_r = strtok(NULL, DELIMITER);
	char* IP_r = strtok(NULL, DELIMITER);
	char* PORTA_r = strtok(NULL, DELIMITER);
	char* msg = strtok(NULL, DELIMITER);

	if(msg == NULL){
		//mensagem de erro
	}else{
		// printar mensagem (função? direto aqui?)
		//mensagem de ok
	}
}

void print_contato(contato tmp){
    printf("nome: %s\n", tmp.nome);
    printf("ip: %s\n", tmp.ip);
    printf("porta: %d\n", tmp.porta);
}