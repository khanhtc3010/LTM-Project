int login(int sockfd){
	SocketData s_data;
	User user;

	printf("\nENTER USERNAME:\t");
	scanf(" %[^\n]", user.username);
	printf("ENTER PASSWORD:\t");
	scanf(" %[^\n]", user.password);
	memcpy(buff,&user, sizeof(User));

	writeBuff(sockfd, LOG_IN, buff);
	s_data = readBuff(sockfd);
	if(s_data.header == LOG_IN){
		return 1;
	}
	return 0;
}

void signUp(int sockfd){
	SocketData s_data;
	User user;

	printf("\nCreate a new account:\n");
	printf("\nENTER USERNAME:\t");
	scanf(" %[^\n]", user.username);
	printf("ENTER PASSWORD:\t");
	scanf(" %[^\n]", user.password);
	memcpy(buff,&user, sizeof(User));

	writeBuff(sockfd, SIGN_UP, buff);
	s_data = readBuff(sockfd);
	printf("%s\n", s_data.data);
}