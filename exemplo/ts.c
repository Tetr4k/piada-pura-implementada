// No codeblocks inclua no menu em: Project -> Build Options... -> Linker settings -> Other link options -l wsock32
//#define WIN // Se n�o for no windows comente essa linha e compile no terminal: gcc -o ts ts.c
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

#define MAXPENDING 5    /* N�mero m�ximo de requisi��es para conex�o pendentes */

int criar_socket(int porta)
{
    int sock;
    struct sockaddr_in endereco; /* Endere�o Local */

    /* Cria��o do socket TCP para recep��o e envio de pacotes */
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\nErro na cria��o do socket!\n");fflush(stdout);
        return(-1);
    }

    if (porta > 0)
    {
        /* Constru��o da estrutura de endere�o local */
        memset(&endereco, 0, sizeof(endereco));       /* Zerar a estrutura */
        endereco.sin_family      = AF_INET;           /* Fam�lia de endere�amento da Internet */
        endereco.sin_addr.s_addr = htonl(INADDR_ANY); /* Qualquer interface de entrada */
        endereco.sin_port        = htons(porta);      /* Porta local */

        /* Instanciar o endereco local */
        if (bind(sock, (struct sockaddr *) &endereco, sizeof(endereco)) < 0)
        {
           printf("\nErro no bind()!\n");fflush(stdout);
           return(-1);
        }

        /* Indica que o socket escutara as conex�es */
        if (listen(sock, MAXPENDING) < 0)
        {
           printf("\nErro no listen()!\n");fflush(stdout);
           return(-1);
        }

    }

    return(sock);
}

int aceitar_conexao(int sock)
{
    int                socket_cliente;
    struct sockaddr_in endereco; /* Endere�o Local */
    int                tamanho_endereco;

    /* Define o tamanho do endere�o de recep��o e envio */
    tamanho_endereco = sizeof(endereco);

    /* Aguarda pela conex�o de um cliente */
    if ((socket_cliente = accept(sock, (struct sockaddr *) &endereco, &tamanho_endereco)) < 0)
    {
        printf("\nErro no accept()!\n");fflush(stdout);
        return(0);
    }
    return(socket_cliente);
}

int receber_mensagem(char *mensagem,int sock)
{
    /* Limpar o buffer da mensagem */
    memset((void *) mensagem,(int) NULL,TAM_MENSAGEM);

    /* Espera pela recep��o de alguma mensagem do cliente conectado*/
    if (recv(sock, mensagem, TAM_MENSAGEM, 0) < 0)
    {
        printf("\nErro na recep��o da mensagem\n");fflush(stdout);
        return(-1);
    }

    printf("\nTCP Servidor: Recebi (%s)\n",mensagem);fflush(stdout);

    return(0);
}

int enviar_mensagem(char *mensagem,int sock)
{
    /* Devolve o conte�do da mensagem para o cliente */
    if (send(sock, mensagem, strlen(mensagem), 0) != strlen(mensagem))
    {
        printf("\nErro no envio da mensagem\n");fflush(stdout);
        return(-1);
    }

    printf("\nTCP Servidor: Enviei (%s)\n",mensagem);fflush(stdout);

    return(0);
}

int main()
{
    int                sock;                   /* Socket */
    int                socket_cliente;         /* Socket de conex�o com o cliente */
    int                resultado;              /* Resultado das fun��es */
    char               mensagem[TAM_MENSAGEM]; /* Buffer para a recep��o da string de echo */
    #ifdef WIN
        WORD wPackedValues;
        WSADATA  SocketInfo;
        int      nLastError,
                nVersionMinor = 1,
                nVersionMajor = 1;
        wPackedValues = (WORD)(((WORD)nVersionMinor)<< 8)|(WORD)nVersionMajor;
        nLastError = WSAStartup(wPackedValues, &SocketInfo);
    #endif

    sock = criar_socket(PORTA_SERVIDOR_TCP);
    if (sock < 0)
    {
        printf("\nErro na cria��o do socket!\n");
        return(1);
    }

    for (;;) /* Loop eterno */
    {
        /* Aguarda por uma conex�o e a aceita criando o socket de contato com o cliente */
        socket_cliente = aceitar_conexao(sock);
        if (socket_cliente == 0)
        {
            printf("\nErro na conexao do socket!\n");
            return(1);
        }

        /* Recebe a mensagem do cliente */
        resultado = receber_mensagem(mensagem,socket_cliente);
        if (resultado < 0)
        {
            printf("\nErro no recebimento da mensagem\n");
            return(1);
        }

        /* Devolve o conte�do da mensagem para o cliente */
        resultado = enviar_mensagem(mensagem,socket_cliente);
        if (resultado < 0)
        {
            printf("\nErro no envio da mensagem\n");
            return(1);
        }

        close(socket_cliente);    /* Fecha o socket do cliente */
    }
    /*n�o passa por aqui */
}
