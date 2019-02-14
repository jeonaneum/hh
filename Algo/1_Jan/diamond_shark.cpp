#include <iostream>

#define SOURCE_SIZE 1001
#define TARGET_SIZE 11

using namespace std;

void input(char *source, char *target)
{
	cin >> target >> source;
}

int getStrLen(char *str)
{
	int ret=0;
	for(;str[ret];ret++){}
	return ret;
}

bool isSameString(char *str1, char *str2, int len)
{
	while(len--){
		if(*str1 != *str2)
			return false;
		str1++, str2++;
	}
	return true;
}

int solve(char *source, char *target)
{
	int ret = 0;
	int len = getStrLen(target);
	int loop = getStrLen(source) - len + 1;
	for(int i=0; i<loop; i++){
		if(isSameString(source+i, target, len))
			ret++;
	}
	return ret;
}


int main(void)
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);	cout.tie(NULL);
	//freopen("input.txt", "r", stdin);
	for(int tc=1; tc<=10; tc++){
		int temp;
		cin >> temp;
		char source[SOURCE_SIZE] = {0, };
		char target[TARGET_SIZE] = {0, };
		input(source, target);
		cout << "#" << tc << " " << solve(source, target) << '\n';
	}

	return 0;
}
