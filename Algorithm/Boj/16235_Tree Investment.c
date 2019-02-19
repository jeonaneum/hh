#include <stdio.h>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

vector<int> tree[10][10];
int N, M, K;
int ans;
int now[10][10];
int summer[10][10];
int winter[10][10];

void Spring(int year)
{
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (tree[i][j].size()) {
				sort(tree[i][j].begin(), tree[i][j].end(), greater<int>());
				int death_cnt = 0;
				for (int k = 0; k < tree[i][j].size(); k++) {
					int age = year - tree[i][j][k];
					if (now[i][j] >= age) {
						now[i][j] -= age;
					}
					else {
						death_cnt++;
						summer[i][j] += age / 2;
					}
				}

				for (int k = 0; k < death_cnt; k++) {
					tree[i][j].pop_back();
				}
			}
		}
	}
}


void SummerWinter()
{
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			now[i][j] += (summer[i][j] + winter[i][j]);
			summer[i][j] = 0;
		}
	}
}


void Autumn(int year)
{
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (tree[i][j].size()) {
				for (int k = 0; k < tree[i][j].size(); k++) {
					int age = year - tree[i][j][k] + 1;
					if (age % 5) continue;

					for (int x = -1; x <= 1; x++) {
						for (int y = -1; y <= 1; y++) {
							int nrx = i + x;
							int nry = j + y;
							if ( nrx < 0 || nrx >= N || nry < 0 || nry >= N
								|| (!x && !y))	
								continue;
							tree[nrx][nry].push_back(year);
						}
					}
				}
			}
		}
	}
}


int main()
{
	scanf_s("%d %d %d", &N, &M, &K);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			scanf_s("%d", &winter[i][j]);
			now[i][j] = 5;
		}
	}

	for (int i = 0; i < M; i++) {
		int x, y, z;
		scanf_s("%d %d %d", &x, &y, &z);
		tree[x - 1][y - 1].push_back(-z);
	}

	for (int i = 0; i < K; i++) {
		Spring(i);
		SummerWinter();
		Autumn(i);
	}

	ans = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			ans += tree[i][j].size();
		}
	}
	printf("%d\n", ans);
}