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

void resetPlayerList(User* player_list){
    int i = 1;
    while(i < MAX_PLAYER){
        if(player_list[i].status!=0){
            player_list[i].status = 0;
        }
        i += 1;
    }
}

int checkNormalPlayer(User* player_list){
    int i = 1;
    int count = 0;

    while(i<MAX_PLAYER){
        if(player_list[i].status == 1){
            count += 1;
        }
        i += 1;
    }
    return count;
}

char* wonScore(User* player_list){
    int score;
    score = MAX_PLAYER - checkNormalPlayer(player_list) - 1;
    return intToChar(score);
}


int checkCurrentPlayer(User user, User* player_list){
    int i = 1;
    int result;

    while(i<MAX_PLAYER){
        if(player_list[i].status != 0){
            if(strcmp(user.username, player_list[i].username)==0){
                return 1;
            }
        }
        i += 1;
    }
    return 0;
}