char* selectLevel(){
	int lvl;

	printf("\nSelect question's level:\n");
	printf("1.easy\n");
	printf("2.hard\n");

	do{
		printf("SELECT (1-2): ");
		scanf("%d",&lvl);
	}while(lvl!=1&&lvl!=2);
	return intToChar(lvl);
}

/*char* getQuestion(int question, int level){
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
}*/
