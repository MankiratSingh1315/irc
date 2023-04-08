#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define CONNECTION_PORT 3500

int main()
{
    // creating socket
    int socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketDescriptor < 0)
    {
        perror("Socket not formed");
        exit(EXIT_FAILURE);
    }

    // setting socket options
    int option_value = 1;
    int status = setsockopt(socketDescriptor, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(option_value));
    if (status < 0)
    {
        perror("Socket options unable to set");
        exit(EXIT_FAILURE);
    }

    // binding
    struct sockaddr_in serverAddress;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(CONNECTION_PORT);
    serverAddress.sin_zero[8] = '\0';
    status = bind(socketDescriptor,(struct sockaddr*)&serverAddress, sizeof(struct sockaddr));
    if (status < 0)
    {
        perror("Couldn't bind");
        exit(EXIT_FAILURE);
    }

    // listening
    status = listen(socketDescriptor, 4);
    if (status < 0)
    {
        perror("Couldn't listen");
        exit(EXIT_FAILURE);
    }

    // accepting connection
    struct sockaddr_in clientAdress;
    int lengthOfConnectionAddress = sizeof(clientAdress);
    int clientSocket = accept(socketDescriptor, (struct sockaddr*)&clientAdress, &lengthOfConnectionAddress);
    if (clientSocket < 0)
    {
        perror("Couldn't connect with client");
        exit(EXIT_FAILURE);
    }

    char RecievedNum[4];
    int sum=0;
    read(clientSocket,RecievedNum,sizeof(RecievedNum));
    sum+=atoi(RecievedNum);
    read(clientSocket,RecievedNum,sizeof(RecievedNum));
    sum+=atoi(RecievedNum);
    char sumtosend[10];
    
    char message[50];
    sprintf(message,"Sum of numbers sent by server is %d",sum);
    send(clientSocket, message, strlen(message), 0);

    close(socketDescriptor);
    close(clientSocket);
    return 0;
}