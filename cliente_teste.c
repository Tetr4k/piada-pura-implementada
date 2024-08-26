#include "lib/estruturas.h" // => Inclui lista de contatos e porta_escuta_cliente
#include "lib/socket.h"
#include "lib/tratamento_msg.h"
#include "lib/socketEnvia.h"
#include "lib/socketEscuta.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Uso: %s <IP do Servidor> <Seu_nome> <Sua_Porta>\n", argv[0]);
        return 1;
    }

    init_mutexes();

    char *server_ip = argv[1];
    char *nome = argv[2];
    int porta_escuta = atoi(argv[3]);
    char mensagem[MAX_STRING]="\0";

    init_meu_contato(nome, porta_escuta);

    int sock_escuta = socket_escuta(porta_escuta);

    // REGISTRAR
    socket_envia(server_ip, PORTA_SERVIDOR, criar_mensagem(3, ""));

    while(1){
        memset((void *) mensagem,'\0',MAX_STRING);

        printf("Digite sua mensagem\n");
        fgets(mensagem, MAX_STRING, stdin);

        if(strcmp(mensagem, "exit\n")==0)
            break;

        char pacote[MAX_PACOTE] = "";
        strncpy(pacote, criar_mensagem(4, mensagem), MAX_PACOTE);
        //printf("ENVIADO: %s", pacote);
        socket_envia(server_ip, PORTA_SERVIDOR, pacote);

        memset((void *) pacote,'\0',MAX_PACOTE);

        print_contatos();
        print_mensagens();
    }

    // Broadcast
    broadcast_message(criar_mensagem(5, "alou sou um broadcast!"));

    // DESLOGAR
    socket_envia(server_ip, PORTA_SERVIDOR, criar_mensagem(6, ""));
    
    destroy_mutexes();
    close(sock_escuta);
    return 0;
}