#include <stdio.h>

int N;
int plat[30][30] = { 0, };
int dr[4] = { 0, 0, 1, -1 };
int dc[4] = { 1, -1, 0, 0 };
double ans, prob[4];

void dfs(int r, int c, int depth, double p)
{
	if (depth == N) {
		ans += p;
	}
	else {
		for (int i = 0; i < 4; i++) {
			int nr = r + dr[i];
			int nc = c + dc[i];

			if (!plat[nr][nc]) {
				plat[nr][nc] = 1;
				dfs(nr, nc, depth + 1, p * prob[i]);
				plat[nr][nc] = 0;
			}
		}
	}
}


int main()
{
	scanf("%d %lf %lf %lf %lf", &N, &prob[0], &prob[1], &prob[2], &prob[3]);

	for (int i = 0; i < 4; i++) {
		prob[i] /= 100.0;
	}

	ans = 0.0;
	plat[15][15] = 1;
	dfs(15, 15, 0, 1.0);

	printf("%.10lf\n", ans);
	return 0;
}