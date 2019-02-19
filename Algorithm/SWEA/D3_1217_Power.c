#include <stdio.h>

int squre(int n, int m)
{
	if(!m)	return 1;
    return n * squre(n, m-1);
}

int main()
{
    int T = 0;
    int N, M;
    int answer = 0;
    
    for(int i = 0; i < 10; i++)
    {
    	scanf("%d", &T);
        scanf("%d %d", &N, &M);
        answer = squre(N, M);
        
        printf("#%d\n", T);
        printf("%d\n", answer);
    }
}