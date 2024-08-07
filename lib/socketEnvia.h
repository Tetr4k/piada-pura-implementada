//#include "socket.h"

int conectar(int sock, char *IP, int porta){
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(IP);
    addr.sin_port = htons(porta);

    // Conectar ao servidor
    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        printf("\nErro ao conectar\n");
        return -1;
    }

    return 1;
}

int socket_envia(char* server_ip, int porta_servidor, char* mensagem){
    int sock = criar_socket(0);
    
    if(conectar(sock, server_ip, porta_servidor)<0) 
        return -1;

    // Aqui vem a criação das mensagens
    enviar_mensagem(mensagem, sock);
    receber_mensagem(mensagem, sock);
    //Tratamento da resposta

    close(sock);
    return 1;
}


// criar socket
// enviar mensagem
// processar resposta
