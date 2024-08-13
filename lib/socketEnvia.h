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
