#include <stdio.h>
#include <queue>
#define MAP_MAX 1001
using namespace std;

typedef struct {
	int r;
	int c;
	int cnt;
	int hammer;
} st;
queue<st> q;

int N, M;
int map[MAP_MAX][MAP_MAX];
int visited[MAP_MAX][MAP_MAX];
int dr[4] = { -1, 0, 1, 0 };
int dc[4] = { 0, 1, 0, -1 };
int hammer_num = 1;
int ans;

int main()
{
	scanf_s("%d %d", &N, &M);

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			scanf_s("%1d", &map[i][j]);
		}
	}

	while (!q.empty())	q.pop();
	
	st start = { 0, 0, 1, hammer_num };
	st end = { N - 1, M - 1, 0, 0 };
	ans = -1;
	visited[start.r][start.c] = 1;
	q.push(start);

	while (!q.empty()) {
		int r = q.front().r;
		int c = q.front().c;
		int cnt = q.front().cnt;
		int hammer = q.front().hammer;

		if (r == N-1 && c == M-1) {
			ans = cnt;
			break;
		}

		q.pop();

		for (int d = 0; d < 4; d++) {
			st next = { r + dr[d], c + dc[d], cnt + 1, hammer };

			if (next.r < 0 || next.r >= N || next.c < 0 || next.c >= M) continue;

			if (visited[next.r][next.c] == 1) continue;

			if (visited[next.r][next.c] == 2 && !next.hammer) continue;

			if (map[next.r][next.c] == 0) {
				q.push(next);
				if (next.hammer)
					visited[next.r][next.c] = 1;
				else
					visited[next.r][next.c] = 2;
			}
			else
			{
				if (hammer) {
					next.hammer--;
					q.push(next);
					visited[next.r][next.c] = 2;
				}
			}
		}
	}
	printf("%d\n", ans);
}




/*
/// 메모리 초과 visited 없어서.

#include <stdio.h>
#include <queue>
#define MAP_MAX 1002
using namespace std;

typedef struct {
	int r;
	int c;
	int cnt;
	int hammer;
} st;
queue<st> q;

int N, M;
char map[MAP_MAX][MAP_MAX];
int dr[4] = { -1, 0, 1, 0 };
int dc[4] = { 0, 1, 0, -1 };
int hammer_num = 1;
int ans;

int main()
{
	scanf("%d %d", &N, &M);

	for (int i = 0; i < N; i++) {
		scanf("%s", map[i], sizeof(map));
	}

	while (!q.empty())	q.pop();
	
	st start = { 0, 0, 1, hammer_num };
	st end = { N - 1, M - 1, 0, 0 };
	ans = -1;

	q.push(start);

	while (!q.empty()) {
		int r = q.front().r;
		int c = q.front().c;
		int cnt = q.front().cnt;
		int hammer = q.front().hammer;

		if (r == end.r && c == end.c) {
			ans = cnt;
			break;
		}

		q.pop();

		for (int d = 0; d < 4; d++) {
			st next = { r + dr[d], c + dc[d], cnt + 1, hammer };

			if (next.r < 0 || next.r >= N || next.c < 0 || next.c >= M) continue;

			if (map[next.r][next.c] == '0') {
				q.push(next);
			}
			else
			{
				if (hammer) {
					next.hammer--;
					q.push(next);
				}
			}
		}
	}
	printf("%d\n", ans);
}
*/