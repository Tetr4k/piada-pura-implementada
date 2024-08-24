#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "./estruturas.h"
#include "./defines.h"

// Mudar mandar respostas: nome não intuitivo!!!!!!

void tratar_pacote(char* pacote);					// Redireciona para um tratamento de pacote de acordo com a ITO
void receber_resposta();							// Trata mensagem de ERRO/OK
void mandar_mensagem(int ito, char* msg);			// Monta pacote de mensagem
bool contato_esta_vazio(); 							// (AUX)Verifica se dado contato está vazio
void print_contatos(); 								// Função para imprimir lista de contatos
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
	case 3:
		// SOLICITAÇÃO DE REGISTRO

		contato novoContato;

		if (qtdContatos == NUM_CONTATOS){
			mandar_mensagem(3, "ERRO: MAXIMO DE CLIENTES ATINGIDO!");
			return;
		}

		// captura e valida dados
		char* nome	= strtok(NULL, DELIMITER);
		char* ip	= strtok(NULL, DELIMITER);
		char* porta	= strtok(NULL, DELIMITER);

		if (nome == NULL || ip == NULL || porta == NULL){
			mandar_mensagem(3, "ERRO: FALHA AO CAPTURAR UM TOKEN!");
			return;
		}

		// adiciona contato a lista de contatos
		qtdContatos++;

		contato novoContato;
		sprintf(novoContato.nome, "%s", nome);
		sprintf(novoContato.ip, "%s", ip);
		novoContato.porta	= atoi(porta);

		lista_contatos[qtdContatos-1] = novoContato;
	case 2:
		// DEVOLVE LISTA

		char str_lista[] = "";

		for(int i=0; i<qtdContatos; i++){
			char tmp[40];
			sprintf(tmp, "|%s|%s|%d", lista_contatos[i].nome, lista_contatos[i].ip, lista_contatos[i].porta);
			strcat(str_lista, tmp);
		}

		mandar_mensagem(1, str_lista);
		return;
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
	                mandar_mensagem(ERRO, "ERRO");
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

    for (int i = 0; i < qtdContatos; i++) {
        if (send(client_sockets[i], package, strlen(package), 0) == -1) {
            perror("Erro ao enviar mensagem broadcast");
        }
    }
}

void desconectar_cliente(int client_socket) {
    // Enviar mensagem de confirmação de desconexão
    mandar_mensagem(OK, "Desconectado com sucesso");

    // Fechar o socket do cliente
    close(client_socket);

    // Remover o cliente da lista de clientes
    for (int i = 0; i < qtdContatos; i++) {
        if (client_sockets[i] == client_socket) {
            // Desloca os outros sockets para preencher o espaço do cliente desconectado
            for (int j = i; j < qtdContatos - 1; j++) {
                client_sockets[j] = client_sockets[j + 1];
            }
            qtdContatos--;
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

void mandar_mensagem(int ito, char* msg){
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
		mandar_mensagem(ERRO,"ERRO");
	}else{
		mandar_mensagem(OK,"OK");
		// printar mensagem (função? direto aqui?)
		//mensagem de ok
	}
}
