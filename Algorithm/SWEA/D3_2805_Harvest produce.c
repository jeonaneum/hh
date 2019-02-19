#include <stdio.h>
int T, N;
int farm[50][50];
char str[50];
 
int abs(int a)
{
    if (a > 0)   return a;
    else        return -a;
}
int main()
{
    scanf("%d", &T);
    for (int test_case = 1; test_case <= T; test_case++)
    {
        int sum = 0;
        int medium = 0;
        int start = 0, end = 0;
 
        scanf("%d", &N);
        medium = N / 2;
        for (int i = 0; i < N; i++)
        {
            scanf("%s", str, sizeof(str));
            //while (getchar() != '\n');
            for (int j = 0; j < N; j++)
            {
                farm[i][j] = 0;
                farm[i][j] = (int)str[j] - '0';
            }
        }
        for (int i = 0; i < N; i++)
        {
            start = abs(medium - i);
            end = N - 1 - abs(medium - i);
            for (int j = start; j <= end; j++)
            {
                sum += farm[i][j];
            }
        }
        printf("#%d %d\n", test_case, sum);
    }
    return 0;
}
