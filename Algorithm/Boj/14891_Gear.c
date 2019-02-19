#include <stdio.h>
#include <math.h>

int K, ans;
int rot_k[100][2];
int visited[4];
char gear[4][9];

#define N			'0'
#define S			'1'
#define CLOCK		1
#define ANTICLOCK	-1


void PrintGear()
{
	printf(" %c%c%c   %c%c%c   %c%c%c   %c%c%c\n", gear[0][7], gear[0][0], gear[0][1],
		gear[1][7], gear[1][0], gear[1][1], gear[2][7], gear[2][0], gear[2][1],
		gear[3][7], gear[3][0], gear[3][1]);
	printf("%c[0]%c %c[1]%c %c[2]%c %c[3]%c\n", gear[0][6], gear[0][2],
		gear[1][6], gear[1][2], gear[2][6], gear[2][2],
		gear[3][6], gear[3][2]);
	printf(" %c%c%c   %c%c%c   %c%c%c   %c%c%c\n\n", gear[0][5], gear[0][4], gear[0][3],
		gear[1][5], gear[1][4], gear[1][3], gear[2][5], gear[2][4], gear[2][3],
		gear[3][5], gear[3][4], gear[3][3]);
}


void Rotate(char* g, int dir)
{
	if (dir == CLOCK) {
		for (int i = 6; i >= 0; i--) {
			char tmp = g[i];
			g[i] = g[i + 1];
			g[i + 1] = tmp;
		}
	}
	else if(dir == ANTICLOCK){
		for (int i = 0; i < 7; i++) {
			char tmp = g[i];
			g[i] = g[i + 1];
			g[i + 1] = tmp;
		}
	}
	//PrintGear();	// for debug
}


void Gear(int idx, int dir) 
{
	if (idx < 0 || idx > 3) return;
	if (visited[idx]) return;

	visited[idx] = 1;

	int near1 = idx - 1;
	int near2 = idx + 1;

	if (near1 >= 0 && near1 <= 3 && !visited[near1]) {
		if (gear[near1][2] != gear[idx][6]) {
			Gear(near1, dir*(-1));
			Rotate(gear[near1], dir*(-1));
		}
	}

	if (near2 >= 0 && near2 <= 3 && !visited[near2]) {
		if (gear[idx][2] != gear[near2][6]) {
			Gear(near2, dir*(-1));
			Rotate(gear[near2], dir*(-1));
		}
	}
}


int CalResult()
{
	int ret = 0;

	for (int i = 0; i < 4; i++) {
		if (gear[i][0] == S) {
			ret += pow(2, i);
		}
	}
	return ret;
}


int main()
{
	for (int i = 0; i < 4; i++) {
		scanf_s("%s", gear[i], sizeof(gear));
	}

	scanf_s("%d", &K);
	for (int i = 0; i < K; i++) {
		scanf_s("%d %d", &rot_k[i][0], &rot_k[i][1]);
	}

	//PrintGear();	// inital gears status

	for (int i = 0; i < K; i++) {
		Gear(rot_k[i][0]-1, rot_k[i][1]);
		Rotate(gear[rot_k[i][0] - 1], rot_k[i][1]);
		for (int j = 0; j < 4; j++) {
			visited[j] = 0;
		}
	}

	ans = CalResult();
	printf("%d\n", ans);
}