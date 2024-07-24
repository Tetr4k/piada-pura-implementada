// No codeblocks inclua no menu em: Project -> Build Options... -> Linker settings -> Other link options -l wsock32
//#define WIN // Se não for no windows comente essa linha e compile no terminal: gcc -o tc tc.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#ifdef WIN
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

#define TAM_MENSAGEM 255     /* mensagem de maior tamanho */

#define PORTA_SERVIDOR_TCP 9999

#define MAXPENDING 5

int criar_socket(int porta)
{
    int sock;
    struct sockaddr_in endereco; /* Endereço Local */

    /* Criação do socket datagrama/UDP para recepção e envio de pacotes */
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\nErro na criação do socket!\n");fflush(stdout);
        return(-1);
    }

    if (porta > 0)
    {
        /* Construção da estrutura de endereço local */
        memset(&endereco, 0, sizeof(endereco));       /* Zerar a estrutura */
        endereco.sin_family      = AF_INET;           /* Família de endereçamento da Internet */
        endereco.sin_addr.s_addr = htonl(INADDR_ANY); /* Qualquer interface de entrada */
        endereco.sin_port        = htons(porta);      /* Porta local */

        /* Instanciar o endereco local */
        if (bind(sock, (struct sockaddr *) &endereco, sizeof(endereco)) < 0)
        {
           printf("\nErro no bind()!\n");fflush(stdout);
           return(-1);
        }

        /* Indica que o socket escutara as conexões */
        if (listen(sock, MAXPENDING) < 0)
        {
           printf("\nErro no listen()!\n");fflush(stdout);
           return(-1);
        }

    }

    return(sock);
}

int conectar_com_servidor(int sock,char *IP,int porta)
{
    struct sockaddr_in endereco; /* Endereço Local */

    /* Construção da estrutura de endereço do servidor */
    memset(&endereco, 0, sizeof(endereco));   /* Zerar a estrutura */
    endereco.sin_family      = AF_INET;       /* Família de endereçamento da Internet */
    endereco.sin_addr.s_addr = inet_addr(IP); /* Endereço IP do Servidor */
    endereco.sin_port        = htons(porta);  /* Porta do Servidor */

    /* Estabelecimento da conexão com o servidor de echo */
    if (connect(sock, (struct sockaddr *) &endereco, sizeof(endereco)) < 0)
    {
        printf("\nErro no connect()!\n");fflush(stdout);
        return(-1);
    }
    return(0);
}

int enviar_mensagem(char *mensagem,int sock)
{
    /* Envia o conteúdo da mensagem para o cliente */
    if (send(sock, mensagem, strlen(mensagem), 0) != strlen(mensagem))
    {
        printf("\nErro no envio da mensagem\n");fflush(stdout);
        return(-1);
    }

    printf("\nTCP Cliente: Enviei (%s)\n",mensagem);fflush(stdout);

    return(0);
}

int receber_mensagem(char *mensagem,int sock)
{
    /* Limpar o buffer da mensagem */
    memset((void *) mensagem,(int) NULL,TAM_MENSAGEM);

    /* Espera pela recepção de alguma mensagem do cliente conectado*/
    if (recv(sock, mensagem, TAM_MENSAGEM, 0) < 0)
    {
        printf("\nErro na recepção da mensagem\n");fflush(stdout);
        return(-1);
    }

    printf("\nTCP Cliente: Recebi (%s)\n",mensagem);fflush(stdout);

    return(0);
}


int main(int argc, char *argv[])
{
    int  sock;                   /* Socket */
    int  resultado;              /* Resultado das funções */
    char mensagem[TAM_MENSAGEM]; /* Buffer para a recepção da string de echo */
    char IP[TAM_MENSAGEM];       /* Endereço IP do servidor */
#ifdef WIN
    WORD wPackedValues;
    WSADATA  SocketInfo;
    int      nLastError,
	         nVersionMinor = 1,
	         nVersionMajor = 1;
    wPackedValues = (WORD)(((WORD)nVersionMinor)<< 8)|(WORD)nVersionMajor;
    nLastError = WSAStartup(wPackedValues, &SocketInfo);
#endif

    if (argc != 3)    /* Testa se o número de parâmetros está correto */
    {
        printf("Uso: %s <IP Servidor> <Palavra de Echo>\n", argv[0]);
        return(1);
    }
    memset((void *) IP      ,(int) NULL,TAM_MENSAGEM);
    strcpy(IP      ,argv[1]); /* IP Servidor */
    memset((void *) mensagem,(int) NULL,TAM_MENSAGEM);
    strcpy(mensagem,argv[2]);

    sock = criar_socket(0);
    if (sock < 0)
    {
        printf("\nErro na criação do socket!\n");
        return(1);
    }

    resultado = conectar_com_servidor(sock,IP,PORTA_SERVIDOR_TCP);
    if (resultado < 0)
    {
        printf("\nErro na conexão com o servidor\n");
        return(1);
    }

    resultado = enviar_mensagem(mensagem,sock);
    if (resultado < 0)
    {
        printf("\nErro no envio da mensagem\n");
        return(1);
    }

    /* Recebendo como resposta a mesma string vinda do servidor */
    resultado = receber_mensagem(mensagem,sock);
    if (resultado < 0)
    {
        printf("\nErro no recebimento da mensagem\n");
        return(1);
    }

    close(sock);

    return(0);
}
