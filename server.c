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
#define max_player 5

void formatBuff(char* string);
int makeListenSocket(struct sockaddr_in serverAddr, int portNo, int backlog);
void setupPollingServer(int listenSock, struct pollfd* client, int clientArrLen);
void headerFactory(int sockfd, SocketData s_data, User user, int* player_list);
void writeBuff(int sockfd, Header header, char* data);
SocketData readBuff(int sockfd);
int checkLogin(User user);
int checkPlayerNumber(int* player_list);
int createPlayerList(int* player_list);

char buff[1024];
int tranBytes;

int main()
{
    int listenSock, connSock;
    struct sockaddr_in serverAddr, clientAddr;
    int clientAddrLenght = sizeof(clientAddr);
    int portNo = 5500;

    User user;
    int player_list[max_player];
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
                    formatBuff(buff);
                    tranBytes = read(sockfd, buff, 1024);
                    
                    if(tranBytes == 0){
                        close(sockfd);
                        client[i].fd = -1;
                    }else{
                        //dosomething
                        s_data = *((struct SocketData *)buff);
                        headerFactory(sockfd, s_data, user, player_list);
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

int checkPlayerNumber(int* player_list){
    int i;
    for(i = 0; i < max_player; i++){
        if(player_list[i] != 0){
            return i;
        }
    }
    return max_player;
}

int createPlayerList(int* player_list){
    int i = 0;
    while(i < max_player){
        if(player_list[i] != 0){
            player_list[i] = 0;
            break;
        }else{
            i += 1;
        }
    }
    i = checkPlayerNumber(player_list);
    if(i == max_player){
        return 1; //da du so nguoi choi
    }else{
        return 0; //chua du so nguoi choi, doi nguoi choi moi dang nhap
    }
}

void headerFactory(int sockfd, SocketData s_data, User user, int* player_list){
    int check;
    switch(s_data.header){
        case LOG_IN:
            user = *((struct User *)(s_data.data));
            if(checkLogin(user)!=1){
                writeBuff(sockfd, LOG_IN, "LOGIN FAIL");
            }else{
                writeBuff(sockfd, LOG_IN, "LOGIN SUCCESS");
                check = createPlayerList(player_list);
                if(check == 1)  printf("\nGame start!\n");
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