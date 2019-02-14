#include <iostream>

#define SIZE 20

using namespace std;

int check[SIZE][SIZE];

int dx[] = {0, 1, 1, 1, 0, -1, -1, -1};
int dy[] = {-1, -1, 0, 1, 1, 1, 0, -1};

bool isWall(int x, int y, int n)
{
	return (x<0 || y<0 || x>=n || y>=n);
}

void checking(bool delta, int _x, int _y, int n)
{
	for(int i=0; i<8; i++){
		int x = _x, y = _y;
		while(1){
			int nx = x + dx[i];
			int ny = y + dy[i];
			if(isWall(nx, ny, n))
				break;
			delta ? check[ny][nx]++ : check[ny][nx]--;
			x = nx, y = ny;
		}
	}
}

int nqueen(int depth, int n)
{
	if(depth == n){
		return 1;
	}
	int ret = 0;
	int y = depth;
	for(int x=0; x<n; x++){
		if(!check[y][x]){
			checking(true, x, y, n);
			ret += nqueen(depth+1, n);
			checking(false, x, y, n);
		}
	}
	return ret;
}

int main(void)
{
	ios::sync_with_stdio(false);
	int n;
	cin >> n;
	cout << nqueen(0, n) << '\n';
	return 0;
}