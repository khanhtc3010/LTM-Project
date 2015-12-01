#include "myLib.h"
#include "cli_func/setup.h"
#include "cli_func/login.h"

void playGame(int sockfd);

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
	int choice;
	if(login(sockfd) == 1){
		printf("Login success!\nWait for other player...\n");
		s_data = readBuff(sockfd);
		if(s_data.header == START){
			printf("%s\n",s_data.data);
			/*if(strcmp(s_data.data, "Game start!\nYou are main player!")==0){
				choice = selectLevel();
			}
			do{
				s_data = readBuff(sockfd);
			}while(s_data.header != LOSE)*/
			return;
		}
	}else{
		printf("Login fail!\n");
		return;
	}
}