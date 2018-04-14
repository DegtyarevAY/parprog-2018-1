#include <iostream> 
#include <fstream>
#include <omp.h> 
#include <ctime>

using namespace std;

void ParallelResultCalculation(double* A, double* B, double *C, int Size, int NumThreads) {
	int Shift = int(sqrt((int)NumThreads));
	int blockSize = Size / Shift;
	omp_set_num_threads(NumThreads);
	int i, j, k;
#pragma omp parallel default(none) private(i,j,k) shared(A, B, C, Size, Shift ,blockSize)
	{ 
		int threadNum = omp_get_thread_num();
		int row = threadNum / Shift;
		int col = threadNum % Shift;
		for (int shift = 0; shift<Shift; shift++)
		{ 
			for (i = row*blockSize; i < (row + 1)*blockSize; i++) {
				for (j = col*blockSize; j < (col + 1)*blockSize; j++) {
					for (k = shift*blockSize; k < (shift + 1)*blockSize; k++)
					{
						C[i*Size + j] += A[i*Size + k] * B[k*Size + j];
					}
				}
			}
		}
	}
}


int main(int argc, char *argv[]) 
{
	setlocale(0, "RUS");
	FILE *stream;	// ���� ������� ������
	FILE *answer;	// ���� �������� ������
	char* name;		// ��� �����
	errno_t err;
	int NumThreads;	// ����� �������
	if (argc > 2) {
		name = argv[1];
		cout << name << endl;
		err = freopen_s(&stream, name, "rb", stdin);
		if (err != 0)
		{
			cout << "���� �� ������!" << endl;
			return 0;
		}
		NumThreads = atoi(argv[2]);
	}
	else {
		err = freopen_s(&stream, "matr.in", "rb", stdin);
		if (err != 0)
		{
			cout << "���� �� ������!" << endl;
			return 0;
		}
		NumThreads = 1;
	}
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

	int Shift = int(sqrt((int)NumThreads));
	if (Shift * Shift != NumThreads)
	{
		cout << "����� ������� ������ ���� ������ ���������." << endl;
		return 0;
	}
	if (N % Shift != 0)
	{
		cout << "������ ������� ������ ���� ������ ����� �� ����� �������" << endl;
		return 0;
	}

	// ���������� ������������ ��������� (OpenMP)
	double time = omp_get_wtime();
	ParallelResultCalculation(A, B, C, N, NumThreads);
	time = omp_get_wtime() - time;

	freopen_s(&answer, "matr.out", "wb", stdout);
	// ������ ������� ����������
	fwrite(&time, sizeof(time), 1, stdout);
	// ������ ������� �������
	fwrite(&N, sizeof(N), 1, stdout);
	// ������ �������������� �������
	fwrite(C, sizeof(*C), N*N, stdout);

	fclose(stream);
	fclose(answer);
	delete[] A;
	delete[] B;
	delete[] C;

	return 0;
}