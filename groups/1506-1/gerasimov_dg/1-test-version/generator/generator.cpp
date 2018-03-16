#include <iostream>
#include <iomanip>
#include <cstdio> 
#include <random> 
#include <ctime> 
#include <chrono> 

using namespace std;

int n_tests[] = { 1, 2, 2, 10, 3, 4, 5, 40, 23, 8,
9, 10, 10, 10, 10, 100, 200, 300, 400, 500, 600, 700,
800, 900, 1000 };

void LineMatrixMultiply(double* A, double* B, double* C, int n)
{
	for (int i = 0; i < n;i++)
	{
		for (int j = 0; j < n;j++)
		{
			C[i*n + j] = 0;
			for (int k = 0; k < n; k++)
			{
				C[i*n + j] += A[i*n + k] * B[k*n + j];
			}
		}
	}
}


int main(int argc, char * argv[])
{
	// �������������� ����� stdout � ���� matr.in   
	//freopen("matr.in", "wb", stdout);  
	FILE *stream;
	freopen_s(&stream, "matr.in", "wb", stdout);
	FILE *answer;
	
	// ������ ��������� ��������� ����� � seed ������ ���������� ������� � ������ �����   
	default_random_engine generator(chrono::system_clock::now().time_since_epoch().count());
	// ������ ����������� ������������� ��������� �������� ���� double � ���������    //   [-10000, 10000]   
	//uniform_real_distribution <double> distribution(-1e4, 1e4);   
	uniform_real_distribution <double> distribution(-500, 500);
	// ����� ������ ������   
	int n = 10;
	// ���� �������� ����� ����� � ���������� ��������� ������, �� ���� ������ �� n_tests   
	if (argc > 1)
		n = n_tests[atoi(argv[1])];
	// ���������� � �������� ���� ����������� ������   
	fwrite(&n, sizeof(n), 1, stdout);
	// ������ ��������� ������ ��� ������ �������  
	double *cur = new double[n];
	double *A = new double[n*n];
	double *B = new double[n*n];
	double *C = new double[n*n];
	double tempDis = 0.0;
	// ���������� ������ �������   
	for (int i = 0; i < n; i++)
	{   // ��������� ���������� ������� �� ������������ ������������� ��������� ������ �������     
		for (int j = 0; j < n; j++) {
			//tempDis = distribution(generator);
			cur[j] = distribution(generator);
			A[i*n + j] = cur[j];
		}
		// ���������� ������ � �������� ���� � ����    
		fwrite(cur, sizeof(*cur), n, stream);
	}
	// ���������� ���������� ������ �������   
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++) {
			//tempDis = distribution(generator);
			cur[j] = distribution(generator);
			B[i*n + j] = cur[j];
		}
		fwrite(cur, sizeof(*cur), n, stream);
	}

	LineMatrixMultiply(A, B, C, n);
	double time = 0.0;	// ������ ����������, ��� ���������� ������ viewer'�, ���� ��������� ��������� ��������� ��������� �������.

	freopen_s(&answer, "answer.ans", "wb", stdout);
	fwrite(&time, sizeof(time), 1, answer);
	fwrite(&n, sizeof(n), 1, answer);
	fwrite(C, sizeof(*C), n*n, answer);


	fclose(answer);
	fclose(stream);
	return 0;
}