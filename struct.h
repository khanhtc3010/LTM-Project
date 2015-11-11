#ifndef __HEADER_H__
#define __HEADER_H__

#define MAX_STRING_LEN 100
#define MAX_DATA_LEN 200

// typedef struct {
// 	char string[MAX_STRING_LEN];
// }String;

typedef struct User
{
	char username[MAX_STRING_LEN];
	char password[MAX_STRING_LEN];
}User;

typedef struct Answer{
	// TODO
}Answer;

typedef enum{
<<<<<<< HEAD
	LOG_IN, LOG_OUT, SIGN_UP,
	RQ_ANSWER, RQ_ESCAPE,
	ERROR
=======
	LOG_IN, LOG_OUT,
	RQ_ANSWER, RQ_ESCAPE
>>>>>>> 0b7190b7b9b1a4a07ba6ed8eeed1511af08794fa
}Header;

typedef struct SocketData{
	Header header;
	char data[MAX_DATA_LEN];
}SocketData;

#endif