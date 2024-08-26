# PIADA PURA Protocol (Protocolo Interativo de Amigos com Diversão A rodo)

Este projeto é uma implementação do **Protocolo Interativo de Amigos com Diversão A rodo** (PIADA PURA), desenvolvido para a matéria de **Sistemas Distribuídos**. O protocolo foi criado para promover a interação divertida entre clientes e um servidor central, permitindo a troca de mensagens de forma leve e descontraída.

## Estrutura do Projeto

- **servidor.c**: Implementação do servidor que gerencia as conexões dos clientes, recebendo e transmitindo mensagens conforme o protocolo PIADA PURA.
- **cliente.c**: Implementação do cliente que se conecta ao servidor, enviando mensagens e recebendo respostas dentro do protocolo.

## Como Compilar

Para compilar os arquivos `servidor.c` e `cliente.c`, você pode usar o GCC (GNU Compiler Collection). Abra o terminal na pasta do projeto e execute os seguintes comandos:

### Compilar o Servidor

```bash
gcc -o servidor servidor.c -pthread
```

### Compilar o Cliente

```bash
gcc -o cliente cliente.c -pthread
```

> Nota: O uso de -pthread é necessário para compilar o servidor corretamente, pois ele utiliza threads para gerenciar múltiplas conexões de clientes.

## Como Executar
### Executando o Servidor
Para iniciar o servidor, utilize o comando abaixo. O servidor ficará aguardando conexões na porta especificada no código:

```bash
./servidor
```

### Executando o Cliente
Para conectar ao servidor, use o seguinte comando, substituindo <IP do Servidor> pelo endereço IP do servidor, <Seu_nome> pelo seu nome de usuário, e <Sua_Porta> pela porta que deseja utilizar:

```bash
./cliente <IP do Servidor> <Seu_nome> <Sua_Porta>
```

Por exemplo, para conectar ao servidor no IP 192.168.1.10 com o nome Joao usando a porta 4444:

```bash
./cliente 192.168.1.10 Joao 4444
```

## Observações
Certifique-se de que o servidor esteja em execução antes de tentar conectar o cliente.
O servidor suporta múltiplas conexões simultâneas, permitindo que vários clientes interajam ao mesmo tempo.

## Agradecimentos
Um abraço especial ao professor Leandro da matéria Tópicos Especiais em Redes e Sistemas Distribuídos por todo o suporte e orientação ao longo do desenvolvimento deste projeto.