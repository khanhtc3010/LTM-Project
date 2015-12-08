int checkAnswer(int question, int level, int answer){
	FILE *fp;
	char *token;
	char data[256];

	if((fp=fopen("ser_func/data/txt_answer.txt","r"))==NULL){
		printf("OPEN txt_answer.txt FAIL\n");
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

char* getQuestion(int question_num, int level){
	FILE *fp;
	char *token;
	char data[256];
	char question[1024];

	strcpy(question,"Q: ");

	switch(level){
		case 1:
		if((fp=fopen("ser_func/data/txt_easy.txt","r"))==NULL){
			printf("OPEN txt_easy.txt FAIL\n");
			fclose(fp);
    	    //return NULL;
		}
		break;
		case 2:
		if((fp=fopen("ser_func/data/txt_hard.txt","r"))==NULL){
			printf("OPEN txt_hard.txt FAIL\n");
			fclose(fp);
        	//return NULL;
		}
		break;
	}

	while(!feof(fp)){
		fgets(data,256,fp);

		token = strtok(data, "|");
		if(strcmp(token,intToChar(question_num))==0){
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
	return "\0";
}

int randAnswer(){
	int i = 0;
    while(i == 0){
        srand(time(NULL));
        i = rand() % 4;
    }
    return i;
}

int getCorrectAnswer(int question, int level){
	FILE *fp;
	char *token;
	char data[256];

	if((fp=fopen("ser_func/data/txt_answer.txt","r"))==NULL){
		printf("OPEN txt_answer.txt FAIL\n");
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
				return atoi(token);
			}
			// hard level
			if(level == 2){
			
				token= strtok(data,"|");
				token= strtok(data,"|");
				return atoi(token);
			}
		}
	}
	printf("SOMETHING WRONG!\n");
	return 0;
}

int popWrongAnswer(int question, int level){
	int correctAns;

	correctAns = getCorrectAnswer(question, level);
	if(correctAns!=1)	return 1;
	if(correctAns!=2)	return 2;
	if(correctAns!=3)	return 3;
}