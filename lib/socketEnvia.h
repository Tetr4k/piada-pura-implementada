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


int socket_envia(char* dst_ip, int dst_porta, char* mensagem){
    int sock = criar_socket(0);
    
    if(conectar(sock, dst_ip, dst_porta)<0) 
        return -1;

    // Mensagem já foi construida fora do socket
    enviar_mensagem(mensagem, sock);

    /*
    // Com tratar_mensagem
    receber_mensagem(mensagem, sock);
    tratar_mensagem(mensagem);
    */

    //Sem tratamento da resposta
    receber_mensagem(mensagem, sock);

    close(sock);
    return 1;
}


// criar socket
// enviar mensagem
// processar resposta
