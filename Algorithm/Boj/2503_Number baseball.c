#include <stdio.h>
#include <algorithm>

using namespace std;

typedef struct {
	int n;
	int s;
	int b;
}bb;

int N;
bb sb[100];
bool no_candi[1000];
int ans = 0;

bool chk(int val, int list, int strike, int ball) 
{
	bool ret = true;
	int sCnt = 0, bCnt = 0;
	int n[3] = { val / 100, (val % 100) / 10, (val % 100) % 10 };
	int m[3] = { list / 100, (list % 100) / 10, (list % 100) % 10 };
	
	if (!n[0] || !n[1] || !n[2] || n[0] == n[1] || n[1] == n[2] || n[2] == n[0])
		return ret;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (i == j) {
				if (n[i] == m[j])	sCnt++;
			}
			else {
				if (n[i] == m[j])	bCnt++;
			}
		}
	}

	if (sCnt == strike && bCnt == ball)
		ret = false;
	
	return ret;
}

int main()
{
	scanf("%d", &N);

	for (int i = 0; i < N; i++) {
		scanf("%d %d %d", &sb[i].n, &sb[i].s, &sb[i].b);

		for (int j = 123; j <= 987; j++) {
			if(!no_candi[j])	no_candi[j] = chk(j, sb[i].n, sb[i].s, sb[i].b);
		}
	}

	for (int i = 123; i <= 987; i++) {
		if (!no_candi[i])	ans++;
	}

	printf("%d\n", ans);
}
