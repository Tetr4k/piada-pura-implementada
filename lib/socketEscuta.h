//#include "socket.h"


void* thread_socket_comunicacao(void* sock){
    // Le a mensagem
    // Trata ela de maneira correspondente
    // responde
    // fecha o socket

    int socket_cliente = *((int*)sock);
    //free(sock);

    char mensagem[MAX_STRING];    /* Buffer para a recepção da mensagem*/

    if(receber_mensagem(mensagem, socket_cliente)<0){
        printf("Erro ao receber mensagem! SOCKET: %d\n", socket_cliente);fflush(stdout);
        return NULL;
    }

    enviar_mensagem("OK!", socket_cliente);
    close(socket_cliente);
}


void* thread_socket_escuta(void* sock_escuta){
    // criar socket
    // receber mensagem
    // tratar mensagem
    // responder

    int sock = *((int*)sock_escuta);
    printf("sock: %d\n", sock);
    //free(sock_escuta);

    for (;;) {
        // Aguarda conexão e cria um socket para se comunicar com o cliente
        int socket_cliente = aceitar_conexao(sock);
        cria_thread(thread_socket_comunicacao, socket_cliente);
    }

    close(sock);
    return 0;
}

int socket_escuta(int porta){

    int  socket_escuta;           /* Socket */
    int  socket_cliente;          /* Socket de conex�o com o cliente */
    int  resultado;               /* Resultado das fun��es */

    #ifdef WIN
        WORD wPackedValues;
        WSADATA  SocketInfo;
        int      nLastError,
                nVersionMinor = 1,
                nVersionMajor = 1;
        wPackedValues = (WORD)(((WORD)nVersionMinor)<< 8)|(WORD)nVersionMajor;
        nLastError = WSAStartup(wPackedValues, &SocketInfo);
    #endif

    // criar socket escuta
    socket_escuta = criar_socket(porta);
    if (socket_escuta < 0){
        printf("\nErro na criação do socket de escuta na porta: %d!\n", porta);fflush(stdout);
        return -1;
    }

    printf("1\n");
    cria_thread(thread_socket_escuta, socket_escuta);
    printf("2\n");
    return socket_escuta;
}

