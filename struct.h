#ifndef __HEADER_H__
#define __HEADER_H__

#define MAX_STRING_LEN 100
#define MAX_DATA_LEN 200
#define MAX_PLAYER 3

typedef struct User
{
	char username[MAX_STRING_LEN];
	char password[MAX_STRING_LEN];
	int fd;
	int status;
}User;

typedef struct Answer{
	// TODO
}Answer;

typedef enum{
	LOG_IN, LOG_OUT, SIGN_UP,
	RQ_ANSWER, EXIT,
	ERROR, START, LOSE
}Header;

typedef struct SocketData{
	Header header;
	char data[MAX_DATA_LEN];
}SocketData;

#endif
