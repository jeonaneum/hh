#include<stdio.h>
int main()
{
    int num;
    int f = 0;
    int x;
    scanf("%d", &num);
    if (num == 1)
		x = -1;
	else
		x = (num - 2) / 3;
    
    while(1)
    {
        if(f*(f+1) <= x)
            f++;
        else{
            break;
        }
            
    }
    printf("%d", f+1);
}