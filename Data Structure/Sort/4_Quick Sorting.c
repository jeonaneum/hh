#include <stdio.h>
void print(int* ar);
void quickSort(int* data, int start, int end);

int main()
{
	int arr[10] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

	print(arr);
	
	quickSort(arr, 0, 9);
	
	print(arr);
	
    return 0;
}


void print(int* ar)
{
	for (int i = 0; i < 10; i++)
		printf("%d ", ar[i]);
	printf("\n");
}


void quickSort(int* data, int start, int end)
{
	if (start >= end) {
		return;
	}

	int key = start;
	int i = start + 1;
	int j = end;

	while (i <= j) {	/// 엇갈릴 때 반복
		while (data[i] <= data[key]) {	/// 키 값보다 큰 값일 때까지 i++
			i++;
		}
		while (data[j] >= data[key] && j > start) {	///키 값보다 작은 값일 때까지 j--
			j--;
		}

		int tmp;
		if (i > j) {	///엇갈린 상태면 키값과 교체
			tmp = data[key];
			data[key] = data[j];
			data[j] = tmp;
		}
		else {
			tmp = data[i];
			data[i] = data[j];
			data[j] = tmp;
		}
	}
	
	quickSort(data, start, j - 1);
	quickSort(data, j + 1, end);
}