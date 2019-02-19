#include <stdio.h>
#define abs(x)	(x<0)?-x:x
char alph[26][2];
int t;

void swap(char* a, char* b)
{
	char tmp[2] = { a[0], a[1] };
	for (int i = 0; i < 2; i++) {
		a[i] = b[i];
		b[i] = tmp[i];
	}
}


int main()
{
	int n, m;
	scanf("%d %d", &n, &m);

	getchar();
	for(int i = n - 1; i >= 0; i--){
		scanf("%c", &alph[i][0]);
		alph[i][1] = 0;
	}
	getchar();
	for (int i = 0; i < m; i++) {
		scanf("%c", &alph[n + i][0]);
		alph[n + i][1] = 1;
	}
	getchar();
	scanf("%d", &t);

	while (t--) {
		for (int i = 0; i < n + m - 1; i++) {
			if (!alph[i][1]) {
				if (alph[i + 1][1]) {
					swap(alph[i], alph[i + 1]);
					i++;
				}
			}
		}
	}
	for (int i = 0; i < n + m; i++) {
		printf("%c", alph[i][0]);
	}
	printf("\n");
}
