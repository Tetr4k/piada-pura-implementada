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
    tratar_pacote(mensagem);          6


Servidor:
-> socket_escuta
    receber_mensagem(mensagem, sock);     2
    tratar_pacote(mensagem);            3
    5|100|nome|ip|porta|....
    enviar_mensagem(mensagem, sock);      4
*/

int socket_envia(char* dst_ip, int dst_porta, char* mensagem){
    int sock = criar_socket(0);
    
    if(conectar(sock, dst_ip, dst_porta)<0) 
        return -1;

    // Aqui vem a criação das mensagens
    enviar_mensagem(mensagem, sock);
    receber_mensagem(mensagem, sock);
    tratar_pacote(mensagem);
    //Tratamento da resposta

    close(sock);
    return 1;
}

void broadcast_message(const char *message) {
    char package[MAX_PACOTE];
    snprintf(package, sizeof(package), "%s", message);

	pthread_mutex_lock(&mutex_contatos);

    for (int i = 0; i < qtdContatos; i++) {
        if (socket_envia(lista_contatos[i].ip, lista_contatos[i].porta, package) < 0) {
            perror("Erro ao enviar mensagem broadcast");
        }
    }

	pthread_mutex_unlock(&mutex_contatos);
}


// criar socket
// enviar mensagem
// processar resposta
