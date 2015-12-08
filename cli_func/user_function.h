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

char* get_answer(int time_out)
{
    struct pollfd mypoll = { STDIN_FILENO, POLLIN|POLLPRI };
    int answer;

    int t = poll(&mypoll, 1, time_out);

    if(t==0){
    	return "0";
    }
    if(t==1){
    	while(1){
    		scanf("%d",&answer);
    		if(answer==1||answer==2||answer==3){
    			return intToChar(answer);
    		}
    	}
    }
}

void delay(int time_delay){
	time_t first = time(NULL), second = first;
	while(second - first < time_delay)
		second = time(NULL);
}

int sendHelp(int* helpArray){
	int help = 0;
	int count = 0;
	int i;
	char choice;
	for(i = 0; i < 3; i++){
		if(helpArray[i]==1)	count += 1;
	}
	if(count==0){
		return 0;
	}
	
	printf("Ban co muon dung tro giup?(y/n): ");
	fflush(stdin);
	scanf("%c",&choice);
	while(getchar()!='\n');
	if(choice=='y'||choice=='Y'){
		do{
			printf("Ban co cac quyen tro giup(): \n");
			if(helpArray[0]==1)	printf("\t1.Skip this question.\n");
			if(helpArray[1]==1) printf("\t2.Bo 1 dap an sai.\n");
			if(helpArray[2]==1)	printf("\t3.Tham khao nguoi choi khac.\n");
			printf("Your choice: ");
			scanf("%d",&help);
			while(getchar()!='\n');
		}while(helpArray[help-1]==0);
		helpArray[help-1] = 0;
		return help;
	}
	return 0;
}
