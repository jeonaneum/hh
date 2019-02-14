#include <iostream>

#define SIZE 21

using namespace std;

int arr[SIZE];
bool picked[SIZE];

void input(int &n, int &s)
{
	cin >> n >> s;
	for(int i=0; i<n; i++){
		cin >> arr[i];
	}
}

int subset(int depth, int start, int n, int r, int sum, int s)
{
	if(depth == r){
		// base case
		if(sum == s){
			return 1;
		}
		else
			return 0;
	}
	int ret = 0;
	int limit = n-r+depth;
	for(int i=start; i<=limit; i++){
		if(!picked[i]){
			picked[i] = true;
			ret += subset(depth+1, i+1, n, r, sum+arr[i], s);
			picked[i] = false;
		}
	}
	return ret;
}

int solve(int n, int s)
{
	int ret = 0;
	for(int i=1; i<=n; i++){
		ret += subset(0, 0, n, i, 0, s);
	}
	return ret;
}

int main(void)
{
	ios::sync_with_stdio(false);
	//freopen("input.txt", "r", stdin);
	int n, s;
	input(n, s);
	cout << solve(n, s) << endl;
	return 0;
}

import java.util.Scanner;
public class baekjoon_1192  {
	static int N, S, result;
	static int[] arr, visited;
	public static void main(String[] args) {
		//	부분집합의 합 구하기 
		Scanner input = new Scanner(System.in);
		N = input.nextInt();
		S = input.nextInt();
		
		arr = new int[N];
		visited = new int[N];
		
		for(int i=0; i< N; i++){
			arr[i] = input.nextInt();
		}
		
		//	
		result=0;
		solve(0);
		
		result = (S == 0) ? result-- : result;
		System.out.println(result);
	}
	
	public static void solve(int index){
		if(index>=N){
			int sum =0;
			for(int i=0; i<N; i++){
				if(visited[i] ==1)
					sum += arr[i];
			}
			
			if(sum == S)
				result++;
			
			return;
		}
		
		// 자기 자신을 포함한 부분집합과 그렇지 않은 부분 집합으로  재귀를 돌린다. 
		
		solve(index+1);
		visited[index] =1;
		
		solve(index+1);
		visited[index] =0;
	}
	
}

#include <stdio.h>

int N, S, num[20];
int visited[20];
int ans = 0;

void dfs(int cnt, int sum)
{
	if (cnt == N) {
		if(sum == S)
			ans++;
	}
	else {
		
	dfs(cnt + 1, sum + num[cnt]);
	dfs(cnt + 1, sum);

	}
}


int main()
{
	scanf_s("%d %d", &N, &S);

	for (int i = 0; i < N; i++) {
		scanf_s("%d", &num[i]);
	}

	dfs(0, 0);
	
	if (!S) ans -= 1;

	printf("%d\n", ans);
}