#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
//#include "./estruturas.h"
#include "./defines.h"

// Mudar mandar respostas: nome não intuitivo!!!!!!

void tratar_pacote(char* pacote);									// Redireciona para um tratamento de pacote de acordo com a ITO
char* devolver_lista_servidor(); 									// "|NOME1|IP1|PORTA1|NOME2|IP2|PORTA2"
void receber_resposta();											// Trata mensagem de ERRO/OK
char* criar_mensagem(int ito, char* msg);							// Monta pacote de mensagem
bool contato_esta_vazio(); 											// (AUX)Verifica se dado contato está vazio
void m_concat_str(char** dest, contato contato); 					// (AUX) Concatena informações do contato em uma string => "|NOME|IP|PORTA"
void print_contatos(); 												// Função para imprimir lista de contatos
void broadcast_message(const char *message); 						// Função para enviar mensagem em broadcast
char* desconectar_cliente(char *nome, char *ip, int porta); 		// Função para desconectar cliente
void cria_lista(char str_lista[]);									// Cria a lista de contato passada por referência
void receber_mensagem_cliente();									// Processa a mensagem recebida e a coloca no buffer_msg


// Função para tratar pacote e redirecionar para o tipo de operação
void tratar_pacote(char* pacote){

	int ito = atoi(strtok(pacote, DELIMITER));
	int tam_pacote = atoi(strtok(NULL, DELIMITER));

	// captura e valida dados
	char* nome		= strtok(NULL, DELIMITER);
	char* ip		= strtok(NULL, DELIMITER);
	char* porta_str	= strtok(NULL, DELIMITER);

	if (nome == NULL || ip == NULL || porta_str == NULL){
		criar_mensagem(3, "ERRO: FALHA AO CAPTURAR UM TOKEN!");
		return;
	}

	int porta = atoi(porta_str);

	switch (ito)
	{
	case 0:
		receber_resposta(); // ERRO
		break;

	case 1:
		receber_resposta(); // SUCESSO
		break;

	case 3:
		// SOLICITAÇÃO DE REGISTRO

		if (qtdContatos == NUM_CONTATOS){
			criar_mensagem(3, "ERRO: MAXIMO DE CLIENTES ATINGIDO!");
			return;
		}

		// adiciona contato a lista de contatos
		qtdContatos++;

		contato novoContato;
		sprintf(novoContato.nome, "%s", nome);
		sprintf(novoContato.ip, "%s", ip);
		novoContato.porta	= porta;

		lista_contatos[qtdContatos-1] = novoContato;

	case 2:
		// DEVOLVE LISTA

		char str_lista[MAX_PACOTE] = "";
		cria_lista(str_lista);

		criar_mensagem(OK, str_lista);
		return;

	case 4:
		receber_mensagem_cliente(); // TRATAR MENSAGEM RECEBIDA
		break;

	case 5: // RECEBER MENSAGEM BROADCAST
		char* msg 	= strtok(NULL, DELIMITER);

		if (msg != NULL) {
			broadcast_message(msg);
		} else {
			criar_mensagem(ERRO, "ERRO");
		}
	
		break;
	case 6:
		
		// SOLICITAÇÃO PARA DESCONECTAR
		desconectar_cliente(nome, ip, porta);
		break;

	default:
		break;
	}
}

// Função Servidor
char* desconectar_cliente(char* nome, char *ip, int porta) {

	pthread_mutex_lock(&mutex_contatos);
    // Remover o cliente da lista de clientes
    for (int i = 0; i < qtdContatos; i++) {
        if (strcmp(lista_contatos[i].nome, nome) == 0) {
			lista_contatos[i] = lista_contatos[qtdContatos-1];
            qtdContatos--;
            break;
        }
    }
	pthread_mutex_unlock(&mutex_contatos);
    printf("Cliente desconectado: %s\n", nome);
}

void print_contatos(){

	printf("________________________\n");

	for(int i = 0; i < qtdContatos; i++){
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

char* criar_mensagem(int ito, char* msg){
	char* pacote = malloc(MAX_PACOTE*sizeof(char));
	int pacote_tam;

	sprintf(pacote, "%d|%03ld|%s|%s|%d|", ito, strlen(msg), meu_contato.nome, meu_contato.ip, meu_contato.porta);
	pacote_tam = strlen(pacote) + strlen(msg);
	pacote = realloc(pacote, pacote_tam);
	strcat(pacote, msg);

	return pacote;
}

void cria_lista(char str_lista[]){
	for(int i=0; i<qtdContatos; i++){
		char tmp[40];
		sprintf(tmp, "%s|%s|%d|", lista_contatos[i].nome, lista_contatos[i].ip, lista_contatos[i].porta);
		strcat(str_lista, tmp);
	}
}

void receber_resposta(){
	char* msg = strtok(NULL, DELIMITER);
	printf("%s!\n",msg);
}

void guardar_mensagem(char* nome, char* msg){
	pthread_mutex_lock(&mutex_mensagens);

	mensagem nova_msg;

	strncpy(nova_msg.nome, nome, MAX_NAME - 1);
    nova_msg.nome[MAX_NAME - 1] = '\0'; // Garantir que a string será null-terminated

    strncpy(nova_msg.mensagem, msg, MAX_STRING - 1);
    nova_msg.mensagem[MAX_STRING - 1] = '\0'; // Garantir que a string será null-terminated

	
	// Caso o buffer esteja cheio
	if (buffer_ult >= NUM_MSG){
		for(int i=0; i < NUM_MSG-1; i++){
			memset(&buffer_msg[i], 0, sizeof(mensagem));
			memcpy(&buffer_msg[i], &buffer_msg[i+1], sizeof(mensagem));
		}
		memset(&buffer_msg[NUM_MSG-1], 0, sizeof(mensagem));
		memcpy(&buffer_msg[NUM_MSG-1], &nova_msg, sizeof(mensagem));
	}
	else{
		memset(&buffer_msg[buffer_ult], 0, sizeof(mensagem));
		memcpy(&buffer_msg[buffer_ult], &nova_msg, sizeof(mensagem));
		buffer_ult++;
	}
	
	pthread_mutex_unlock(&mutex_mensagens);	
}

void print_mensagens(){
	for(int i=0; i < NUM_MSG; i++)
		printf("%s> %s\n", buffer_msg[i].nome, buffer_msg[i].mensagem);
	
	printf("________________________\n");
}

void receber_mensagem_cliente(){
	char* msg = strtok(NULL, DELIMITER);

	if(msg == NULL){
		criar_mensagem(ERRO,"ERRO");

	}else{
		criar_mensagem(OK,"OK");
		// Adicionar memnsagem e nome no buffer de contatos
	}
}
