#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <poll.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include "struct.h"
#define max_player 15

void formatBuff(char* string);
int makeListenSocket(struct sockaddr_in serverAddr, int portNo, int backlog);
void setupPollingServer(int listenSock, struct pollfd* client, int clientArrLen);
int checkLogin(User user);

int main()
{
    int listenSock, connSock;
    struct sockaddr_in serverAddr, clientAddr;
    int clientAddrLenght = sizeof(clientAddr);
    int portNo = 5500;
    char recvBuff[1024], sendBuff[1024];
    int tranBytes;

    /*User* user = (User*)calloc(1, sizeof(User));
    SocketData* s_data = (SocketData*)calloc(1, sizeof(SocketData));*/
    User user;
    SocketData s_data;

    struct pollfd client[max_player];
    int i, maxi, sockfd, maxfd;
    int rv;
    maxi = 0;

    listenSock = makeListenSocket(serverAddr, portNo, 5);
    setupPollingServer(listenSock, client, max_player);

    while(1){
        rv = poll(client, maxi+1, 60000);
        if(rv == -1){
            perror("Error poll!");
        }else if(rv == 0){
            printf("Timeout occurred! No data after 1min.\n");
        }else{
            if(client[0].revents & POLLRDNORM){
                connSock = accept(listenSock, (struct sockaddr*)&clientAddr, (socklen_t*)&clientAddrLenght);
                for(i = 1; i < max_player; i++){
                    if(client[i].fd < 0){
                        client[i].fd = connSock;
                        break;
                    }
                }
                client[i].events = POLLRDNORM;
                if(i > maxi) maxi = i;
                if(--rv <= 0) continue;
            }
            for(i = 1; i < max_player; i++){
                if((sockfd = client[i].fd) <0)  continue;
                if(client[i].revents & (POLLRDNORM | POLLERR)){
                    //dosomething
                    formatBuff(recvBuff);
                    tranBytes = read(sockfd, recvBuff, 1024);
                    
                    if(tranBytes == 0){
                        close(sockfd);
                        client[i].fd = -1;
                    }else{
                        //s_data = *((struct SocketData *)recvBuff);
                        printf("%d\n", s_data.header);
                        user = *((struct User *)(s_data.data));
                        if(checkLogin(user)!=1){
                            tranBytes = write(sockfd, "LOGIN FAIL", 1024);
                        }
                        else{
                            tranBytes = write(sockfd, "LOGIN SUCCESS", 1024);
                        }
                    }
                    if(--rv <= 0)   break;
                }
            }
        }
    }

    close(listenSock);
    return 0;
}


/*FUNCTIONS*/
void formatBuff(char* string){
    int i = 0;
    while(string[i] != '\0'){
        string[i] = '\0';
        i++;
    }
}

int makeListenSocket(struct sockaddr_in serverAddr, int portNo, int backlog){
    int listenSock;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(portNo);

    listenSock = socket(AF_INET, SOCK_STREAM, 0);
    if(listenSock < 0)  error("Error opening socket!");

    if(bind(listenSock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0){
        error("Error binding socket!");
        exit(0);
    }

    if(listen(listenSock, backlog) == -1){
        error("Error listening socket!");
        exit(0);
    }
    return listenSock;
}

void setupPollingServer(int listenSock, struct pollfd* client, int clientArrLen){
    int i;

    client[0].fd = listenSock;
    client[0].events = POLLRDNORM;
    for(i = 1; i < clientArrLen; i++) client[i].fd = -1;
}

int checkLogin(User user){
    FILE *fp;
    char *token;
    char data[256];
    printf("name = %s\n", user.username);
    printf("pass = %s\n", user.password);

    if((fp=fopen("txt_user.txt","r"))==NULL){
        printf("OPEN USER.TXT FAIL\n");
        return 0;
    }

    while(!feof(fp)){
        fgets(data,256,fp);
        if(data[0]=='U' && data[1]=='|'){

            token = strtok(data, "|");
            token = strtok(NULL, "|");
            
            if(strcmp(token,user.username)==0){      
                token = strtok(NULL, "|");
                
                if(strcmp(token,user.password)==0){
                    fclose(fp);
                    return 1;
                }else{
                    fclose(fp);
                    return 0;
                }           
            }
        }
    }
    fclose(fp);
    return 0;
}