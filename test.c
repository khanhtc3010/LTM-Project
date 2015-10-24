#include <stdio.h>

typedef enum{
	black,
	green,
	blue
}Color;

int check(Color color){
	if(color == black)	return 1;
	return 0;
}

int main()
{
	int _check = check(black);
	if(_check == 1)	printf("Ok\n");
	return 0;
}