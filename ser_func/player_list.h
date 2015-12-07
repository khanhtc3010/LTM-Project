int checkPlayerNumber(User* player_list){
    int i;
    for(i = 1; i < MAX_PLAYER; i++){
        if((player_list[i].status != 1)&&(player_list[i].status != 2)){
            printf("player number = %d\n", i-1);
            return i-1;
        }
    }
    return MAX_PLAYER;
}

int appendPlayer(int sockfd, User user, User* player_list){
    int i = 1;
    while(i < MAX_PLAYER){
        if((player_list[i].status != 1)&&(player_list[i].status != 2)){
            player_list[i].status = 1;
            strcpy(player_list[i].username,user.username);
            strcpy(player_list[i].password,user.password);
            player_list[i].fd = sockfd;
            printf("socket = %d\n", player_list[i].fd);
            break;
        }else{
            i += 1;
        }
    }
    i = checkPlayerNumber(player_list);
    if(i == MAX_PLAYER){
        return 1; //da du so nguoi choi
    }else{
        return 0; //chua du so nguoi choi, doi nguoi choi moi dang nhap
    }
}

int setMainPlayer(User* player_list){
    int i = 0;
    while(i == 0){
        srand(time(NULL));
        i = rand() % MAX_PLAYER;
    }
    printf("main player on socket %d\n", player_list[i].fd);
    player_list[i].status = 2;
    return player_list[i].fd;
}

int checkGameStatus(User* player_list){
    int i;
    for(i = 1; i < MAX_PLAYER; i++){
        if(player_list[i].status == 2){
            return 1; //game dang dien ra chan dang nhap
        }
    }
    return 0; //game chua dien ra duoc phep dang nhap
}

int removePlayer(int sockfd, User* player_list){
    int i = 1;
    while(i < MAX_PLAYER){
        if(player_list[i].fd == sockfd){
            if(player_list[i].status == 1){
                player_list[i].status = 0;
                return 0;   //removed normal player
            }
            if(player_list[i].status == 2){
                player_list[i].status = 0;
                return 1;   //removed main player
            }
        }else{
            i += 1;
        }
    }
}