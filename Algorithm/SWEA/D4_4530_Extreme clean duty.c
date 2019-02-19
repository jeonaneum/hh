#include <stdio.h>
int T;
long long A, B;

long long abs(long long n);
long long cnt(long long n);

int main()
{
	scanf("%d", &T);
	for (int tc = 1; tc <= T; tc++)
	{
		long long ans = 0;
		scanf("%lld %lld", &A, &B);

		if (A < 0 && B > 0)	ans = cnt(abs(A)) + cnt(abs(B)) - 1;
		else				ans = abs(cnt(abs(A)) - cnt(abs(B)));

		printf("#%d %lld\n", tc, ans);
	}
}


long long abs(long long n)
{
	return (n > 0) ? n : -n;
}


long long cnt(long long n)
{
	long long tmp = n % 10;

	if (tmp > 4)	tmp--;
	if (n < 10)		return tmp;
	else			return 9 * cnt(n / 10) + tmp;
}