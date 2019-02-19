//#include <stdio.h>
#include <queue>

#define D	1
#define S	2
#define L	3
#define R	4
using namespace std;

queue<int> q;

int T, input1, input2;
int d[4];
int visited[2][10000];
int ans[10000];

void ConvInt2Arr(int input, int* arr)
{
	int idx = 4;

	while (input) {
		--idx;
		arr[idx] = input % 10;
		input = input / 10;
	}
}


int ConvArr2Int(int* arr)
{
	return (((arr[0] * 10 + arr[1]) * 10 + arr[2]) * 10 + arr[3]);
}


void DSLR(int input, int dslr)
{
	int ret = input;
	int tmp[4] = { 0, };
	int dummy = 0;

	switch (dslr) {
	case D:
		ret = ret * 2;
		if (ret > 9999)	ret %= 10000;
		break;
	case S:
		if (ret == 0) return;
		ret = ret - 1;
		if (!ret) ret = 9999;
		break;
	case L:
		ConvInt2Arr(ret, tmp);
		for (int i = 0; i < 3; i++) {
			dummy = tmp[i];
			tmp[i] = tmp[i + 1];
			tmp[i + 1] = dummy;
		}
		ret = ConvArr2Int(tmp);
		break;
	case R:
		ConvInt2Arr(ret, tmp);

		for (int i = 2; i >= 0; i--) {
			dummy = tmp[i];
			tmp[i] = tmp[i + 1];
			tmp[i + 1] = dummy;
		}
		ret = ConvArr2Int(tmp);
		break;
	}

	if (visited[0][ret]) return;

	visited[0][ret] = input;
	visited[1][ret] = dslr;
	q.push(ret);
}


int main()
{
	scanf_s("%d", &T);
	for (int tc = 0; tc < T; tc++) {
		scanf_s("%d %d", &input1, &input2);

		while (!q.empty()) q.pop();

		q.push(input1);
		visited[0][input1] = 10000;

		while (!q.empty()) {
			int num = q.front();

			q.pop();

			if (num == input2) break;

			for (int i = 1; i <= 4; i++) {
				DSLR(num, i);
			}
		}

		int goal = input2;
		int index = 0;
		while (visited[0][goal] < 10000) {
			ans[index] = visited[1][goal];
			index++;
			goal = visited[0][goal];
		}

		for (int i = index; i >= 0; i--) {
			switch (ans[i]) {
			case D:
				printf("D");
				break;
			case S:
				printf("S");
				break;
			case L:
				printf("L");
				break;
			case R:
				printf("R");
				break;
			}
		}
		printf("\n");
	}
}

