#include <windows.h>
#include <iostream>

using namespace std;
HANDLE hSumSemaphore = CreateSemaphore(NULL, 0, 1, NULL);
HANDLE hWorkSemaphore;

struct ArrStruct //такие структуры. чтобы вместе с массивом хранить и другие данные
{
	volatile int* arr;
	int n;
};

struct ArrStruct2
{
	volatile int* arr;
	int n;
	int rest;
};

DWORD WINAPI work(ArrStruct2* arrStr) {
	int temp; // временная переменная для обмена элементов местами

	//cout << "Sourted array: " << endl;

// Сортировка массива пузырьком
	for (int i = 0; i < arrStr->n - 1; i++) {
		for (int j = 0; j < arrStr->n - i - 1; j++) {
			if (arrStr->arr[j] > arrStr->arr[j + 1]) {
				// меняем элементы местами
				temp = arrStr->arr[j];
				arrStr->arr[j] = arrStr->arr[j + 1];
				arrStr->arr[j + 1] = temp;

				//cout << arrStr->arr[i] << endl;

				ReleaseSemaphore(hWorkSemaphore, 1, NULL);
				Sleep(arrStr->rest * 1000); //отдыхаем, как требуется по ТЗ
			}
			else { // if it is false and no need sort - free semaphore to publish element in work
				ReleaseSemaphore(hWorkSemaphore, 1, NULL);
			}
		}
	}


	ReleaseSemaphore(hWorkSemaphore, 1, NULL);
	return 0;
}

DWORD WINAPI sum(ArrStruct* arrStr) {
	WaitForSingleObject(hSumSemaphore, INFINITE);

	int sum = 0;
	for (int i = 0; i < arrStr->n; i++)
		sum += arrStr->arr[i];
	cout << endl << "Sum = " << sum << endl;

	return 0;
}

int main() {
	HANDLE hWork, hMult;
	DWORD IDWork, IDMult;
	ArrStruct arrStr;
	ArrStruct2 arrStr2;

	int k;
	volatile int* arr;

	int rest;
	int a;
	int n;

	cout << "Please enter k: ";
	cin >> k;

	cout << "Please enter n: ";
	cin >> n;

	arr = new int[n];


	cout << "Please enter " << n << " elements of array divided by space: ";
	for (int i = 0; i < n; i++) {
		int b;
		cin >> b;
		arr[i] = b;
	}

	arrStr.arr = arr;
	arrStr2.arr = arr;

	cout << "Please enter rest time: ";
	cin >> rest;

	cout << "Source array: " << endl;
	for (int i = 0; i < n; i++)
		cout << arr[i] << " ";
	cout << endl;

	hWorkSemaphore = CreateSemaphore(NULL, 0, n+1, NULL); //remove n+1


	arrStr.n = k;

	arrStr2.n = n;
	arrStr2.rest = rest;

	hWork = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)work, &arrStr2, 0, &IDWork);
	hMult = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)sum, &arrStr, 0, &IDMult);



	cout << "New array: " << endl; // and sort it one time
	for (int i = 0; i < n; i++) {
		WaitForSingleObject(hWorkSemaphore, INFINITE);
		cout << arrStr.arr[i] << " ";
	}
	cout << endl;

	ReleaseSemaphore(hSumSemaphore, 1, NULL);

	CloseHandle(hSumSemaphore);
	CloseHandle(hWorkSemaphore);
	CloseHandle(hMult);
	CloseHandle(hWork);
	//delete[] arr;


	system("pause");
	return 0;
}
