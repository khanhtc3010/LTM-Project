int makeConnectSocket(struct sockaddr_in serverAddr, int portNo, char* serverIp){
	int sockfd;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)	error("Error opening socket!");

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(portNo);
	if(inet_pton(AF_INET, serverIp, &serverAddr.sin_addr) <= 0){
		printf("Error setup server!\n");
		exit(0);
	}

	if(connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0){
		printf("Error connect server!\n");
		exit(0);
	}
	return sockfd;
}