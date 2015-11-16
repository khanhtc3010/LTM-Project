#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include "struct.h"

void formatBuff(char* string);
int makeConnectSocket(struct sockaddr_in serverAddr, int portNo, char* serverIp);
//void playGame(User user, int sockfd, char* buff);
void playGame(int sockfd);
//int login(User user, int sockfd, char* buff);
int login(int sockfd);
SocketData encoreSocketData(Header header, char* data);
int menu();

int main()
{
	int sockfd;
	int portNo = 5500;
	struct sockaddr_in serverAddr;
	
	int choiceOption;

	sockfd = makeConnectSocket(serverAddr, portNo, "127.0.0.1");

	do{
		//do something
		choiceOption = menu();
		if(choiceOption == 1){
			playGame(sockfd);
			continue;
		}
		if(choiceOption == 2){
			printf("Help\n");
			continue;
		}
		if(choiceOption == 3)	break;
	}while(1);

	close(sockfd);
	return 1;
}


/*FUNCTIONS*/
void formatBuff(char* string){
    int i = 0;
    while(string[i] != '\0'){
        string[i] = '\0';
        i++;
    }
}

int makeConnectSocket(struct sockaddr_in serverAddr, int portNo, char* serverIp){
	int sockfd;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)	error("Error opening socket!");

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(portNo);
	if(inet_pton(AF_INET, serverIp, &serverAddr.sin_addr) <= 0){
		printf("Error setup server!\n");
		exit(0);
	}

	if(connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0){
		printf("Error connect server!\n");
		exit(0);
	}
	return sockfd;
}

int menu(){
	int choice;
	printf("===========1 vs 100==========\n\t1.Play Game\n\t2.Help\n\t3.Exit\n=============================\nChoice: ");
	scanf("%d", &choice);
	return choice;
}

int login(int sockfd){
	int tranBytes;
	SocketData s_data;
	char buff[1024];
	User user;

	printf("ENTER USERNAME:\t");
	scanf(" %[^\n]", user.username);
	printf("ENTER PASSWORD:\t");
	scanf(" %[^\n]", user.password);
	memcpy(buff,&user, sizeof(User));

	s_data = encoreSocketData(LOG_IN, buff);
	formatBuff(buff);
	memcpy(buff,&s_data, sizeof(SocketData));

	tranBytes = write(sockfd, buff, sizeof(SocketData));

	if(tranBytes < 0)	error("Error writing socket!");
	tranBytes = read(sockfd, buff, 1024);
	if(strcmp(buff, "LOGIN SUCCESS") == 0){
		return 1;
	}
	return 0;
}

void playGame(int sockfd){
	if(login(sockfd) == 1){
		printf("Game start!\n");
		return;
	}else{
		printf("Login fail!\n");
		return;
	}
}

SocketData encoreSocketData(Header header, char* data){
	SocketData s_data;
	s_data.header = header;
	memcpy(s_data.data, data, MAX_DATA_LEN);
	return s_data;
}