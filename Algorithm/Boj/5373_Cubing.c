#include <stdio.h>

int t, n;
char cube[6][9];

void InitCube()
{
	char color[6] = { 'w', 'y', 'r', 'o', 'g', 'b' };
	//u: 0-w, d: 1-y, f: 2-r, b: 3-o, l: 4-g, r: 5-b
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 9; j++) {
			cube[i][j] = color[i];
		}
	}
}

void Rotation_Suface(char *arr, char rot)
{
	char tmp = arr[0];
	if (rot == '+') {
		arr[0] = arr[6];
		arr[6] = arr[8];
		arr[8] = arr[2];
		arr[2] = tmp;
		tmp = arr[1];
		arr[1] = arr[3];
		arr[3] = arr[7];
		arr[7] = arr[5];
		arr[5] = tmp;
	}
	else {
		arr[0] = arr[2];
		arr[2] = arr[8];
		arr[8] = arr[6];
		arr[6] = tmp;
		tmp = arr[1];
		arr[1] = arr[5];
		arr[5] = arr[7];
		arr[7] = arr[3];
		arr[3] = tmp;
	}
}


void Rotation_U(char rot)
{
	char tmp[3];
	if (rot == '+') {
		for (int i = 0; i < 3; i++) {
			tmp[i] = cube[2][i];
			cube[2][i] = cube[5][i];
			cube[5][i] = cube[3][i];
			cube[3][i] = cube[4][i];
			cube[4][i] = tmp[i];
		}
	}
	else {
		for (int i = 0; i < 3; i++) {
			tmp[i] = cube[2][i];
			cube[2][i] = cube[4][i];
			cube[4][i] = cube[3][i];
			cube[3][i] = cube[5][i];
			cube[5][i] = tmp[i];
		}
	}
}


void Rotation_D(char rot)
{
	char tmp[3];
	if (rot == '-') {
		for (int i = 0; i < 3; i++) {
			tmp[i] = cube[2][6 + i];
			cube[2][6 + i] = cube[5][6 + i];
			cube[5][6 + i] = cube[3][6 + i];
			cube[3][6 + i] = cube[4][6 + i];
			cube[4][6 + i] = tmp[i];
		}
	}
	else {
		for (int i = 0; i < 3; i++) {
			tmp[i] = cube[2][6 + i];
			cube[2][6 + i] = cube[4][6 + i];
			cube[4][6 + i] = cube[3][6 + i];
			cube[3][6 + i] = cube[5][6 + i];
			cube[5][6 + i] = tmp[i];
		}
	}
}


void Rotation_F(char rot)
{
	char tmp[3];
	if (rot == '-') {
		for (int i = 0; i < 3; i++) {
			tmp[i] = cube[0][6 + i];
			cube[0][6 + i] = cube[5][3 * i];
			cube[5][3 * i] = cube[1][2 - i];
			cube[1][2 - i] = cube[4][8 - (3 * i)];
			cube[4][8 - (3 * i)] = tmp[i];
		}
	}
	else {
		for (int i = 0; i < 3; i++) {
			tmp[i] = cube[0][6 + i]; 
			cube[0][6 + i] = cube[4][8 - (3 * i)];
			cube[4][8 - (3 * i)] = cube[1][2 - i];
			cube[1][2 - i] = cube[5][3 * i];
			cube[5][3 * i] = tmp[i];
		}
	}
}


void Rotation_B(char rot)
{
	char tmp[3];
	if (rot == '+') {
		for (int i = 0; i < 3; i++) {
			tmp[i] = cube[0][2 - i];
			cube[0][2 - i] = cube[5][8 - 3 * i];
			cube[5][8 - 3 * i] = cube[1][6 + i];
			cube[1][6 + i] = cube[4][3 * i];
			cube[4][3 * i] = tmp[i];
		}
	}
	else {
		for (int i = 0; i < 3; i++) {
			tmp[i] = cube[0][2 - i];
			cube[0][2 - i] = cube[4][3 * i];
			cube[4][3 * i] = cube[1][6 + i];
			cube[1][6 + i] = cube[5][8 - 3 * i];
			cube[5][8 - 3 * i] = tmp[i];
		}
	}
}


void Rotation_L(char rot)
{
	char tmp[3];
	if (rot == '+') {
		for (int i = 0; i < 3; i++) {
			tmp[i] = cube[0][3 * i];
			cube[0][3 * i] = cube[3][8 - 3 * i];
			cube[3][8 - 3 * i] = cube[1][3 * i];
			cube[1][3 * i] = cube[2][3 * i];
			cube[2][3 * i] = tmp[i];
		}
	}
	else {
		for (int i = 0; i < 3; i++) {
			tmp[i] = cube[0][3 * i];
			cube[0][3 * i] = cube[2][3 * i];
			cube[2][3 * i] = cube[1][3 * i];
			cube[1][3 * i] = cube[3][8 - 3 * i];
			cube[3][8 - 3 * i] = tmp[i];
		}
	}
}


void Rotation_R(char rot)
{
	char tmp[3];
	if (rot == '-') {
		for (int i = 0; i < 3; i++) {
			tmp[i] = cube[0][8 - 3 * i];
			cube[0][8 - 3 * i] = cube[3][3 * i];
			cube[3][3 * i] = cube[1][8 - 3 * i];
			cube[1][8 - 3 * i] = cube[2][8 - 3 * i];
			cube[2][8 - 3 * i] = tmp[i];
		}
	}
	else {
		for (int i = 0; i < 3; i++) {
			tmp[i] = cube[0][8 - 3 * i];
			cube[0][8 - 3 * i] = cube[2][8 - 3 * i];
			cube[2][8 - 3 * i] = cube[1][8 - 3 * i];
			cube[1][8 - 3 * i] = cube[3][3 * i];
			cube[3][3 * i] = tmp[i];
		}
	}
}


int main()
{
	scanf("%d", &t);

	for (int tc = 1; tc <= t; tc++) {
		scanf("%d", &n);

		InitCube();
		for (int i = 0; i < n; i++) {
			char inst[3];
			scanf("%s", &inst, sizeof(inst));

			int suf = 0;
			switch (inst[0]) {
			case 'U':
				Rotation_U(inst[1]);
				break;
			case 'D':
				suf += 1;
				Rotation_D(inst[1]);
				break;
			case 'F':
				suf += 2;
				Rotation_F(inst[1]);
				break;
			case 'B':
				suf += 3;
				Rotation_B(inst[1]);
				break;
			case 'L':
				suf += 4;
				Rotation_L(inst[1]);
				break;
			case 'R':
				suf += 5;
				Rotation_R(inst[1]);
				break;
			}
			Rotation_Suface(cube[suf], inst[1]);
		}
		for (int i = 0; i < 9; i++) {
			printf("%c", cube[0][i]);
			if (i % 3 == 2) printf("\n");
		}
	}
}