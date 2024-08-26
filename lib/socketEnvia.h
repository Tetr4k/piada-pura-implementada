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

    if(receber_mensagem(mensagem, sock)<0){
        printf("Erro ao receber mensagem! SOCKET: %d\n", sock);fflush(stdout);
        return 0;
    }
    //Tratamento da resposta
    int ito = tratar_pacote(mensagem);

    // Caso receba erro
    if(!ito) return 0;

    close(sock);
    return 1;
}

void broadcast_message(const char *message) {
    char package[MAX_PACOTE];
    snprintf(package, sizeof(package), "%s", message);

    char buffer_package[MAX_PACOTE] = "";

	pthread_mutex_lock(&mutex_contatos);

    for (int i = 0; i < qtdContatos; i++) {

        if(strcmp(lista_contatos[i].nome, meu_contato.nome) != 0){
            strncpy(buffer_package, package, MAX_PACOTE);
            //printf("BROADCASTING: %s => %s\n", lista_contatos[i].nome, buffer_package);

            if (socket_envia(lista_contatos[i].ip, lista_contatos[i].porta, buffer_package) < 0) {
                perror("Erro ao enviar mensagem broadcast");
            }

            memset(buffer_package, '\0', MAX_PACOTE);
        }
    }

	pthread_mutex_unlock(&mutex_contatos);
}


// criar socket
// enviar mensagem
// processar resposta
