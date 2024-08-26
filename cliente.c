#include "lib/estruturas.h" // => Inclui lista de contatos e porta_escuta_cliente
#include "lib/socket.h"
#include "lib/tratamento_msg.h"
#include "lib/socketEnvia.h"
#include "lib/socketEscuta.h"
#include "lib/tela.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Uso: %s <IP do Servidor> <Seu_nome> <Sua_Porta>\n", argv[0]);
        return 1;
    }

    // Inicializar a thread do socket de escuta
    // Enviar as mensagens, criando a thread do socketEnvia

    // Inicia a thread tela
    // Possui lista de contato

    // possui opções para as operações
    // 1. mensagem direta
    // 2. broadcast
    // 3. desconectar
    // 4. atualizar tela

    init_mutexes();

    char *server_ip = argv[1];
    char *nome = argv[2];
    int porta_escuta = atoi(argv[3]);
    char mensagem[MAX_STRING]="\0";

    init_meu_contato(nome, porta_escuta);

    int sock_escuta = socket_escuta(porta_escuta);

    // REGISTRAR
    if(!socket_envia(server_ip, PORTA_SERVIDOR, criar_mensagem(3, ""))){
        printf("Erro ao se conectar com o servidor!\n");
        exit(0);
    }
    printf("Você foi conectado ao servidor.\n");

    printf("\nLista de contatos online:\n");
    print_contatos();

    int escolha;

    do {
        mostrar_menu();
        printf("\nEscolha uma opção: ");
        scanf("%d", &escolha);
        getchar();

        switch (escolha) {
            case 1:
                opcao_1();
                break;
            case 2:
                opcao_2();
                break;
            case 3:
                opcao_3();
                break;
            case 4:
                limpaTela();
                print_mensagens();
                break;
            case 0:
                limpaTela();
                printf("Fechando programa...\n");
                break;
            default:
                limpaTela();
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (escolha != 0);
    

    // DESLOGAR
    socket_envia(server_ip, PORTA_SERVIDOR, criar_mensagem(6, ""));
    
    destroy_mutexes();
    close(sock_escuta);
    return 0;
}