int makeListenSocket(struct sockaddr_in serverAddr, int portNo, int backlog){
    int listenSock;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(portNo);

    listenSock = socket(AF_INET, SOCK_STREAM, 0);
    if(listenSock < 0)  error("Error opening socket!");

    if(bind(listenSock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0){
        printf("\nBind loi");
        error("Error binding socket!");
        exit(0);
    }

    if(listen(listenSock, backlog) == -1){
        error("Error listening socket!");
        exit(0);
    }
    return listenSock;
}

void setupPollingServer(int listenSock, struct pollfd* client, int clientArrLen){
    int i;

    client[0].fd = listenSock;
    client[0].events = POLLRDNORM;
    for(i = 1; i < clientArrLen; i++) client[i].fd = -1;
}