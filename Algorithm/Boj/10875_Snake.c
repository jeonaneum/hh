#include <stdio.h>

#include <queue>
#include <vector>

using namespace std;

vector<pair<int, int>> snake;
vector<pair<int, char>> inst;
int L, N;
int ans;
int dr[4] = { 1, 0, -1, 0 };
int dc[4] = { 0, 1, 0, -1 };
int dir;

int CalDir(char rot)
{
	if (rot == 'R') {
		return (dir + 1) % 4;
	}
	else if (rot == 'L') {
		return (dir +3) % 4;
	}
}


int Go(int move, char rot)	/// true: Game End
{
	while (move) {
		int hr = snake.back().first + dr[dir];
		int hc = snake.back().second + dc[dir];
		
		if (hr > abs(L) || hc > abs(L)) {
			return true;
		}
		else {
			for (int i = 0; i < snake.size(); i++) {
				if (snake[i].first == hr && snake[i].second == hc) {
					return true;
				}
			}
		}

		snake.push_back(make_pair(hr, hc));
		move--;
		ans++;
	}

	dir = CalDir(rot);
	return false;
}


int main()
{
	inst.clear();
	snake.clear();
	dir = 1;
	ans = 0;

	scanf_s("%d", &L);
	scanf_s("%d", &N);
	for (int i = 0; i < N; i++) {
		int dist;
		char rot;
		scanf_s("%d %c", &dist, &rot);
		inst.push_back(make_pair(dist, rot));
	}
	
	snake.push_back(make_pair(0, 0));

	for (int i = 0; i < inst.size(); i++) {
		if (Go(inst[i].first, inst[i].second))	break;
	}

	printf("%d\n", ans + 1);
}