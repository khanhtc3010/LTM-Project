#ifndef __HEADER_H__
#define __HEADER_H__

#define MAX_STRING_LEN 100
#define MAX_DATA_LEN 200
#define MAX_PLAYER 4

typedef struct User
{
	char username[MAX_STRING_LEN];
	char password[MAX_STRING_LEN];
	int fd;
	int status;
}User;

typedef enum{
	LOG_IN,
	ANSWER, QUESTION, LEVEL, HELP,
	START, EXIT, LOSE, WIN,
	ERROR
}Header;

typedef struct SocketData{
	Header header;
	char data[MAX_DATA_LEN];
}SocketData;

#endif
