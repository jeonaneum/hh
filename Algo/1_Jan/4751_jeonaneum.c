#include <stdio.h>
#include <string.h>
 
int N;
char str[51];
 
int main()
{
    int str_len = 0;
    scanf("%d", &N);
 
    for (int test_case = 1; test_case <= N; test_case++)
    {
        int col = 0;
        int row = 0;
        scanf("%s", str, sizeof(str));
        str_len = strlen(str);  
        row = (str_len) * 4 + 1;                /// calculate str_size after decorate 
         
        while (col < 5 && str_len)           /// print str for each row
        {
            switch (col)
            {
            case 0: case 4:                     /// first, last sentence // 2, 6, 10, 14, ... -> '#', else -> '.'
                for (int i = 0; i < row; i++)
                {
                    if (i % 4 == 2)
                        printf("#");
                    else
                        printf(".");
                }
                printf("\n");
                break;
            case 1: case 3:                 /// 2nd, 4th sentence // 1, 3, 5, 7, ... -> '#', else -> '.'
                for (int i = 0; i < row; i++)
                {
                    if (i % 2 == 1)
                        printf("#");
                    else
                        printf(".");
                }
                printf("\n");
                break;
            case 2:                     /// 3rd sentence // 2, 6, 10, 14, ... -> char, 0, 4, 8, ... -> '#', else -> '.'
                for (int i = 0; i < row; i++)
                {
                    if (i % 4 == 2)
                        printf("%c", str[i/4]);
                    else if(i % 4 == 0)
                        printf("#");
                    else
                        printf(".");
                }
                printf("\n");
                break;
            }
            col++;
        }
    }
    return 0;
}
