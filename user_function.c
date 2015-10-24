#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "struct.h"

int checkLogin(char* username, char* password){
	FILE *fp;
	char *token;
	char data[256];

	if((fp=fopen("txt_user.txt","r"))==NULL){
		printf("OPEN USER.TXT FAIL\n");
		fclose(fp);
		return 0;
	}

	while(!feof(fp)){
		fgets(data,256,fp);
		if(data[0]=='U' && data[1]=='|'){
			
			token = strtok(data, "|");
			token = strtok(NULL, "|");
			
			if(strcmp(token,username)==0){		
				token = strtok(NULL, "|");
				
				if(strcmp(token,password)==0){
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

int login(){
	User user;

	do{
		printf("ENTER USERNAME:\t");
		scanf(" %[^\n]", user.username);

		printf("ENTER PASSWORD:\t");
		scanf(" %[^\n]", user.password);
	}while(checkLogin(user.username,user.password)!=1);
	
	printf("LOGIN SUCCES\n");
	return 1;
}

int selectLevel(){
	int lvl;
	int tranBytes;
	int sockfd;

	printf("EASY OR HARD:\n");
	printf("1.EASY\n");
	printf("2.HARD\n");

	do{
		printf("SELECT (1-2): ");
		scanf(" %[^\n]",&lvl);

	}while(lvl!=1||lvl!=2);

	// send to server
	tranBytes = write(sockfd, lvl, 1024);

	return 1;

}

char* getQuestion(int question, int level){
	FILE *fp;
	char *token;
	char data[256];
	char question[1024];

	strcpy(question,"Q: ");

	switch(level){
		case 1:
		if((fp=fopen("txt_easy.txt","r"))==NULL){
			printf("OPEN USER.TXT FAIL\n");
			fclose(fp);
    	    // return 0;
		}
		break;
		case 2:
		if((fp=fopen("txt_hard.txt","r"))==NULL){
			printf("OPEN USER.TXT FAIL\n");
			fclose(fp);
        	// return 0;
		}
		break;
	}

	while(!feof(fp)){
		fgets(data,256,fp);

		token = strtok(data, "|");
		if(data[0]==atoi(question) && data[1]=='|'){
			token = strtok(NULL, "|");

			strcat(question,token);
			strcat(question,"\n1. ");

			token = strtok(NULL, "|");

			strcat(question,token);
			strcat(question,"\n2. ");

			token = strtok(NULL, "|");

			strcat(question,token);
			strcat(question,"\n3. ");

			token = strtok(NULL, "|");

			strcat(question,token);
			strcat(question,"\n");

			fclose(fp);
			return question;
		}
	}		
	fclose(fp);
	return question;
}

void displayQuestion(char* question){
	printf("%s\n",question );
}

void answer(){
	int answer;
	int tranBytes;
	int sockfd;

	do{
		printf("ANSWER (1-3): ");
		scanf(" %d",&answer);
	}while(answer!=1||answer!=2||answer!=3);

	tranBytes = write(sockfd, answer, 1024);
	
	return;
}

int main()
{
	login();

	return 0;
}
