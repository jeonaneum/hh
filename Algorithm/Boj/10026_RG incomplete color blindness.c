#include <stdio.h>

int N;
char rgb[101][101];
int visited[100][100];
int section_cnt, index;
int dr[4] = { -1, 0, 1, 0 };
int dc[4] = { 0, 1, 0, -1 };
int ans1, ans2;

void dfs(int r, int c, char color)
{
	for (int d = 0; d < 4; d++) {
		int nr = r + dr[d];
		int nc = c + dc[d];

		if (nr < 0 || nr >= N || nc < 0 || nc >= N) continue;

		/// 방문하지 않고, dfs를 처음 들어올 때의 색과 같으면
		if (!visited[nr][nc] && rgb[nr][nc] == color) {
			visited[nr][nc] = section_cnt;	///visited[][]에 몇 번째 구역인지 입력
			dfs(nr, nc, color);
		}
	}
}

void LookRGB()
{
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {

			if (!index && !visited[i][j]) {
				section_cnt++;
				visited[i][j] = section_cnt;
				index = rgb[i][j];
				dfs(i, j, index);
				index = 0;
			}
		}
	}
}


int main()
{
	scanf_s("%d", &N);
	for (int i = 0; i < N; i++) {
		scanf_s("%s", rgb[i], sizeof(rgb));
	}
	index = 0;
		
	/// R, G, B case
	section_cnt = 0;
	LookRGB();
	ans1 = section_cnt;

	/// R+G, B case
	section_cnt = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (rgb[i][j] == 'G')	
				rgb[i][j] = 'R';
			visited[i][j] = 0;
		}
	}
	LookRGB();	
	ans2 = section_cnt;

	printf("%d %d\n", ans1, ans2);
}