#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

#define CONNECTION_PORT 4444

int main(){
    //creating socket
    int socketDescriptor=socket(AF_INET,SOCK_STREAM,0);
     if(socketDescriptor<0){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    //connect()ing
    struct sockaddr_in serverAdress;
    serverAdress.sin_addr.s_addr=INADDR_ANY;
    serverAdress.sin_family=AF_INET;
    serverAdress.sin_port=htons(CONNECTION_PORT);
    serverAdress.sin_zero[8]='\0';
    int status=connect(socketDescriptor,(struct sockaddr*)&serverAdress,sizeof(serverAdress));
    if(status<0){
        perror("Couldn't connect with the server");
        exit(EXIT_FAILURE);
    }

    while(1){
    //writing(sending message to server)
    char message[80];
    printf("Enter a message : ");
    fgets(message,80,stdin);
    write(socketDescriptor,message,strlen(message));

    //receiving message or reading
    char recievedMessage[100];
    read(socketDescriptor,recievedMessage,100);
    printf("%s",recievedMessage);
    }
    close(socketDescriptor);
    return 0;
}