#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "./estruturas.h"
#include "./defines.h"
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

char mensagem[TAM_MENSAGEM];

void limpaTela(){
    #ifdef __linux__
        system("clear");
    #elif _WIN32
        system("cls");
    #else
         //Outros sistemas 
    #endif
}

//Função para limpar o buffer de entrada
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
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
    
    printf("Digite uma mensagem: ");
    limpar_buffer();
    fgets(mensagem, TAM_MENSAGEM, stdin); 
    //func enviar_mensagem(mensagem)
    printf("Mensagem enviada!\n");    
}

void opcao_2() {
    limpaTela();

    printf("Digite uma mensagem: ");
    limpar_buffer();
    fgets(mensagem, TAM_MENSAGEM, stdin); 
    //func enviar_mensagem_broadcast(mensagem)
    printf("Mensagem enviada!\n");
}

void opcao_3() {
    limpaTela();
    printf("\nLista de contatos online:\n");
    //print_contatos();
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


    //SERVIDOR.c (add contato)
    printf("Você foi conectado ao servidor.\n");
    printf("\nLista de contatos online:\n");
    //print_contatos();

    do {
        mostrar_menu();
        printf("\nEscolha uma opção: ");
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
