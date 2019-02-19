#include<stdio.h>
#include<string.h>
int main()
{
    int R, T;
    char str[21];
    int len;
    scanf("%d", &T);
    for(int tc = 1; tc<=T; tc++){
        scanf("%d %s", &R, str);
        len = strlen(str);
        for(int i = 0; i<len;i++){
            for(int j = 0; j<R;j++){
                printf("%c", str[i]);
            }
        }
        printf("\n");
    }
}