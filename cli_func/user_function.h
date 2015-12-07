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

char* answer(){
	int answer;

	do{
		printf("ANSWER (1-3): ");
		scanf("%d",&answer);
	}while(answer!=1&&answer!=2&&answer!=3);
	return intToChar(answer);
}
/*
void displayQuestion(char* question){
	printf("%s\n",question );
}
*/
