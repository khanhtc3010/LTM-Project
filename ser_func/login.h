int checkLogin(User user){
    FILE *fp;
    char *token;
    char data[256];
    printf("username = %s\n", user.username);
    printf("password = %s\n", user.password);

    if((fp=fopen("ser_func/data/txt_user.txt","r"))==NULL){
        printf("OPEN USER.TXT FAIL\n");
        return 0;
    }

    while(!feof(fp)){
        fgets(data,256,fp);
        if(data[0]=='U' && data[1]=='|'){

            token = strtok(data, "|");
            token = strtok(NULL, "|");
            
            if(strcmp(token,user.username)==0){      
                token = strtok(NULL, "|");
                
                if(strcmp(token,user.password)==0){
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

int checkSignUp(User user){
    FILE *fp;
    char data[256];
    int check;

    check = checkLogin(user);
    if(check==0){
        fp = fopen("ser_func/data/txt_user.txt","a");
        strcpy(data,"U|");
        strcat(data,user.username);
        strcat(data,"|");
        strcat(data,user.password);
        strcat(data,"|");
        fprintf(fp, "%s\n", data);
        fclose(fp);
        return 1;
    }else{
        return 0;
    }
}
