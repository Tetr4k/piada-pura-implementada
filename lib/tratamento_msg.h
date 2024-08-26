#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
//#include "./estruturas.h"
#include "./defines.h"

// Mudar mandar respostas: nome não intuitivo!!!!!!

int tratar_pacote(char* pacote);									// Redireciona para um tratamento de pacote de acordo com a ITO
char* devolver_lista_servidor(); 									// "|NOME1|IP1|PORTA1|NOME2|IP2|PORTA2"
void receber_resposta();											// Trata mensagem de ERRO/OK
char* criar_mensagem(int ito, char* msg);							// Monta pacote de mensagem
void print_contatos(); 												// Função para imprimir lista de contatos
void broadcast_message(const char *message); 						// Função para enviar mensagem em broadcast
char* desconectar_cliente(char *nome, char *ip, int porta); 		// Função para desconectar cliente
void cria_lista(char str_lista[]);									// Cria a lista de contato passada por referência
int receber_mensagem_cliente(char *nome, int broadcast);			// Processa a mensagem recebida e a coloca no buffer_msg
void cria_mensagem_lista(char* pacote);
void atualiza_contatos();


// Função para tratar pacote e redirecionar para o tipo de operação
int tratar_pacote(char* pacote){
	// Poderia retornar o ito, pois caso tenha erro a tela ou o socket tem q saber como tratar

	int ito = atoi(strtok(pacote, DELIMITER));
	int tam_pacote = atoi(strtok(NULL, DELIMITER));

	printf("*\n");

	// captura e valida dados
	char* nome		= strtok(NULL, DELIMITER);
	char* ip		= strtok(NULL, DELIMITER);
	char* porta_str	= strtok(NULL, DELIMITER);

	printf("**\n");


	if (nome == NULL || ip == NULL || porta_str == NULL){
		strncpy(pacote, criar_mensagem(0, "ERRO: FALHA AO CAPTURAR UM TOKEN!"), MAX_PACOTE);
		return 0;
	}

	printf("***\n");

	int porta = atoi(porta_str);

	printf("****\n");

	switch (ito)
	{
	case 0:
		//receber_resposta(); // ERRO
		break;

	case 1:
		//receber_resposta(); // SUCESSO
		break;

	case 2: // RECEBI LISTA (Cliente)
		
		// atualizar lista de contatos
		atualiza_contatos();
		strncpy(pacote, criar_mensagem(OK, ""), MAX_PACOTE);
		break;

	case 3: // SOLICITAÇÃO DE REGISTRO (Servidor)

		contato novoContato;
		sprintf(novoContato.nome, "%s", nome);
		sprintf(novoContato.ip, "%s", ip);
		novoContato.porta	= porta;

		// adiciona contato a lista de contatos
		pthread_mutex_lock(&mutex_contatos);

		if (qtdContatos >= NUM_CONTATOS){
			strncpy(pacote, criar_mensagem(0, "ERRO: MAXIMO DE CLIENTES ATINGIDO!"), MAX_PACOTE);
			return 0;
		}

		printf("Novo contato: %s\n", nome);
		lista_contatos[qtdContatos] = novoContato;
		qtdContatos++;

		printf("Terminou de adicionar\n");

		pthread_mutex_unlock(&mutex_contatos);

		printf("Cria mensagem lista\n");
		cria_mensagem_lista(pacote);
		break;

	case 4: // TRATAR MENSAGEM RECEBIDA
		if (!receber_mensagem_cliente(nome, 0))
			strncpy(pacote, criar_mensagem(0, "ERRO: Mensagem vazia!"), MAX_PACOTE);
		else
			strncpy(pacote, criar_mensagem(1, ""), MAX_PACOTE);

		break;

	case 5: // RECEBER MENSAGEM BROADCAST
		if (!receber_mensagem_cliente(nome, 1))
			strncpy(pacote, criar_mensagem(0, "ERRO: Mensagem vazia!"), MAX_PACOTE);
		else
			strncpy(pacote, criar_mensagem(1, ""), MAX_PACOTE); 

		break;

	case 6:
		
		// SOLICITAÇÃO PARA DESCONECTAR
		desconectar_cliente(nome, ip, porta);
		cria_mensagem_lista(pacote);
		break;

	default:
		break;
	}

	return ito;
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

	pthread_mutex_lock(&mutex_contatos);

	for(int i = 0; i < qtdContatos; i++){
		printf("Nome: %s\n", lista_contatos[i].nome);
		printf("IP: %s\n", lista_contatos[i].ip);
		printf("Porta: %d\n", lista_contatos[i].porta);
		printf("________________________\n");
	}

	pthread_mutex_unlock(&mutex_contatos);
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

	pthread_mutex_lock(&mutex_contatos);

	for(int i=0; i<qtdContatos; i++){
		char tmp[40];
		sprintf(tmp, "%s|%s|%d|", lista_contatos[i].nome, lista_contatos[i].ip, lista_contatos[i].porta);
		strcat(str_lista, tmp);
	}

	pthread_mutex_unlock(&mutex_contatos);
}

void cria_mensagem_lista(char *pacote){
	// Gera lista de contatos nova
	printf("1");
	char str_lista[MAX_PACOTE] = "";
	cria_lista(str_lista);
	printf("2");
	// Retorna a lista de contatos
	strncpy(pacote, criar_mensagem(2, str_lista), MAX_PACOTE);
}

void atualiza_contatos(){
	pthread_mutex_lock(&mutex_contatos);

	memset(lista_contatos, 0, NUM_CONTATOS*sizeof(contato));

	int contador = 0;
    char* token = strtok(NULL, "|");
    
    while (token != NULL && contador < NUM_CONTATOS) {
        strncpy(lista_contatos[contador].nome, token, MAX_NAME);
        lista_contatos[contador].nome[MAX_NAME - 1] = '\0';
        
        token = strtok(NULL, "|");
        if (token != NULL) {
            strncpy(lista_contatos[contador].ip, token, 16);
            lista_contatos[contador].ip[15] = '\0';
        }

        token = strtok(NULL, "|");
        if (token != NULL) {
            lista_contatos[contador].porta = atoi(token);
        }

        contador++;
        token = strtok(NULL, "|");
    }

	qtdContatos = contador;

	pthread_mutex_unlock(&mutex_contatos);
}

void receber_resposta(){
	char* msg = strtok(NULL, DELIMITER);
	printf("%s!\n",msg);
}

void guardar_mensagem(char* nome, char* msg, int broadcast){
	pthread_mutex_lock(&mutex_mensagens);

	mensagem nova_msg;

	strncpy(nova_msg.nome, nome, MAX_NAME - 1);
    nova_msg.nome[MAX_NAME - 1] = '\0'; // Garantir que a string será null-terminated

    strncpy(nova_msg.mensagem, msg, MAX_STRING - 1);
    nova_msg.mensagem[MAX_STRING - 1] = '\0'; // Garantir que a string será null-terminated

	nova_msg.broadcast = broadcast;

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

int receber_mensagem_cliente(char nome[MAX_NAME], int broadcast){
	char* msg = strtok(NULL, DELIMITER);

	if(msg == NULL)
		return 0;

	guardar_mensagem(nome, msg, broadcast);
	return 1;
}
