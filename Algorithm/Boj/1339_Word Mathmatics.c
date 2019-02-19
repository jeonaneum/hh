#include <stdio.h>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

vector<string> v;

int N;
char str[10][9];
int alph[26] = { 0, };
int ans = 0;


bool desc(int a, int b)
{
	return a > b;
}


int CalAlphNum(int idx, int size)
{
	int tmp = size - idx - 1;
	int ret = 1;
	while (tmp) {
		ret *= 10;
		tmp--;
	}

	return ret;
}


int main()
{
	v.clear();
	for (int i = 0; i < 26; i++) {
		alph[i] = 0;
	}
	scanf("%d", &N);

	for (int i = 0; i < N; i++) {
		scanf("%s", str[i], sizeof(str));
		v.push_back(str[i]);
	}

	for (int i = 0; i < v.size(); i++) {
		for (int j = 0; j < v[i].size(); j++) {
			int n = v[i][j] - 'A';
			alph[n] += CalAlphNum(j, v[i].size());
		}
	}

	sort(alph, alph + 26, desc);

	for(int i = 0; alph[i] != 0; i++){
		ans += alph[i] * (9 - i);
	}

	printf("%d\n", ans);
}