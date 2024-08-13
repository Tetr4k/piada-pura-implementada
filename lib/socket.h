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
    memset((void *) mensagem, '\0', MAX_STRING);

    /* Espera pela recep��o de alguma mensagem do cliente conectado*/
    if (recv(sock, mensagem, MAX_STRING, 0) < 0)
    {
        printf("\nErro na recepção da mensagem\n");fflush(stdout);
        return(-1);
    }

    printf("Recebido: %s\n", mensagem);
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

    printf("Enviado: %s\n", mensagem);

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