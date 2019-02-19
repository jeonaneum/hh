#include <stdio.h>
#define MAX_SIZE	1000001
int num[MAX_SIZE];
void cal_num();

int main()
{
	cal_num();
	for (int i = 2; i <= MAX_SIZE; i++)
	{
		if (num[i] != 0)
			printf("%d ", num[i]);
	}
}


void cal_num()
{
	num[1] = 0;
	for (int i = 2; i <= MAX_SIZE; i++)
	{
		num[i] = i;
	}

	for (int i = 2; i <= MAX_SIZE; i++)
	{
		if (num[i] == 0) continue;
		for (int j = i + i; j <= MAX_SIZE; j += i)
			num[j] = 0;
	}
}