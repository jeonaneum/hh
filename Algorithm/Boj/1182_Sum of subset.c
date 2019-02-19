#include <stdio.h>

int N, S, num[20];
int visited[20];
int ans = 0;

void dfs(int cnt, int sum)
{
	if (cnt == N) {
		if(sum == S)
			ans++;
	}
	else {
		
	dfs(cnt + 1, sum + num[cnt]);
	dfs(cnt + 1, sum);

	}
}


int main()
{
	scanf("%d %d", &N, &S);

	for (int i = 0; i < N; i++) {
		scanf("%d", &num[i]);
	}

	dfs(0, 0);
	
	if (!S) ans -= 1;

	printf("%d\n", ans);
}