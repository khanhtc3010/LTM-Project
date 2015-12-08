#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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

//--------------------------------------------
char buff[1024];
int tranBytes;

//-------------/*FUNCTIONS*/------------------
void formatBuff(char* string){
	int i = 0;
	while(string[i] != '\0'){
		string[i] = '\0';
		i++;
	}
}

int menu(){
	int choice;
	printf("===========1 vs 100==========\n\t1.Play Game\n\t2.About\n\t3.Exit\n=============================\nChoice: ");
	scanf("%d", &choice);
	return choice;
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

void writeAllSocket(User* player_list, Header header, char* mPlayerData, char* nPlayer){
	int i;
	for(i = 1; i < MAX_PLAYER; i++){
		if(player_list[i].status == 2){
			writeBuff(player_list[i].fd, header, mPlayerData);
		}
		if(player_list[i].status == 1){
			writeBuff(player_list[i].fd, header, nPlayer);
		}
	}
}

char* intToChar(int num){
	switch(num){
		case 1:	return "1";
		case 2:	return "2";
		case 3:	return "3";
		case 4:	return "4";
		case 5:	return "5";
		case 6:	return "6";
		case 7:	return "7";
		case 8:	return "8";
		case 9:	return "9";
		case 10: return "10";
		case 11: return "11";
		case 12: return "12";
		case 13: return "13";
		case 14: return "14";
		case 15: return "15";
		default: return "0";
	}
}

int charToInt(char* chr){
	if(strcpy(chr, "1")==0)	return 1;
	if(strcpy(chr, "2")==0)	return 2;
	if(strcpy(chr, "3")==0)	return 3;
}