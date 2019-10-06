#include <stdio.h>
//#include <string.h>
//#include <errno.h>
//#include <limits.h>
//#include <assert.h>



//Structs

//Functions
void WriteArg(char** argv);


int main(int argc, char** argv)
{
	printf("Hallo World\n");
	WriteArg(argv);
	return 0;


}

void WriteArg(char** argv)
{
	if(*argv == NULL)
		return;
	printf("%s\n", *(argv));
	WriteArg((++argv));
}
