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
#define MAX_PLAYER 3

void formatBuff(char* string);
int makeListenSocket(struct sockaddr_in serverAddr, int portNo, int backlog);
void setupPollingServer(int listenSock, struct pollfd* client, int clientArrLen);
void headerFactory(int sockfd, SocketData s_data, User user);
void writeBuff(int sockfd, Header header, char* data);
SocketData readBuff(int sockfd);
int checkLogin(User user);
int checkPlayerNumber();
int createPlayerList(int sockfd, User user);

char buff[1024];
int tranBytes;
User player_list[MAX_PLAYER];

int main()
{
    int listenSock, connSock;
    struct sockaddr_in serverAddr, clientAddr;
    int clientAddrLenght = sizeof(clientAddr);
    int portNo = 5500;

    User user;
    SocketData s_data;

    struct pollfd client[MAX_PLAYER];
    int i, maxi, sockfd, maxfd;
    int rv;
    maxi = 0;

    listenSock = makeListenSocket(serverAddr, portNo, 5);
    setupPollingServer(listenSock, client, MAX_PLAYER);

    while(1){
        rv = poll(client, maxi+1, 60000);
        if(rv == -1){
            perror("Error poll!");
        }else if(rv == 0){
            printf("Timeout occurred! No data after 1min.\n");
        }else{
            if(client[0].revents & POLLRDNORM){
                connSock = accept(listenSock, (struct sockaddr*)&clientAddr, (socklen_t*)&clientAddrLenght);
                for(i = 1; i < MAX_PLAYER; i++){
                    if(client[i].fd < 0){
                        client[i].fd = connSock;
                        break;
                    }
                }
                client[i].events = POLLRDNORM;
                if(i > maxi) maxi = i;
                if(--rv <= 0) continue;
            }
            for(i = 1; i < MAX_PLAYER; i++){
                if((sockfd = client[i].fd) <0)  continue;
                if(client[i].revents & (POLLRDNORM | POLLERR)){
                    formatBuff(buff);
                    tranBytes = read(sockfd, buff, 1024);
                    
                    if(tranBytes <= 0){
                        close(sockfd);
                        client[i].fd = -1;
                    }else{
                        //dosomething
                        printf("CO data\n");
                        s_data = *((struct SocketData *)buff);
                        headerFactory(sockfd, s_data, user);
                        printf("out head\n");
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
        printf("\nBind loi");
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

void writeBuff(int sockfd, Header header, char* data){
    SocketData s_data;
    s_data.header = header;
    memcpy(s_data.data, data, MAX_DATA_LEN);

    formatBuff(buff);
    memcpy(buff, &s_data, sizeof(SocketData));

    tranBytes = write(sockfd, buff, sizeof(SocketData));
    if(tranBytes < 0)   error("Error writing socket!");
}

SocketData readBuff(int sockfd){
    SocketData s_data;
    tranBytes = read(sockfd, buff, 1024);
    if(tranBytes < 0)   error("Error reading socket!");
    s_data = *((struct SocketData *)buff);
    return s_data;
}

int checkPlayerNumber(){
    int i;
    for(i = 0; i < MAX_PLAYER-1; i++){
        if(player_list[i].status != 1){
            printf("player number = %d\n", i);
            return i;
        }
    }
    return MAX_PLAYER-1;
}

int createPlayerList(int sockfd, User user){
    int i = 0;
    while(i < MAX_PLAYER){
        if(player_list[i].status != 1){
            player_list[i].status = 1;
            strcpy(player_list[i].username,user.username);
            strcpy(player_list[i].password,user.password);
            player_list[i].fd = sockfd;
            printf("sock = %d\n", player_list[i].fd);
            break;
        }else{
            i += 1;
        }
    }
    i = checkPlayerNumber();
    if(i == MAX_PLAYER-1){
        return 1; //da du so nguoi choi
    }else{
        return 0; //chua du so nguoi choi, doi nguoi choi moi dang nhap
    }
}

void headerFactory(int sockfd, SocketData s_data, User user){
    int check, i;
    switch(s_data.header){
        case LOG_IN:
            user = *((struct User *)(s_data.data));
            if(checkLogin(user)!=1){
                writeBuff(sockfd, LOG_IN, "LOGIN FAIL");
            }else{
                writeBuff(sockfd, LOG_IN, "LOGIN SUCCESS");
                check = createPlayerList(sockfd, user);
                if(check == 1){
                    for(i = 0; i < MAX_PLAYER; i++){
                        writeBuff(player_list[i].fd, LOG_IN, "GAME START");
                    }
                }else{
                    printf("Chua du ng choi\n");
                }
            }
            break;
        case RQ_ANSWER:
            break;
    }
}

int checkLogin(User user){
    FILE *fp;
    char *token;
    char data[256];
    printf("username = %s\n", user.username);
    printf("password = %s\n", user.password);

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