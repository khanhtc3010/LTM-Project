int checkAnswer(int question, int level, int answer){
	FILE *fp;
	char *token;
	char data[256];

	if((fp=fopen("txt_answer.txt","r"))==NULL){
		printf("OPEN ANSWER.TXT FAIL\n");
		fclose(fp);
		// return 0;
	}

	while(!feof(fp)){
		fgets(data,256,fp);
		// read questin number
		token= strtok(data,"|");

		if(atoi(data)==question){
			// easy level
			if(level == 1){
				// read easy answer
				token= strtok(data,"|");
			
				if(atoi(token)==answer){
					return 1;
				}else{
					return 0;
				}
			}
			// hard level
			if(level == 2){
			
				token= strtok(data,"|");
				token= strtok(data,"|");

				if(atoi(token)==answer){
					return 1;
				}else{
					return 0;
				}
			}
		}
	}
	printf("SOMETHING WRONG\n");
	return 0;
}
