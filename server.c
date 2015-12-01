#include "myLib.h"
#include "ser_func/setup.h"
#include "ser_func/login.h"
#include "ser_func/player_list.h"

void headerFactory(int sockfd, SocketData s_data, User* player_list);
//User player_list[MAX_PLAYER];

int main()
{
    int listenSock, connSock;
    struct sockaddr_in serverAddr, clientAddr;
    int clientAddrLenght = sizeof(clientAddr);
    int portNo = 5500;

    SocketData s_data;
    User player_list[MAX_PLAYER];

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
                        printf("//-----recv data---------\n");
                        s_data = *((struct SocketData *)buff);
                        headerFactory(sockfd, s_data, player_list);
                        printf("//-----------------------\n");
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

void headerFactory(int sockfd, SocketData s_data, User* player_list){
    int check, i;
    User user;
    switch(s_data.header){
        case LOG_IN:
            user = *((struct User *)(s_data.data));
            if((checkLogin(user)!=1)||(checkGameStatus(player_list)==1)){
                writeBuff(sockfd, ERROR, "LOGIN FAIL");
            }else{
                writeBuff(sockfd, LOG_IN, "LOGIN SUCCESS");
                check = appendPlayer(sockfd, user, player_list);
                if(check == 1){
                    setMainPlayer(player_list);
                    for(i = 1; i < MAX_PLAYER; i++){
                        if(player_list[i].status == 2){
                            writeBuff(player_list[i].fd, START, "Game start!\nYou are main player!");
                        }else{
                            writeBuff(player_list[i].fd, START, "Game start!");
                        }
                    }
                }else{
                    printf("Chua du ng choi\n");
                }
            }
            break;
        case EXIT:
            if(removePlayer(sockfd, player_list) == 0){
                close(sockfd);
                printf("Disconnect player on socket = %d\n", sockfd);
            }else{
                
            }
            break;
    }
}