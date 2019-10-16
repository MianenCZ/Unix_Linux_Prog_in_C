#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
//#include <errno.h>
//#include <limits.h>
//#include <assert.h>

#define 	ROWS		30
#define		COLUMNS		211



//Structs

//Functions
void Init();
void Print();
void Generate();

void Rize(int Length);
void StayStill(int Length);
void Down(int Length);



int Mountain [ROWS][COLUMNS];
int ActualRow;
int ActualColumn;

int main()
{
	Init();
	Print();


	printf("Hallo World\n");
	return 0;
}

void Generate()
{
	
}

void Rize(int Length)
{
	
}

void StayStill(int Length)
{
	
}

void Down(int Length)
{
		
}

void Init()
{
	ActualRow = 0;
	ActualColumn = 0;
	for(int i = 0; i < ROWS; i++)
	{
		for(int j = 0; j < COLUMNS; j++)
		{
			Mountain[i][j] = (i % 10);	
		}
	}
}

void Print()
{
	for(int i = 0; i < ROWS; i++)
	{
		for(int j = 0; j < COLUMNS; j++)
		{
			printf("%d", Mountain[i][j]);	
		}
		printf("\n");
	}		
}
