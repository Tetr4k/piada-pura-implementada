#include "defines.h"
//#include "estruturas.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void limpaTela(){
    #ifdef __linux__
        system("clear");
    #elif _WIN32
        system("cls");
    #else

    #endif
}

//Função para limpar o buffer de entrada
void limpar_buffer(char *buffer) {
    memset((void *) buffer,'\0',MAX_STRING);
}

void mostrar_menu() {
    printf("\nMenu:\n");
    printf("1. Enviar mensagem\n");
    printf("2. Enviar mensagem Broadcast\n");
    printf("3. Lista de usuários online\n");
    printf("4. Mostrar mensagens\n");
    printf("0. Sair\n");
}

contato escolhe_contato(){
    contato lista_contatos_copy[NUM_CONTATOS];

    pthread_mutex_lock(&mutex_contatos);
    memcpy(lista_contatos_copy, lista_contatos, NUM_CONTATOS*sizeof(contato));
    pthread_mutex_unlock(&mutex_contatos);

    int op=-1;
    print_contatos();
    do {
        printf("Escolha o contato: ");
        scanf("%d", &op);
        getchar();

        if((op>=0) || (op<qtdContatos))
            return lista_contatos_copy[op];
        
        printf("Contato não existe!\n");
        op=-1;
    } while(op < 0);
    
}

void opcao_1() {
    limpaTela();

    contato contato_dst = escolhe_contato();
    
    char mensagem[MAX_STRING]="\0";
    printf("Digite uma mensagem: ");
    limpar_buffer(mensagem);
    fgets(mensagem, MAX_STRING, stdin);
    //func enviar_mensagem(mensagem)
    
    char pacote[MAX_PACOTE] = "";
    strncpy(pacote, criar_mensagem(4, mensagem), MAX_PACOTE);
    socket_envia(contato_dst.ip, contato_dst.porta, pacote);

    guardar_mensagem(meu_contato.nome, mensagem, 0);
    printf("Mensagem enviada!\n");    
}

void opcao_2() {
    limpaTela();

    char mensagem[MAX_STRING];
    printf("Digite uma mensagem: ");
    limpar_buffer(mensagem);
    fgets(mensagem, MAX_STRING, stdin); 

    //func enviar_mensagem_broadcast(mensagem)
    broadcast_message(criar_mensagem(5, mensagem));
    guardar_mensagem(meu_contato.nome, mensagem, 1);
    
    printf("Mensagem enviada!\n");
}

void opcao_3() {
    limpaTela();
    printf("\nLista de contatos online:\n");
    print_contatos();
}
