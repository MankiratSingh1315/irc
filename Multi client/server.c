#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define CONNECTION_PORT 4444

int main(){
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

    
    struct sockaddr_in newAddr;
    int newSocket;
    pid_t childpid;
    socklen_t addr_size;
    char buffer[1024];

    fd_set currentSocket, readySockets;
    FD_ZERO(&currentSocket);
    FD_SET(socketDescriptor,&currentSocket);

    while(1){
        readySockets=currentSocket;
        if(select(FD_SETSIZE+1,&readySockets/*for reading*/,NULL/*for writing*/,NULL/*for errors*/,NULL/*timeout*/)<0){
            perror("Select error");
            exit(EXIT_FAILURE);
        }
        for(int i=0;i<FD_SETSIZE;i++){
            if(FD_ISSET(i,&readySockets)){
                if(i==socketDescriptor){
                // accepting connection
                newSocket = accept(socketDescriptor, (struct sockaddr*)&newAddr, &addr_size);
                if(newSocket < 0){
                exit(1);
                }
                printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr),ntohs(newAddr.sin_port));
                FD_SET(newSocket,&currentSocket);    
                }else{
                    read(newSocket,buffer,sizeof(buffer));
                    send(newSocket, buffer, strlen(buffer), 0);
                    FD_CLR(i,&currentSocket);
                    
                }
            }
        }
    }
    //     // accepting connection
    //     newSocket = accept(socketDescriptor, (struct sockaddr*)&newAddr, &addr_size);
    //     if(newSocket < 0){
    //         exit(1);
    //     }
    //     printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr),ntohs(newAddr.sin_port));
    //     if((childpid = fork()) == 0){
    //         close(socketDescriptor);

    //      while(1){
    //         recv(newSocket, buffer, 1024, 0);
    //         if(strcmp(buffer, ":exit") == 0){
    //            printf("Disconnected from %s:%d\n",inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
    //            break;
    //         }else{
    //            printf("Client: %s\n", buffer);
    //            send(newSocket, buffer, strlen(buffer), 0);
    //            bzero(buffer, sizeof(buffer));
    //             }
    //         }
    //     }
    // }

    // fd_set currentSocket, readySockets;
    // FD_ZERO(&currentSocket);
    // FD_SET(socketDescriptor,&currentSocket);
    close(socketDescriptor);
    close(newSocket);
    return 0;
}