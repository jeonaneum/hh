#include <stdio.h>
#include <queue>
using namespace std;

int N, K, L;
int d = 1;
bool end_flag = false;
int time = 0;
int bd[100][100];
int dr[4] = { -1, 0, 1, 0 };
int dc[4] = { 0, 1, 0, -1 };

queue<pair<int, int>> snake;

void Go(int mov, char dir)
{
	int nr = snake.back().first;
	int nc = snake.back().second;

	while (time < mov) {
		int r = snake.front().first;
		int c = snake.front().second;

		nr += dr[d];
		nc += dc[d];

		if (nr < 0 || nr >= N || nc < 0 || nc >= N) {
			end_flag = true;
			return;
		}

		if (bd[nr][nc] == 1) {
			end_flag = true;
			return;
		}
		else if (bd[nr][nc] == 2) {
			bd[nr][nc] = 1;
			snake.push(make_pair(nr, nc));
		}
		else {
			bd[nr][nc] = 1;
			bd[r][c] = 0;
			snake.pop();
			snake.push(make_pair(nr, nc));
		}
		
		time++;	
	}
	d = (d + ((dir == 'L') ? 3 : 1)) % 4;
}


int main() 
{
	scanf("%d", &N);
	scanf("%d", &K);
	for (int i = 0; i < K; ++i) {
		int r_apple, c_apple;
		scanf("%d %d", &r_apple, &c_apple);
		bd[r_apple - 1][c_apple - 1] = 2;
	}

	snake.push(make_pair(0, 0));
	bd[0][0] = 1;

	scanf("%d", &L);
	for (int i = 0; i < L; ++i) {
		int m;
		char d;
		scanf("%d %c", &m, &d);

		Go(m, d);
		if (end_flag)	break;
	}
	
	if (!end_flag) {
		Go(100, 0);
	}
	printf("%d\n", time+1);
}