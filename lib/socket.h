#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef WIN
#include <winsock2.h>
#include <windows.h>    // API do windows para lidar com threads
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

#include <pthread.h>
#include "defines.h"

#include <ifaddrs.h>

void get_ip(char* buffer_ip){
    struct ifaddrs *interfaces = NULL;
    struct ifaddrs *temp_addr = NULL;
    int success = 0;

    // Recupera as interfaces de rede do sistema
    success = getifaddrs(&interfaces);
    if (success == 0) {
        // Itera sobre as interfaces e procura por uma IPv4
        for (temp_addr = interfaces; temp_addr != NULL; temp_addr = temp_addr->ifa_next) {
            if (temp_addr->ifa_addr->sa_family == AF_INET) {
                // Ignora o loopback interface
                if (strcmp(temp_addr->ifa_name, "lo") != 0) {
                    // Copia o endereço IP
                    strcpy(buffer_ip, inet_ntoa(((struct sockaddr_in *)temp_addr->ifa_addr)->sin_addr));
                    break;
                }
            }
        }
    }

    // Libera a memória alocada para as interfaces
    freeifaddrs(interfaces);
}

void init_meu_contato(char* nome, int porta_escuta){
    // Variavel global para outras funções
    meu_contato.porta = porta_escuta;
    get_ip(meu_contato.ip);
    strncpy(meu_contato.nome, nome, MAX_NAME);

    printf("Nome: %s\nIP: %s\nPorta: %d\n\n", meu_contato.nome, meu_contato.ip, meu_contato.porta); 
}


// Funções de sockets
int criar_socket(int porta)
{
    int sock;
    struct sockaddr_in endereco; /* Endere�o Local */

    /* Cria��o do socket TCP para recep��o e envio de pacotes */
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\nErro na criacao do socket!\n");fflush(stdout);
        return(-1);
    }

    if (porta > 0)
    {
        /* Construçao da estrutura de endereço local */
        memset(&endereco, 0, sizeof(endereco));       /* Zerar a estrutura */
        endereco.sin_family      = AF_INET;           /* Família de endereçamento da Internet */
        endereco.sin_addr.s_addr = htonl(INADDR_ANY); /* Qualquer interface de entrada */
        endereco.sin_port        = htons(porta);      /* Porta local */

        /* Instanciar o endereco local */
        if (bind(sock, (struct sockaddr *) &endereco, sizeof(endereco)) < 0)
        {
           printf("\nErro no bind()!\n");fflush(stdout);
           return(-1);
        }

        /* Indica que o socket escutara as conex�es */
        if (listen(sock, 0) < 0)
        {
           printf("\nErro no listen()!\n");fflush(stdout);
           return(-1);
        }

    }

    return(sock);
}

int receber_mensagem(char *mensagem, int sock)
{
    /* Limpar o buffer da mensagem */
    memset((void *) mensagem, '\0', MAX_PACOTE);

    /* Espera pela recep��o de alguma mensagem do cliente conectado*/
    if (recv(sock, mensagem, MAX_PACOTE, 0) < 0)
    {
        printf("\nErro na recepção da mensagem\n");fflush(stdout);
        return(-1);
    }

    //printf("Recebido: %s\n", mensagem);
    return(0);
}

int enviar_mensagem(char *mensagem, int sock)
{
    /* Devolve o conte�do da mensagem para o cliente */
    if (send(sock, mensagem, strlen(mensagem), 0) != strlen(mensagem))
    {
        printf("\nErro no envio da mensagem\n");fflush(stdout);
        return(-1);
    }

    //printf("Enviado: %s\n", mensagem);

    return(0);
}

int aceitar_conexao(int sock)
{
    int                socket_cliente;
    struct sockaddr_in endereco; /* Endere�o Local */
    int                tamanho_endereco;

    /* Define o tamanho do endere�o de recep��o e envio */
    tamanho_endereco = sizeof(endereco);

    /* Aguarda pela conex�o de um cliente */
    if ((socket_cliente = accept(sock, (struct sockaddr *) &endereco, (unsigned int*) &tamanho_endereco)) < 0)
    {
        printf("\nErro no accept()!\n");fflush(stdout);
        return(0);
    }
    return(socket_cliente);
}

// ==============================================
// Funções para Threads


// Definição dos mutexes
pthread_mutex_t mutex_contatos, mutex_mensagens;

int init_mutexes(){
    if (pthread_mutex_init(&mutex_contatos, NULL) != 0) {
        printf("Erro ao inicializar o mutex\n");
        return 0;
    }

    if (pthread_mutex_init(&mutex_mensagens, NULL) != 0) {
        printf("Erro ao inicializar o mutex\n");
        return 0;
    }

    return 1;
}

int destroy_mutexes(){
    pthread_mutex_destroy(&mutex_contatos);
    pthread_mutex_destroy(&mutex_mensagens);
}

int cria_thread(void* (*function)(void*), void* param) {

    #ifdef WIN
    HANDLE thread;
    DWORD threadId;

    // Cria uma nova thread
    thread = CreateThread(
        NULL,               // Atributos de segurança
        0,                  // Tamanho da pilha (0 = tamanho padrão)
        function,           // Função que a thread vai executar
        param,              // Parâmetro passado para a função
        0,                  // Opções de criação
        &threadId           // ID da thread
    );

    if (thread == NULL) {
        printf("Erro ao criar a thread\n");
        free(param);
        return -1;
    }

    #else
    pthread_t thread;

    // Cria uma nova thread
    if (pthread_create(&thread, NULL, function, param)) {
        printf("Erro ao criar a thread\n");
        free(param);
        return -1;
    }
    pthread_detach(thread);  // Detach a thread para que seus recursos sejam liberados automaticamente ao terminar
    #endif

    return 1;
}