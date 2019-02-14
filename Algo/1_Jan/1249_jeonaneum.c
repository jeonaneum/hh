#include <stdio.h>
#include <stdlib.h>
#include <queue>
using namespace std;
 
int T;
int dr[4] = { 0, 1, 0, -1 };
int dc[4] = { 1, 0, -1, 0 };
queue<pair<int, int>> q;
int main()
{
    scanf("%d", &T);
    for (int test_case = 1; test_case <= T; test_case++)
    {
        int N;
        int mat[101][101];
        int dp[101][101];
        int ans = 0;
        scanf("%d", &N);
         
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)                  
            {
                mat[i][j] = 0;                          /// mat initialize
                dp[i][j] = 90000;                       /// dp initialize 10000 * 9 = 90000(max value)
                scanf("%1d", &mat[i][j]);           
            }
        }
 
        while (!q.empty()) q.pop();
 
        dp[0][0] = 0;                                   /// start point value = 0
        q.push(make_pair(0, 0));
 
        while (!q.empty())
        {
            int r = q.front().first;
            int c = q.front().second;
            q.pop();
 
            for (int i = 0; i < 4; i++)                  /// up, down, left, right chk
            {
                int nr = r + dr[i];
                int nc = c + dc[i];
 
                if (nr < 0 || nc < 0 || nr >= N || nc >= N) continue;   /// exceed range
 
                if (dp[r][c] + mat[nr][nc] < dp[nr][nc]) 
                {
                    dp[nr][nc] = dp[r][c] + mat[nr][nc];
                    q.push(make_pair(nr, nc));
                }
            }
        }
        printf("#%d %d\n", test_case, dp[N-1][N-1]);
    }
}
