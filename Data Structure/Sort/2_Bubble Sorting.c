#include <stdio.h>
void print(int* ar);

int main()
{
	int arr[10] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

	print(arr);
	
	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 9 - i; j++)
		{
			if (arr[j] > arr[j + 1]) {
				int tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
		}
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