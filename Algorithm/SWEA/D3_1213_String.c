#include <stdio.h>
#include <string.h>
int main()
{
    for (int test_case = 1; test_case <= 10; test_case++)
    {
        int T;
        char search[11];
        char str[1001];
        int search_size = 0, str_size = 0, cnt = 0, ans = 0;
        scanf("%d", &T);
        scanf("%s", search, sizeof(search));
        scanf("%s", str, sizeof(str));
        search_size = strlen(search);
        str_size = strlen(str);
 
        for (int i = 0; i < str_size; i++)
        {
            if (str[i] == search[cnt])
            {
                cnt++;
                if (cnt == search_size)
                {
                    cnt = 0;
                    ans++;
                }
            }
            else
            {
                cnt = 0;
                if (str[i] == search[cnt])
                    cnt++;
            }
        }
        printf("#%d %d\n", test_case, ans);
    }
}
