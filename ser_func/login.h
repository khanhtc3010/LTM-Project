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
