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

/*
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
