#include <stdio.h>
void print(int* ar);

int main()
{
	int arr[10] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

	print(arr);

	for (int i = 0; i < 10; i++){
		int min, index, temp;
		min = 9999;
		for (int j = i; j < 10; j++){
			if (arr[j] < min){
				min = arr[j];
				index = j;
			}
		}
		temp = arr[i];
		arr[i] = arr[index];
		arr[index] = temp;
	}

	print(arr);
	
    return 0;
}

void print(int* ar)
{
	for (int i = 0; i < 10; i++)
		printf("%d ", ar[i]);
	printf("\n");
}