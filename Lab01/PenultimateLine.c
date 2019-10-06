#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <errno.h>
//#include <limits.h>
//#include <assert.h>

//DEFINES

#define		LINE_LENGTH		1024

//Structs

//Functions
void Init();
void AddLine(char*);
void Print();


char* Lines[] = {NULL, NULL};
int main(int argc, const char* argv[])
{
	printf("Hallo World\n");
	Init();
	char* Buffer = calloc(LINE_LENGTH, sizeof(char));
	while(1)
	{
		size_t len = getline(&Buffer, LINE_LENGTH, stdin);
		printf("%s", Buffer);
		if(len == 0)
		{
			Print();
			break;
		}
		else
		{
			AddLine(Buffer);
		}
		return 0;
	}

}

void AddLine(char * text)
{
	if(Lines[1] != NULL)
	{
		free(Lines[1]);
	}
	Lines[1] = Lines[0];
	Lines[0] = (char*)malloc(sizeof(char) * strlen(text));
	strncpy(text, Lines[0], strlen(text));
}

void Init()
{
	Lines[0] = NULL;
	Lines[1] = NULL;
	printf("Init succesfull");
}

void Print()
{
	printf("%s\n", Lines[1]);
}
