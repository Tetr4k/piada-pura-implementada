#include "lib/estruturas.h" // => Inclui lista de contatos e porta_escuta_cliente
#include "lib/socket.h"
#include "lib/tratamento_msg.h"
#include "lib/socketEnvia.h"
#include "lib/socketEscuta.h"

void init_meu_contato(int porta_escuta){
    // Variavel global para outras funções
    meu_contato.porta = porta_escuta;
    get_ip(meu_contato.ip);
    strncpy(meu_contato.nome, "cliente", MAX_NAME);

    printf("Nome: %s\nIP: %s\nPorta: %d\n\n", meu_contato.nome, meu_contato.ip, meu_contato.porta); 
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Uso: %s <IP do Servidor> <Porta> <Sua_Porta>\n", argv[0]);
        return 1;
    }

    init_mutexes();

    char *server_ip = argv[1];
    int porta_servidor = atoi(argv[2]);
    int porta_escuta = atoi(argv[3]);
    char mensagem[MAX_STRING]="\0";

    init_meu_contato(porta_escuta);

    printf("%s\n", criar_mensagem(1, "oi alou"));

    contato a = {"matheus", "1.2.3.4", 0001};
    lista_contatos[0] = a;
    contato b = {"juca", "1.2.5.4", 0002};
    lista_contatos[1] = b;
    qtdContatos += 2;

    print_contatos();

    char lista[MAX_STRING] = "";
    cria_lista(lista);
    printf("%s\n", lista);

    printf("%s\n", criar_mensagem(5, lista));

    int sock_escuta = socket_escuta(porta_escuta);

    // while(1){
    //     memset((void *) mensagem,'\0',MAX_STRING);

    //     printf("Digite sua mensagem\n");
    //     fgets(mensagem, MAX_STRING, stdin);

    //     if(strcmp(mensagem, "exit\n")==0)
    //         break;

    //     socket_envia(server_ip, porta_servidor, mensagem);

    // }
    
    destroy_mutexes();
    close(sock_escuta);
    return 0;
}