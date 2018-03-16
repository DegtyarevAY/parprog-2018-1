#include <iostream>
#include <iomanip>
#include <time.h>
#include <fstream>
#include <omp.h>

using namespace std;

void LineMatrixMultiply(double* A, double* B, double* C, int n);

int main(int argc, char *argv[])
{
	setlocale(0, "RUS");
	FILE *stream;	// ���� ������� ������
	char* name = new char[20];	//��� �����
	errno_t err;
	if (argc > 1) {
		name = argv[1];
		cout << name << endl;
		err = freopen_s(&stream, name, "rb", stdin);
		if (err != 0)
		{
			cout << "���� �� ������!" << endl;
			return 0;
		}
	}
	else {
		err = freopen_s(&stream, "matr.in", "rb", stdin);
		if (err != 0)
		{
			cout << "���� �� ������!" << endl;
			return 0;
		}
	}
	freopen_s(&stream, "matr.out", "wb", stdout);
	srand(time(0));
	int N;
	double *A, *B, *C;
	// ��������� ������ �������
	fread(&N, sizeof(N), 1, stdin);
	A = new double[N*N];
	B = new double[N*N];
	C = new double[N*N];

	// ��������� 1-� �������
	fread(A, sizeof(*A), N*N, stdin);
	// ��������� 2-� �������
	fread(B, sizeof(*B), N*N, stdin);

	// ���������� ���������������� ���������
	double time = omp_get_wtime();
	LineMatrixMultiply(A, B, C, N);
	time = omp_get_wtime() - time;

	// ������ ������� ����������
	fwrite(&time, sizeof(time), 1, stdout);
	// ������ ������� �������
	fwrite(&N, sizeof(N), 1, stdout);
	// ������ �������������� �������
	fwrite(C, sizeof(*C), N*N, stdout);
	return 0;
}