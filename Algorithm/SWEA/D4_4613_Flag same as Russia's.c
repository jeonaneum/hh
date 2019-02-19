#include <stdio.h>
#define	white	0
#define blue	1
#define red		2

int T, N, M;
char color[51][51];
int row[51][3];
int main()
{
	scanf("%d", &T);
	for (int tc = 1; tc <= T; tc++)
	{
		scanf("%d %d", &N, &M);
		for (int i = 0; i < N; i++)
		{
			row[i][white] = 0;
			row[i][blue] = 0;
			row[i][red] = 0;
			scanf("%s", color[i], sizeof(color));

			for (int j = 0; j < M; j++)
			{
				switch (color[i][j])		///각 행의 W, B, R 색의 수 카운트
				{
				case 'W': case 'w':
					row[i][white]++;
					break;
				case 'B': case 'b':
					row[i][blue]++;
					break;
				case 'R': case 'r':
					row[i][red]++;
					break;
				}
			}
		}

		int max = 0;
		/// B의 시작 위치와 너비를 조절해가며 색 배치에 따른 바꾸지 않아도 되는 셀 최대값 비교
		for (int i = 1; i < N - 1; i++)			/// Blue의 시작 위치
		{
			for (int bw = 1; bw + i < N; bw++)		/// Blue의 너비
			{
				int pos, w = 0, b = 0, r = 0;
				for (pos = 0; pos < i; pos++)		w += row[pos][white];	/// B의 시작 위치 전까지의 W 카운트
				for (pos = i; pos < i + bw; pos++)	b += row[pos][blue];  /// B의 시작위치부터 너비만큼의 B 카운트
				for (pos = i + bw; pos < N; pos++)	r += row[pos][red];  /// B가 끝난 위치부터 끝까지의 R 카운트

				if (w + b + r > max)
				{
					max = w + b + r;
				}
			}
		}
		printf("#%d %d\n", tc, N * M - max);	/// 변경해야 하는 갯수는 총 셀 - 바꾸지 않아도 되는 값
	}
}