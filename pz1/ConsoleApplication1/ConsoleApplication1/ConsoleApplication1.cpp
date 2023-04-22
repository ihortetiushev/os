#include <iostream>
#include <bitset>

using namespace std;

const int ARR_SIZE = sizeof(int) * 8;

void bitOperations(int num)
{
	cout <<"Task1:" << endl;
	cout << "Number is: " << num << endl;
	cout << "Binary presentation: " << bitset<32>(num) << endl;

	int outArr[ARR_SIZE];

	for (int i = 0; i < ARR_SIZE; i++) {
		outArr[i] = -1;
	}

	_asm
	{
		mov ecx, ARR_SIZE;
		mov eax, num;
		xor edx, edx;
		xor ebx, ebx;
	l1:
		shr eax, 1;
		jnc loop_end;
		//if bit ==1
		mov edi, edx;
		mov outArr[edi], ebx;
		add edx, 4;
		jmp loop_end;
	loop_end:
		inc ebx;
		loop l1;
	}

	cout << "Bit num with '1' (starting from 0):" << endl;
	for (int i = 0; i < ARR_SIZE; i++) {
		if (outArr[i] == -1) {
			break;
		}
		cout << outArr[i] << endl;
	}
}

//2. Знайти суму елементів головної діагоналі матриці 6*6
void diagonalSum()
{
	cout <<"Task2:" << endl;
	int N = 6;
	int sum = 0;
	short int** matr = new short int* [N] {
		new short int[N] { 1, 2, 3, 2, 5, 6 },
		new short int[N] { 6, 2, 1, 7, 6, 6 },
		new short int[N] { 1, 1, 3, 2, 1, 4 },
		new short int[N] { 1, 3, 1, 1, 3, 4 },
		new short int[N] { 2, 2, 3, 4, 5, 2 },
		new short int[N] { 3, 2, 4, 4, 4, 5 }
	};
	cout << "Matrix 6x6" << endl;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			cout << matr[i][j] << "  ";
		}
		cout << endl;
	}

	_asm
	{
		xor ebx, ebx
		mov ecx, N
		mov esi, matr
		xor edx, edx
		star1 :
		mov edi, [esi]
			mov ax, [edi + edx]
			cwde
			add ebx, eax
			add esi, 4
			inc edx
			inc edx
			loop star1
			mov sum, ebx
	}

	cout << "Sum diagonal = " << sum << endl;

}

int main()
{
	int num = 768;
	bitOperations(num);

	cout << endl;

	diagonalSum();

	return 0;
}


