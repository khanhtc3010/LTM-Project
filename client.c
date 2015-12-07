#include "myLib.h"
#include "cli_func/setup.h"
#include "cli_func/login.h"
#include "cli_func/user_function.h"

//global var
int isMP = 0;

void playGame(int sockfd);
void headerFactory(int sockfd, SocketData s_data);

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
		if(choiceOption == 3){
			writeBuff(sockfd,EXIT,"");
			break;
		}
	}while(1);

	close(sockfd);
	return 1;
}


/*FUNCTIONS*/

void playGame(int sockfd){
	SocketData s_data;
	if(login(sockfd) == 1){
		printf("Login success!\nWait for other player...\n");
		do{
			s_data = readBuff(sockfd);
			headerFactory(sockfd, s_data);
			if(s_data.header==LOSE){
				break;
			}
		}while(1);
	}else{
		printf("Login fail!\n");
	}
	return;
}

void headerFactory(int sockfd, SocketData s_data){
	switch(s_data.header){
		case START:
			printf("%s\n", s_data.data);
			if(strcmp(s_data.data, "Game start!\nYou are main player!")==0){
				isMP = 1;
			}
			break;
		case LEVEL:
			if(isMP==1){
				writeBuff(sockfd, LEVEL, selectLevel());
			}
			break;
		case QUESTION:
			printf("%s\n", s_data.data);
			writeBuff(sockfd, ANSWER, answer());
			break;
		case LOSE:
			printf("%s\n", s_data.data);
			break;
	}
}