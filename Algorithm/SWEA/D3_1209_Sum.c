#include <stdio.h>
#define NUM 100
#define max(x, y)	(x > y)? x: y
int main()
{
	int col_sum[NUM];
	int row_sum[NUM];
	int num[NUM][NUM];
	int T;
	
	for (int tc = 1; tc <= 10; tc++) {
		int x_sum[2] = { 0, };
		int max_sum = 0;
		for (int i = 0; i < NUM; i++) {
			row_sum[i] = 0;
			col_sum[i] = 0;
		}
		scanf("%d", &T);
		for (int i = 0; i < NUM; i++) {
			for (int j = 0; j < NUM; j++) {
				scanf("%d", &num[i][j]);
				row_sum[i] += num[i][j];
				col_sum[j] += num[i][j];
				if (i == j)	x_sum[0] += num[i][j];
				else if (i == 100 - j)	x_sum[1] += num[i][j];
			}
		}

		for (int i = 0; i < NUM; i++) {
			max_sum = max(max_sum, row_sum[i]);
			max_sum = max(col_sum[i], max_sum);
		}
		for (int i = 0; i < 2; i++) {
			max_sum = max(max_sum, x_sum[i]);
		}
		printf("#%d %d\n", tc, max_sum);
	}
}