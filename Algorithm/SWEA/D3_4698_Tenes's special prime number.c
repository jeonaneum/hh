#include <stdio.h>
#define MAX_SIZE	1000001
int T, num[MAX_SIZE];
void cal_num();
int judge_D(int n, int d);

int main()
{
	cal_num();				/// 미리 소수 구하기
	scanf("%d", &T);
	for (int tc = 1; tc <= T; tc++)
	{
		int D, A, B;
		int ans = 0;
		scanf("%d %d %d", &D, &A, &B);

		for (int i = A; i <= B; i++)
		{
			if (num[i] != 0)		/// 소수 이면
			{
				if (judge_D(i, D))	ans++;	/// D 들어가는지 판단
			}
		}
		printf("#%d %d\n", tc, ans);
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


int judge_D(int n, int d)
{
	while (n)
	{
		if (n % 10 == d)	return 1;
		n /= 10;
	}
	return 0;
}