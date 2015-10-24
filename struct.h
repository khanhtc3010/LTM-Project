#ifndef __HEADER_H__
#define __HEADER_H__

#define MAX_STRING_LEN 100
#define MAX_DATA_LEN 256

// typedef struct {
// 	char string[MAX_STRING_LEN];
// }String;

typedef struct 
{
	char username[MAX_STRING_LEN];
	char password[MAX_STRING_LEN];
}User;

typedef struct {
	// TODO
}Answer;

typedef enum{
	LOG_IN, LOG_OUT,
	RQ_ANSWER, RQ_ESCAPE
}Header;

typedef struct {
	Header header;
	char* addData;
}SocketData;

#endif