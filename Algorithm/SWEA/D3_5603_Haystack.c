#include <stdio.h>
int T, N, dummy[10001];
int main()
{
	scanf("%d", &T);
	for (int tc = 1; tc <= T; tc++)
	{
		int ave = 0, sum = 0;
		int ans = 0;
		scanf("%d", &N);
		for (int i = 0; i < N; i++)
		{
			dummy[i] = 0;
			scanf("%d", &dummy[i]);
			sum += dummy[i];
		}
		ave = sum / N;

		for (int i = 0; i < N; i++)		/// ave보다 작은 더미 크기만 축적
		{
			int diff = ave - dummy[i];

			if (diff < 1) continue;
			ans += diff;
		}
		printf("#%d %d\n", tc, ans);
	}
}
