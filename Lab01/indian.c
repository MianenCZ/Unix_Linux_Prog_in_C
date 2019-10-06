#include <stdio.h>
//#include <string.h>
//#include <errno.h>
//#include <limits.h>
//#include <assert.h>



//Structs

//Functions
int main(int argc, const char* argv[])
{
	printf("Hallo World\n");
	int a = 150;
	
	int* p_a = &a;
	char* p_ca = (char*)p_a;

	for(size_t i = 0; i < sizeof(int)/sizeof(char); i++)
	{
		printf("add %p:\t%d\n", (p_ca + i), (int)*(p_ca + i));	
	}
	return 0;
}

