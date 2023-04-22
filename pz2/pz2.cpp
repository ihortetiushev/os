#include <iostream>
//#include "stdafx.h"
#include <intrin.h> 
#include <windows.h>

using namespace std;

const int COLUMNS = 4;
const int ROWS = 5;

//2. Обробивши матрицю 4х5, створити масив номерів мінімальних елементів кожного рядка.
static void populateMinArrayPlusPlusFunc(short int** matr, int toPopulate[ROWS])
{
	for (int i = 0; i < ROWS; i++)
	{
		//assume firt element is min
		int minIndex = 0;
		int min = matr[i][0];
		for (int j = 0; j < COLUMNS; j++)
		{
			if (matr[i][j] < min)
			{
				min = matr[i][j];
				minIndex = j;
			}
		}
		toPopulate[i] = minIndex;
	}
}

static void printArray(int toPrint[ROWS])
{
	for (int i = 0; i < ROWS; i++)
	{
		cout << toPrint[i] << "  ";
	}
	cout << endl;
}

static void reset(int arr[ROWS])
{
	for (int i = 0; i < ROWS; i++)
	{
		arr[i] = -1;
	}
}

int main()
{
	unsigned __int64 start;
	unsigned __int64 end;
	short int** matr = new short int* [ROWS] {
		new short int[COLUMNS] { 1, 2, 3, 4 },
			new short int[COLUMNS] { 50, 6, 7, 8 },
			new short int[COLUMNS] { 90, 100, 11, 12 },
			new short int[COLUMNS] { 130, 140, 150, 16 },
			new short int[COLUMNS] { 170, 180, 190, 20 }
	};

	cout << "Matrix 4x5" << endl;

	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLUMNS; j++)
		{
			cout << matr[i][j] << "  ";
		}
		cout << endl;
	}

	//call function
	int outArr[ROWS];
	reset(outArr);
	start = __rdtsc();
	populateMinArrayPlusPlusFunc(matr, outArr);
	end = __rdtsc();
	cout << endl;
	cout << "indexes c++ function" << endl;
	cout << "relative execution time:" << end - start << endl;
	printArray(outArr);

	//plain code
	reset(outArr);
	start = __rdtsc();
	for (int i = 0; i < ROWS; i++)
	{
		//assume firt element is min
		int minIndex = 0;
		int min = matr[i][0];
		for (int j = 0; j < COLUMNS; j++)
		{
			if (matr[i][j] < min)
			{
				min = matr[i][j];
				minIndex = j;
			}
		}
		outArr[i] = minIndex;
	}
	end = __rdtsc();
	cout << endl;
	cout << "min indexes plain c++ code" << endl;
	cout << "relative execution time:" << end - start << endl;
	printArray(outArr);
	cout << endl;
	reset(outArr);

	int minIndex = 0;
	short int minValue = 0;
	int rowIndex;
	//asm
	start = __rdtsc();
	_asm
	{
		xor ebx, ebx//row index
		mov esi, matr
		loop_rows :
		xor edx, edx
		mov ecx, COLUMNS //columns index
		mov edi, [esi]
		mov ax, [edi + 0]//ax will hold min value. for the beginning assume first element is min
		mov minValue, ax
		mov minIndex, 0
		mov rowIndex, 0
		xor edx, edx
		loop_columns :
		mov edi, [esi]
		mov ax, [edi + edx]//getting current array value into ax
		cmp ax, minValue
		jg not_min
		mov minValue, ax //if current is less then minValue, we have new minValue and it's index
		//usind edx to save variable
		//save edx into stack
		push edx
		mov edx, rowIndex
		mov minIndex, edx
		//restore edx value
		pop edx
		not_min :
		add edx, 2//going to next column 
		inc rowIndex
		loop loop_columns
		//here in minIndex we have index of miv value for row
		//ebx holds row count == index in outArr
		//saving edi,ebx
		push edi
		push eax
		mov eax, minIndex
		lea edi, [outArr]
		//saving mix indexx to outArray
		mov  [edi + ebx * TYPE int], eax
		pop eax
		pop edi

		add esi, 4//going to next row
		inc ebx;//inc row
		cmp ebx, ROWS
		jl loop_rows

	}
	end = __rdtsc();
	cout << endl;
	cout << "min indexes asm" << endl;
	cout << "relative execution time:" << end - start << endl;
	printArray(outArr);

	return 0;
}