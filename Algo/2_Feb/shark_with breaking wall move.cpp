
#include <iostream>
#include <queue>

#define SIZE 1001

using namespace std;

int map[SIZE][SIZE];
bool visited[2][SIZE][SIZE];
int dx[] = {0, 1, 0, -1};
int dy[] = {-1, 0, 1, 0};

typedef struct qnode
{
	int x, y, distance, crash;
	qnode(int x, int y, int d, int c):x(x), y(y), distance(d), crash(c){}
}qnode;

void input(int &n, int &m)
{
	cin >> n >> m;
	for(int y=0; y<n; y++){
		for(int x=0; x<m; x++){
			char ch;
			cin >> ch;
			map[y][x] = ch - '0';
		}
	}
}

bool isWall(int x, int y, int n, int m)
{
	return (x<0 || y<0 || x>=m || y>=n);
}

bool isArrive(int x, int y, int n, int m)
{
	return (x == m-1 && y == n-1);
}

int bfs(int n, int m)
{
	queue<qnode> q;
	int ret = -1;
	q.push(qnode(0, 0, 1, 0));
	visited[0][0][0] = true;
	while(!q.empty()){
		int x = q.front().x, y = q.front().y, distance = q.front().distance, crash = q.front().crash;	q.pop();
		if(isArrive(x, y, n, m)){
			ret = distance;
			break;
		}
		for(int i=0; i<4; i++){
			int nx = x + dx[i];
			int ny = y + dy[i];
			if(isWall(nx, ny, n, m))
				continue;
			if(!visited[crash][ny][nx]){
				if(map[ny][nx]){			// 벽인 경우
					if(!crash){				// 아직 벽을 깨지 않음
						visited[1][ny][nx] = true;
						q.push(qnode(nx, ny, distance+1, 1));
					}
				}
				else{
					visited[crash][ny][nx] = true;
					q.push(qnode(nx, ny, distance+1, crash));
				}
			}
		}
	}
	return ret;
}

int main(void)
{
	ios::sync_with_stdio(false);
	int n, m;
	input(n, m);
	cout << bfs(n, m) << '\n';
	return 0;
}
