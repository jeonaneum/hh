#include <stdio.h>

int N;
bool map[15][15];
int ans;


bool chk(int r, int c) 
{
	int nr = r - 1;
	int nc = c;
	
	while (nr >= 0) {
		int diff = r - nr;
		if (map[nr][nc])							return false;	///		'|' direction chk

		if (nc + diff < N && map[nr][nc + diff])	return false;	///		'/' direction chk
		
		if (nc - diff >= 0 && map[nr][nc - diff])	return false;	///		'\' direction chk
		nr -= 1;
	}
	return true;
}


void dfs(int q)
{
	if (q == N) {
		ans++;
	}
	else {
		for (int i = 0; i < N; i++) {	/// Once queen locates [q][i1] or (i1, q), 
			map[q][i] = true;
			if (chk(q, i))				/// After judge to be ok
				dfs(q + 1);				/// Find next queen seat[q + 1][i2] or (i2, q + 1).
			map[q][i] = false;
		}
	}
}

int main()
{
	scanf_s("%d", &N);

	dfs(0);

	printf("%d\n", ans);
}