#include <stdio.h>

#define min(x, y)	(x < y)? x : y
#define max(x, y)	(x > y)? x : y


int main()
{
	int w;
	int building[1000];

	for (int t = 0; t < 10; t++) {
		scanf("%d", &w);

		for (int i = 0; i < w; i++) {
			scanf("%d", &building[i]);
		}

		int sum = 0;
		int high = 0;

		for (int i = 2; i < w - 2; i++) {
			int left = max(building[i - 2], building[i - 1]);
			int right = max(building[i + 1], building[i + 2]);
			high = max(left, right);

			sum += max(building[i] - high, 0);
		}

		printf("#%d %d\n", t + 1, sum);
	}
}