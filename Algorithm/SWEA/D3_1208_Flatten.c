#include <stdio.h>

int main()
{
	int h_min = 100;
	int h_max = 0;
	int height[101];

	for (int tc = 1; tc <= 10; tc++){
		int dump, tmp;
		for (int i = 1; i <= 100; i++)
			height[i] = 0;

		scanf("%d", &dump);
		for (int i = 0; i < 100; i++){
			scanf("%d", &tmp);
			height[tmp]++;
			if (tmp > h_max) h_max = tmp;
			if (tmp < h_min) h_min = tmp;
		}

		while (dump) {
			height[h_max]--;
			height[h_max - 1]++;
			height[h_min]--;
			height[h_min + 1]++;
			if (height[h_min] == 0) h_min++;
			if (height[h_max] == 0) h_max--;
            dump--;
		}

		printf("#%d %d\n", tc, h_max - h_min);
	}
}