#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "./estruturas.h"
#include "./defines.h"


void tratar_pacote(char* pacote);			// Redireciona para um tratamento de pacote de acordo com a ITO
char* devolver_lista_servidor(); 			// "|NOME1|IP1|PORTA1|NOME2|IP2|PORTA2"
void receber_resposta();				// Trata mensagem de ERRO/OK
void mandar_resposta(int ito, char* msg)		// Monta pacote de mensagem de ERRO/OK
bool contato_esta_vazio(); 				// (AUX)Verifica se dado contato está vazio
void m_concat_str(char** dest, contato contato); 	// (AUX) Concatena informações do contato em uma string => "|NOME|IP|PORTA"
void print_contatos(); 					// Função para imprimir lista de contatos
void broadcast_message(const char *message); 		// Função para enviar mensagem em broadcast
void desconectar_cliente(int client_socket); 		// Função para desconectar cliente


// Função para tratar pacote e redirecionar para o tipo de operação
void tratar_pacote(char* pacote){

	int ito = atoi(strtok(pacote, DELIMITER));
	int tam_pacote = atoi(strtok(NULL, DELIMITER));
	
	switch (ito)
	{
	case 0:
		receber_resposta(); // ERRO
		break;
	case 1:
		receber_resposta(); // SUCESSO
		break;
	case 2:
		// DEVOLVE LISTA
		break;
	case 3:
		// SOLICITAÇÃO DE REGISTRO
		break;
	case 4:
		receber_mensagem_cliente(); // TRATAR MENSAGEM RECEBIDA
		break;
	case 5:
		{
		// MANDAR MENSAGEM BROADCAST
			char* msg = strtok(NULL, DELIMITER);
	            	if (msg != NULL) {
	                broadcast_message(msg);
	            	} else {
	                mandar_resposta(ERRO, "ERRO");
	            	}
		}
		break;
	case 6:
		{
			// SOLICITAÇÃO PARA DEDSCONECTAR
			int client_socket = atoi(strtok(NULL, DELIMITER));  // Obtém o socket do cliente que solicitou a desconexão
	            	desconectar_cliente(client_socket);
		}
		break;
	default:
		break;
	}
}

void broadcast_message(const char *message) {
    char package[256];
    snprintf(package, sizeof(package), "ITO5:%s", message);

    for (int i = 0; i < num_clients; i++) {
        if (send(client_sockets[i], package, strlen(package), 0) == -1) {
            perror("Erro ao enviar mensagem broadcast");
        }
    }
}

void desconectar_cliente(int client_socket) {
    // Enviar mensagem de confirmação de desconexão
    mandar_resposta(OK, "Desconectado com sucesso");

    // Fechar o socket do cliente
    close(client_socket);

    // Remover o cliente da lista de clientes
    for (int i = 0; i < num_clients; i++) {
        if (client_sockets[i] == client_socket) {
            // Desloca os outros sockets para preencher o espaço do cliente desconectado
            for (int j = i; j < num_clients - 1; j++) {
                client_sockets[j] = client_sockets[j + 1];
            }
            num_clients--;
            break;
        }
    }

    printf("Cliente desconectado: socket %d\n", client_socket);
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
		mandar_resposta(ERRO,"ERRO");
	}else{
		mandar_resposta(OK,"OK");
		// printar mensagem (função? direto aqui?)
		//mensagem de ok
	}
}
