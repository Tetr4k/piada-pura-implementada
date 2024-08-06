#include "lib/socket.h"
#include "lib/socketEnvia.h"
#include "lib/socketEscuta.h"
#include "lib/estruturas.h"

int idx_contatos = 0;
contato lista_contatos[NUM_CONTATOS];


int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Uso: %s <IP do Servidor> <Porta> <Sua_Porta>\n", argv[0]);
        return 1;
    }

    char *server_ip = argv[1];
    int porta_servidor = atoi(argv[2]);
    int porta_escuta = atoi(argv[3]);
    char mensagem[MAX_STRING]="\0";

    
    int sock_escuta = socket_escuta(porta_escuta);

    while(1){
        memset((void *) mensagem,'\0',MAX_STRING);

        printf("Digite sua mensagem\n");
        fgets(mensagem, MAX_STRING, stdin);

        if(strcmp(mensagem, "exit\n")==0)
            break;

        socket_envia(server_ip, porta_servidor, mensagem);

    }

    close(sock_escuta);
    return 0;
}