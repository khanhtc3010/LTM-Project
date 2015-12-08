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