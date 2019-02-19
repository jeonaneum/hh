#include <stdio.h>
#define N	10
int sorted[N]; /// 반드시 전역 변수로 선언

void merge(int a[], int m, int middle, int n)
{
	int i = m;
	int j = middle + 1;
	int k = m;
	
	/// 작은 순서대로 배열에 삽입
	while(i <= middle && j <= n){
		if(a[i] <= a[j]){
			sorted[k] = a[i];
			i++;
		}
		else{
			sorted[k] = a[j];
			j++;
		}
		k++;
	}
	
	/// 남은 데이터 삽입
	if(i > middle){
		for(int t = j; t <= n; t++){
			sorted[k] = a[t];
			k++;
		}
	}
	else{
		for(int t = i; t <= middle; t++){
			sorted[k] = a[t];
			k++;
		}
	}
	
	/// 정렬된 배열 삽입
	for(int t = m; t <= n; t++){
		a[t] = sorted[t];
	}
}

void mergeSort(int a[], int m, int n)
{
	if(m < n){
		int middle = (m + n) / 2;
		mergeSort(a, m, middle);
		mergeSort(a, middle + 1, n);
		merge(a, m, middle, n);
	}
}


int main()
{
	int arr[10] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

	print(arr);
	
	mergeSort(arr, 0, N-1);
	
	print(arr);
	
    return 0;
}


void print(int* ar)
{
	for (int i = 0; i < 10; i++)
		printf("%d ", ar[i]);
	printf("\n");
}