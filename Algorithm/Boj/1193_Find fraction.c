#include<stdio.h>
int main()
{
    int x, n, index;
    int sum = 0;
    int i;
    scanf("%d", &x);
    for(i = 1; sum < x; i++)
        sum+=i;
    n = --i;
    sum -=i;
    index = x - sum;
    
    if(n%2)
        printf("%d/%d\n", n-index+1, index);
    else
        printf("%d/%d\n", index, n-index+1);
}