#include <stdio.h>
#include <stdlib.h>

#include "libmin.h"
//#include <string.h>
//#include <errno.h>
//#include <limits.h>
//#include <assert.h>



//Structs

//Functions



int main(int argc, const char* argv[])
{
	int* array = malloc(sizeof(int) * argc);

	for(int i = 0; i < argc; i++)
	{
		char* endp;
		array[i] = (int)strtol(argv[i], &endp, 10);
	}

	printf("%d\n", min(array, (size_t)argc));

	return 0;
}

