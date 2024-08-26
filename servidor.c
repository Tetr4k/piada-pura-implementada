
#include "lib/estruturas.h" // => Inclui lista de contatos e porta_escuta_cliente
#include "lib/socket.h"
#include "lib/tratamento_msg.h"
#include "lib/socketEnvia.h"
#include "lib/socketEscuta.h"

#include <signal.h>

int run = 1;
void handle_sigint(int sig) {
    run = 0;
}

int main()  {

    init_mutexes();
    init_meu_contato("Servidor", PORTA_SERVIDOR);

    // inicializar o socket de escuta
    int sock_escuta = socket_escuta(PORTA_SERVIDOR);
    
    // Escuta o sinal de ctrl+c
    signal(SIGINT, handle_sigint);
    printf("Pressione CTRL+C para interromper o servidor\n\n");

    while(run){
    }

    close(sock_escuta);
    destroy_mutexes();
    return 0;
}