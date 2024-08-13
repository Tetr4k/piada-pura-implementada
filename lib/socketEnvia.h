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

/*
Cliente:
"4|000|nome|ip|porta", contato    
int sock = socket_envia(contato.ip, contato.porta, "4|000|nome|ip|porta");
-> socket_envia
    enviar_mensagem(mensagem, sock);    1
    receber_mensagem(mensagem, sock);   5
    5|100|nome|ip|porta|....
    tratar_mensagem(mensagem);          6


Servidor:
-> socket_escuta
    receber_mensagem(mensagem, sock);     2
    tratar_mensagem(mensagem);            3
    5|100|nome|ip|porta|....
    enviar_mensagem(mensagem, sock);      4
*/

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
