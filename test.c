#include <stdio.h>
#include <string.h>
#include "struct.h"

void formatBuff(char* string){
    int i = 0;
    while(string[i] != '\0'){
        string[i] = '\0';
        i++;
    }
}

int main()
{
	User user, user_res;
	SocketData s_data, s_data_res;
	char buff[1000];

	strcpy(user.username, "KhanhTran");
	strcpy(user.password, "dauxanh");
	memcpy(buff,&user, sizeof(User));

	/*user_res = *((struct User *)(buff));

	printf("Username :%s\n", user_res.username);
	printf("Password: %s\n", user_res.password);*/

	s_data.header = LOG_IN;
	memcpy(s_data.data, &buff, MAX_DATA_LEN);
	formatBuff(buff);
	memcpy(buff, &s_data, sizeof(SocketData));

	s_data_res = *((struct SocketData *)buff);
	user_res = *((struct User *)(s_data_res.data));

	printf("header = %d\n", s_data_res.header);
	printf("username = %s\n", user_res.username);
	printf("password = %s\n", user_res.password);
}