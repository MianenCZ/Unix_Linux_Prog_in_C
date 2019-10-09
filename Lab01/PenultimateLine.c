#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <errno.h>
//#include <limits.h>
//#include <assert.h>

//DEFINES

#define		LINE_LENGTH			1024

#define 	ANSI_COLOR_RED     "\x1b[31m"
#define 	ANSI_COLOR_GREEN   "\x1b[32m"
#define 	ANSI_COLOR_YELLOW  "\x1b[33m"
#define 	ANSI_COLOR_BLUE    "\x1b[34m"
#define 	ANSI_COLOR_MAGENTA "\x1b[35m"
#define 	ANSI_COLOR_CYAN    "\x1b[36m"
#define 	ANSI_COLOR_RESET   "\x1b[0m"

//Structs

//Functions
void Init();
void AddLine(char*);
void Print();


char* Lines[] = {NULL, NULL};
int main()
{
	size_t buffersize = 1024;
	char * buffer = (char*)calloc(buffersize, sizeof(char));

	while(1)
	{
		size_t 	len = getline(&buffer, &buffersize, stdin);
//		printf("Readed: %zu:\t%s\n", len, buffer);
		if(len != (size_t)(-1))
		{
			AddLine(buffer);
		}
		else
		{	
			Print();
			break;	
		}	
		
//		for(size_t i = 0; i < buffersize; i++)
//		{
//			printf(".");
//			*(buffer + i) = '\0';
//		}
//		printf("\n");
//		printf("%s\n", buffer);
//		free(buffer);
	}
}

void AddLine(char * text)
{	
//	printf("To shift in: %s\n", text);
	if(Lines[1] != NULL)
	{
		free(Lines[1]);
	}
	Lines[1] = Lines[0];
	Lines[0] = (char*)malloc(sizeof(char) * (1+strlen(text)));
	Lines[0][strlen(text)] = '\0';
//	Lines[0][strlen(text) - 1] = '\0';
	strncpy(Lines[0], text, strlen(text));

//	printf("Shifting:\n");
//	printf("Lines[0]: %s\n", Lines[0]);
//	printf("Lines[1]: %s\n", Lines[1]);


}

void Init()
{	
//	printf("Init start\n");
	Lines[0] = NULL;
	Lines[1] = NULL;
//	printf("Init succesfull\n");
	return;
}

void Print()
{
	printf(ANSI_COLOR_RED "Result: %s\n" ANSI_COLOR_RESET, Lines[1]);
}

