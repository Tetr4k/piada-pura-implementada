#include "lib/socket.h"
#include "lib/socketEnvia.h"
#include "lib/socketEscuta.h"
#include "lib/estruturas.h"

#include <signal.h>

contato lista_contatos[NUM_CONTATOS];

int run = 1;
void handle_sigint(int sig) {
    run = 0;
}

int main(int argc, char *argv[]) {
    char mensagem[MAX_STRING]="\0";
    int sock_escuta = socket_escuta(PORTA_SERVIDOR);

    signal(SIGINT, handle_sigint);
    printf("Pressione CTRL+C para interromper o servidor\n\n");

    while(run){
    }

    close(sock_escuta);
    return 0;
}