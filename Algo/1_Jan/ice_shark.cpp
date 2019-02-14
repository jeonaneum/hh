

/**
  * @author Jihoon Jang
  */

#include <iostream>
#include <queue>
#include <memory.h>

#define SIZE 301

using namespace std;

typedef struct qnode
{
	int x, y, melt;
	qnode(int x, int y, int m):x(x), y(y), melt(m){}
}qnode;

int map[SIZE][SIZE];
bool visited[SIZE][SIZE];
int dx[] = {0, 1, 0, -1};
int dy[] = {-1, 0, 1, 0};
int cnt = 0;				// 남은 빙산 개수

void input(int &n, int &m)
{
	cin >> n >> m;
	for(int y=0; y<n; y++){
		for(int x=0; x<m; x++){
			cin >> map[y][x];
			if(map[y][x])
				cnt++;
		}
	}
}

bool isWall(int x, int y, int n, int m)
{
	return (x<0 || y<0 || x>=m || y>=n);
}

//	인접 바다 저장
int getMelt(int x, int y, int n, int m)
{
	int ret = 0;
	for(int i=0; i<4; i++){
		int nx = x + dx[i];
		int ny = y + dy[i];
		if(isWall(nx, ny, n, m))	continue;
		if(!map[ny][nx])
			ret++;
	}
	return ret;
}

void dfs(int x, int y, int n, int m, queue<qnode> &q)
{
	for(int i=0; i<4; i++){
		int nx = x + dx[i];
		int ny = y + dy[i];
		if(isWall(nx, ny, n, m))	continue;
		if(!visited[ny][nx] && map[ny][nx]){
			visited[ny][nx] = true;
			q.push(qnode(nx, ny, getMelt(nx, ny, n, m)));
			dfs(nx, ny, n, m, q);
		}
	}
}

int counting(int n, int m, queue<qnode> &q)
{
	int ret = 0;
	for(int y=0; y<n; y++){
		for(int x=0; x<m; x++){
			if(!map[y][x] || visited[y][x])	continue;
			visited[y][x] = true;
			q.push(qnode(x, y, getMelt(x, y, n, m)));
			dfs(x, y, n, m, q);
			ret++;
		}
	}
	return ret;
}

void melting(queue<qnode> &q)
{
	while(!q.empty()){
		int x = q.front().x, y = q.front().y, melt = q.front().melt;	q.pop();
		map[y][x] > melt ? map[y][x] -= melt : (map[y][x] = 0, cnt--);
	}
}

int solve(int n, int m)
{
	int ret = 0;
	while(1){
		queue<qnode> q;
		memset(visited, 0, sizeof(visited));
		if(counting(n, m, q) > 1)
			break;
		melting(q);
		if(!cnt)		// 다 녹아버리면 0반환
			return 0;
		ret++;
	}
	return ret;
}

int main(void)
{
	ios::sync_with_stdio(false);
	//freopen("input.txt", "r", stdin);
	int n, m;
	input(n, m);
	cout << solve(n, m) << '\n';
	return 0;
}
