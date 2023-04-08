#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

#define CONNECTION_PORT 3500

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

    int num;
    char a[10];

    printf("Enter first number : ");
    scanf("%d",&num);
    sprintf(a,"%d",num);
    write(socketDescriptor,a,strlen(a));
    
    printf("Enter Second number : ");
    scanf("%d",&num);
    sprintf(a,"%d",num);
    write(socketDescriptor,a,strlen(a));

    char recievedMessage[100];
    read(socketDescriptor,recievedMessage,100);
    printf("%s\n",recievedMessage);
    
    close(socketDescriptor);
    return 0;
}