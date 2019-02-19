#include <stdio.h>
#include <stdlib.h>
#include <queue>

using namespace std;

int T;
char MAT[101][101];

int answer;
int testnum;

int dr[4] = { -1, 0, 1, 0 };
int dc[4] = { 0, 1, 0, -1 };
int sr, sc, gr, gc;

queue<pair<int, int>> q;


int main()
{
	//FILE *t;

	//freopen_s(&t, "test.txt", "w", stdout);
	//scanf("%d", &T);

	for (int test_case = 1; test_case <= 10; test_case++)
	{
		scanf("%d", &testnum);
		for (int i = 0; i < 100; i++)
		{
			scanf("%s", MAT[i]);
			for (int j = 0; j < 100; j++)
			{
				if (MAT[i][j] == '2')
				{
					sr = i;
					sc = j;
				}
				else if (MAT[i][j] == '3')
				{
					gr = i;
					gc = j;
				}
			}
		}
			
		while (!q.empty())	q.pop();

		q.push(make_pair(sr, sc));
		answer = 0;

		while (!q.empty())
		{
			int r = q.front().first;
			int c = q.front().second;

			q.pop();

			if (r == gr && c == gc)
			{
				answer = 1;
				break;
			}

			for (int d = 0; d < 4; d++)
			{
				int nr = r + dr[d];
				int nc = c + dc[d];

				if (nr < 0 || nr >= 100 || nc < 0 || nc >= 100 || MAT[nr][nc] == '1') continue;

				q.push(make_pair(nr, nc));
				MAT[nr][nc] = '1';
			}
		}

		printf("#%d %d\n", test_case, answer);
	}
    
    return 0;
}