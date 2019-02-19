#include<stdio.h>
#include<string.h>
int main()
{
    char a[101];
    int alph[26];
    int len;
    for(int i = 0; i < 26; i++)
        alph[i] = -1;
    scanf("%s", a);
    len = strlen(a);
    for(int i = 0; i < len; i++){
        if(alph[a[i] - 'a'] == -1){
           alph[a[i]-'a'] = i; 
        }
    }
    for(int i = 0; i < 26; i++)
        printf("%d ", alph[i]);
}