#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include "struct.h"
/**
	SERVER SIDE
/**/
int checkSignUp(User user){
	FILE *fp;
	char *token;
	char data[256];
    // printf("name = %s\n", user.username);
    // printf("pass = %s\n", user.password);

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
				return 0;
			}
		}
		fclose(fp);
		return 1;
	}
}
/**
	CLIENT SIDE
/**/

int signUp(User user, int socket, char* buff){
	int tranBytes;
	SocketData s_data;
	formatBuff(buff);
	User user_res;

	memcpy(buff,&user, sizeof(User));

	s_data.header = SIGN_UP;
	memcpy(s_data.data, &buff, 200);
	
	tranBytes = write(sockfd, s_data, 1024);
	s_data = *((struct SocketData *)buff);
    printf("%d\n", s_data.header);
    user = *((struct User *)(s_data.data

    if(tranBytes < 0)	error("Error writing socket!");
	tranBytes = read(sockfd, buff, 1024);
	if(strcmp(buff, "SIGN UP SUCCESS") == 0){
		return 1;
	}
	return 0;    
}