#include <iostream>
#include <ctime>

#include "mkl_spblas.h"




struct crsMatrix
{
	int Size; // ������ �������
	int Size_Z; // ���-�� ���������

	double* Value; // ������ ��������
	int* Col; // ������ ������� ��������
	int* Row_Index;	// ������ �������� �����
};

void InitMatr(int N, int NZ, crsMatrix &matr)
{
	matr.Size = N;
	matr.Size_Z = NZ;
	matr.Value = new double[NZ];
	matr.Col = new int[NZ];
	matr.Row_Index = new int[N + 1];
}

void FreeMatr(crsMatrix &matr)
{
	delete[] matr.Col;
	delete[] matr.Value;
	delete[] matr.Row_Index;
}

void SparseMKLMult(crsMatrix A, crsMatrix B, crsMatrix& C, double& time)
{
	int Size = A.Size;

	// �������� ��������� ��� ������ ������� MKL
	// ��������������� ������� A � B � �������

	for (int i = 0; i < A.Size_Z; i++)
		A.Col[i]++;
	for (int i = 0; i < B.Size_Z; i++)
		B.Col[i]++;
	for (int j = 0; j <= Size; j++)
	{
		A.Row_Index[j]++;
		B.Row_Index[j]++;
	}
	std::cout << "4" << std::endl;
	// ������������ �������, ����������� C = op(A) * B ������� � ���, op(A) = A � �� ����� ��������������� A
	char trans = 'N'; 

	// ���� �� �� �����, ������� ������ ����������
	// ��� �������� ����������, ����������:
	// 1) �������� ������ ��� ������� �������� ����� ic:
	// "���-�� �����+1" ���������;
	// 2) ������� ������� � ���������� request = 1 �
	// � ������� ic ����� �������� ��������� �������
	// 3) �������� ������ ��� �������� c � jc 
	// (���-�� ��������� = ic[���-�� �����]-1)
	// 4) ������� ������� � ���������� request = 2

	int request;
	// ��� ���� ������������� ������: ���� �����������
	// ���������, ����� �� ������������� ������� A, B � C.
	// � ��� ��������������, ��� ��� ������� �����������,
	// �������������, �������� ������� "No-No-Yes", �������
	// ������������� ������ ��������, ����� ����� �����
	// �� 1 �� 7 ������������
	int sort = 8;

	// ���������� ��������� ���������.
	// ������������ ������ ���� request = 0
	int nzmax = -1;

	// ��������� ����������
	int info;
	clock_t start = clock();
	// ������� ������ ��� ������� � ������� C
	C.Row_Index = new int[Size + 1];
	// ��������� ���������� ��������� ��������� � ������� C	request = 1;
	C.Value = 0;
	C.Col = 0;

	std::cout << "5" << std::endl;

	mkl_dcsrmultcsr(&trans,&request,&sort,&Size, &Size, &Size,
		A.Value,A.Col,A.Row_Index, B.Value, B.Col, B.Row_Index, 
		C.Value, C.Col, C.Row_Index,&nzmax,&info);

	std::cout << "6" << std::endl;
	int nzc = C.Row_Index[Size] - 1;
	C.Value = new double[nzc];
	C.Col = new int[nzc];

	// ��������� C = A * B
	request = 2;
	mkl_dcsrmultcsr(&trans, &request, &sort, &Size, &Size, &Size,
		A.Value, A.Col, A.Row_Index, B.Value, B.Col, B.Row_Index,
		C.Value, C.Col, C.Row_Index,&nzmax, &info);
	C.Size = Size;
	C.Size_Z = nzc;

	clock_t finish = clock();

}




void SequenceSparseMult(char* file_in, char* file_out, double &time)
{
	FILE* matr_in, *matr_res;
	
	int N = 0, Nz = 0;
	crsMatrix A, B,C;


	freopen_s(&matr_in, file_in, "rb", stdin);
	
	fread(&N, sizeof(N), 1, stdin);
	fread(&Nz, sizeof(Nz), 1, stdin);

	int size_nonzero = N * Nz;

	std::cout << "2" << std::endl;
	InitMatr(N, size_nonzero, A);
	InitMatr(N, size_nonzero, B);

	fread(A.Value, sizeof(*A.Value), size_nonzero, stdin);
	fread(A.Col, sizeof(*A.Col), size_nonzero, stdin);
	fread(A.Row_Index, sizeof(*A.Row_Index), N + 1, stdin);

	fread(B.Value, sizeof(*B.Value), size_nonzero, stdin);
	fread(B.Col, sizeof(*B.Col), size_nonzero, stdin);
	fread(B.Row_Index, sizeof(*B.Row_Index), N + 1, stdin);

	fclose(matr_in);

	std::cout << "3" << std::endl;
	SparseMKLMult(A,B,C,time);

	
	size_nonzero = C.Size_Z;

	freopen_s(&matr_res, file_out, "wb", stdout);
	fwrite(&N, sizeof(N), 1, stdout);
	fwrite(&size_nonzero, sizeof(size_nonzero), 1, stdout);
	fwrite(C.Value, sizeof(*C.Value), size_nonzero, stdout);
	fwrite(C.Col, sizeof(*C.Col), size_nonzero, stdout);
	fwrite(C.Row_Index, sizeof(*C.Row_Index), N + 1, stdout);

	fclose(matr_res);

	FreeMatr(A);
	FreeMatr(B);
	FreeMatr(C);
	
}

int main(int argc, char* argv[])
{
	double time;
	if (argc != 3)
	{
		std::cout << "Invalid input parameters\n" << std::endl;
		return 0;
	}
	std::cout << "1" << std::endl;
	SequenceSparseMult(argv[1], argv[2], time);
	return 0;
}