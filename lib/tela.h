#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
/*
#ifdef WIN
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#endif
*/

#define TAM_MENSAGEM 255    
#define PORTA_SERVIDOR 8888

void limpaTela(){
    #ifdef __linux__
        system("clear");
    #elif _WIN32
        system("cls");
    #else

    #endif
}

void mostrar_menu() {
    printf("\nMenu:\n");
    printf("1. Enviar mensagem\n");
    printf("2. Enviar mensagem Broadcast\n");
    printf("3. Lista de usuários online\n");
    printf("0. Sair\n");
}

void opcao_1() {
    limpaTela();
    //func enviar_mensagem()
    printf("Você escolheu a Opção 1.\n");
}

void opcao_2() {
    limpaTela();
    //func mensagem_broadcast()
    printf("Você escolheu a Opção 2.\n");
}

void opcao_3() {
    limpaTela();
    //func receber_lista()
    printf("Você escolheu a Opção 3.\n");
}

int main(int argc, char *argv[])  {

    int  sock;                   // Socket 
    int  resultado;              // Resultado das funções 
    char nome[TAM_MENSAGEM];     // Nome do usuário
    char IP[TAM_MENSAGEM];       // Endereço IP do servidor 
    int porta;
    int escolha;

    /* #ifdef WIN
      WORD wPackedValues;
      WSADATA  SocketInfo;
      int      nLastError,
             nVersionMinor = 1,
             nVersionMajor = 1;
      wPackedValues = (WORD)(((WORD)nVersionMinor)<< 8)|(WORD)nVersionMajor;
      nLastError = WSAStartup(wPackedValues, &SocketInfo);
    #endif */

    if (argc != 4) {
        printf("Uso: %s <nome> <porta> <ipServidor>\n", argv[0]);
        return 1;
    }

    memset((void *) IP, 0, TAM_MENSAGEM); // Limpa as variáveis com zeros
    strcpy(IP, argv[3]);    // IP do servidor
    memset((void *) nome, 0, TAM_MENSAGEM);
    strcpy(nome, argv[1]);  // Nome do usuário
    porta = atoi(argv[2]);  // Converte a string para inteiro

    printf("Nome: %s\n", nome);
    printf("Porta: %d\n", porta);
    printf("IP do Servidor: %s\n", IP); 

    //func de registro (? 

    do {
        mostrar_menu();
        printf("Escolha uma opção: ");
        scanf("%d", &escolha);

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
            case 0:
                limpaTela();
                printf("Fechando programa...\n");
                break;
            default:
                limpaTela();
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (escolha != 0);

  return 0;
}
