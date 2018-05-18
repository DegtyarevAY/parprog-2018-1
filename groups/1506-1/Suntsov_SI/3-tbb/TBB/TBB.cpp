
#include  "iostream"
#include <random>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <fstream>
#include <string>
#include <omp.h>
#include "tbb/task_scheduler_init.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"

using namespace tbb;
using namespace std;

int ADD(double* MatrixA, double* MatrixB, double* MatrixResult, int MatrixSize)
{
	for (int i = 0; i < MatrixSize*MatrixSize; i++)
	{

		MatrixResult[i] = MatrixA[i] + MatrixB[i];

	}
	return 0;
}

int SUB(double* MatrixA, double* MatrixB, double* MatrixResult, int MatrixSize)
{
	for (int i = 0; i < MatrixSize*MatrixSize; i++)
	{

		MatrixResult[i] = MatrixA[i] - MatrixB[i];

	}
	return 0;
}

int MUL(double* MatrixA, double * MatrixB, double* MatrixResult, int MatrixSize)
{
	for (int i = 0; i<MatrixSize; i++)
	{
		for (int j = 0; j<MatrixSize; j++)
		{
			MatrixResult[i*MatrixSize + j] = 0;
			for (int k = 0; k<MatrixSize; k++)
			{
				MatrixResult[j + MatrixSize*i] += MatrixA[i*MatrixSize + k] * MatrixB[j + MatrixSize*k];
			}
		}
	}
	return 0;
}

class StrassenTask : public task {
public:
	int N;
	double *MatrixA;
	double *MatrixB;
	double *MatrixC;
	StrassenTask(int N_, double* MatrixA_, double* MatrixB_, double* MatrixC_) :
		N(N_), MatrixA(MatrixA_), MatrixB(MatrixB_), MatrixC(MatrixC_)
	{}
	task* execute() {

		if (N <= 4)
		{
			MUL(MatrixA, MatrixB, MatrixC, N);
		}
		else
		{
			task_list list;
			int count = 1;
			int HalfSize = N / 2;
			int newSize = N / 2;

			double* A11;
			double* A12;
			double* A21;
			double* A22;

			double* B11;
			double* B12;
			double* B21;
			double* B22;

			double* C11;
			double* C12;
			double* C21;
			double* C22;

			double* M1;
			double* M2;
			double* M3;
			double* M4;
			double* M5;
			double* M6;
			double* M7;
			double* tmp1;
			double* tmp2;
			double* tmp3;
			double* tmp4;
			double* tmp5;
			double* tmp6;
			double* tmp7;
			double* tmp8;
			double* tmp9;
			double* tmp10;
			double* AResult;
			double* BResult;

			A11 = new double[newSize*newSize];
			A12 = new double[newSize*newSize];
			A21 = new double[newSize*newSize];
			A22 = new double[newSize*newSize];

			B11 = new double[newSize*newSize];
			B12 = new double[newSize*newSize];
			B21 = new double[newSize*newSize];
			B22 = new double[newSize*newSize];

			C11 = new double[newSize*newSize];
			C12 = new double[newSize*newSize];
			C21 = new double[newSize*newSize];
			C22 = new double[newSize*newSize];

			M1 = new double[newSize*newSize];
			M2 = new double[newSize*newSize];
			M3 = new double[newSize*newSize];
			M4 = new double[newSize*newSize];
			M5 = new double[newSize*newSize];
			M6 = new double[newSize*newSize];
			M7 = new double[newSize*newSize];

			AResult = new double[HalfSize * HalfSize];
			BResult = new double[HalfSize * HalfSize];
			tmp1 = new double[HalfSize * HalfSize];
			tmp2 = new double[HalfSize * HalfSize];
			tmp3 = new double[HalfSize * HalfSize];
			tmp4 = new double[HalfSize * HalfSize];
			tmp5 = new double[HalfSize * HalfSize];
			tmp6 = new double[HalfSize * HalfSize];
			tmp7 = new double[HalfSize * HalfSize];
			tmp8 = new double[HalfSize * HalfSize];
			tmp9 = new double[HalfSize * HalfSize];
			tmp10 = new double[HalfSize * HalfSize];

			for (int i = 0; i < N / 2; i++)
			{
				for (int j = 0; j < N / 2; j++)
				{
					A11[j + i*N / 2] = MatrixA[j + i*N];
					A12[j + i*N / 2] = MatrixA[i*N + (j + N / 2)];
					A21[j + i*N / 2] = MatrixA[(i + N / 2)*N + j];
					A22[j + i*N / 2] = MatrixA[(i + N / 2)*N + (j + N / 2)];

					B11[j + i*N / 2] = MatrixB[j + i*N];
					B12[j + i*N / 2] = MatrixB[i*N + (j + N / 2)];
					B21[j + i*N / 2] = MatrixB[(i + N / 2)*N + j];
					B22[j + i*N / 2] = MatrixB[(i + N / 2)*N + (j + N / 2)];

				}
			}



			//M1[][]
			ADD(A11, A22, tmp1, HalfSize);
			ADD(B11, B22, tmp2, HalfSize);
			count++;
			list.push_back(*new (allocate_child()) StrassenTask(HalfSize, tmp1, tmp2, M1));

			//M2[][]
			ADD(A21, A22, tmp3, HalfSize);  //M2=(A21+A22)B11
											//Mul(AResult,B11,M2);
			count++;
			list.push_back(*new (allocate_child()) StrassenTask(HalfSize, tmp3, B11, M2));


			//M3[][]
			SUB(B12, B22, tmp4, HalfSize);              //M3=A11(B12-B22)
														//Mul(A11,BResult,M3);
			count++;
			list.push_back(*new (allocate_child()) StrassenTask(HalfSize, A11, tmp4, M3));


			//M4[][]
			SUB(B21, B11, tmp5, HalfSize);           //M4=A22(B21-B11)
													 //Mul(A22,BResult,M4);
			count++;
			list.push_back(*new (allocate_child()) StrassenTask(HalfSize, A22, tmp5, M4));


			//M5[][]
			ADD(A11, A12, tmp6, HalfSize);           //M5=(A11+A12)B22
													 //Strassen(HalfSize, AResult, B22, M5);       
													 //Mul(AResult,B22,M5);
			count++;
			list.push_back(*new (allocate_child()) StrassenTask(HalfSize, tmp6, B22, M5));


			//M6[][]
			SUB(A21, A11, tmp7, HalfSize);
			ADD(B11, B12, tmp8, HalfSize);             //M6=(A21-A11)(B11+B12)
													   //Mul(AResult,BResult,M6);
			count++;
			list.push_back(*new (allocate_child()) StrassenTask(HalfSize, tmp7, tmp8, M6));


			//M7[][]
			SUB(A12, A22, tmp9, HalfSize);
			ADD(B21, B22, tmp10, HalfSize);
			//M7=(A12-A22)(B21+B22)
			//Mul(AResult,BResult,M7);
			count++;
			list.push_back(*new (allocate_child()) StrassenTask(HalfSize, tmp9, tmp10, M7));

			set_ref_count(count);
			spawn_and_wait_for_all(list);

			//C11 = M1 + M4 - M5 + M7;
			ADD(M1, M4, AResult, HalfSize);
			SUB(M7, M5, BResult, HalfSize);
			ADD(AResult, BResult, C11, HalfSize);

			//C12 = M3 + M5;
			ADD(M3, M5, C12, HalfSize);

			//C21 = M2 + M4;
			ADD(M2, M4, C21, HalfSize);

			//C22 = M1 + M3 - M2 + M6;
			ADD(M1, M3, AResult, HalfSize);
			SUB(M6, M2, BResult, HalfSize);
			ADD(AResult, BResult, C22, HalfSize);



			for (int i = 0; i < HalfSize; i++)
			{
				for (int j = 0; j < HalfSize; j++)
				{
					MatrixC[i*N + j] = C11[i*HalfSize + j];
					MatrixC[i*N + (j + N / 2)] = C12[i*HalfSize + j];
					MatrixC[(i + N / 2)*N + j] = C21[i*HalfSize + j];
					MatrixC[(i + N / 2)*N + (j + N / 2)] = C22[i*HalfSize + j];
				}
			}
			N = N / 2;
			delete[] A11; delete[] A12; delete[] A21; delete[] A22;
			delete[] B11; delete[] B12; delete[] B21; delete[] B22;
			delete[] C11; delete[] C12; delete[] C21; delete[] C22;
			delete[] M1; delete[] M2; delete[] M3; delete[] M4; delete[] M5;
			delete[] M6; delete[] M7;
			delete[] AResult;
			delete[] BResult;
			delete[] tmp1; delete[] tmp2; delete[] tmp3; delete[] tmp4;
			delete[] tmp5; delete[] tmp6; delete[] tmp7; delete[] tmp8;
			delete[] tmp9; delete[] tmp10;
		}
		return NULL;
	}
};


int main(int argc, char * argv[])
{
	FILE *matr_in, *matr_out;

	char* fileName = "matr.in";
	char* answerName = "matr.out";
	int realSize = 0; //�������� ������ �������



	if (argc > 1) {
		fileName = argv[1];
		string str = string(argv[1]) + string(".out");
		answerName = (char*)str.c_str();
	}
	freopen_s(&matr_in, "matr.in", "rb", stdin);

	fread(&realSize, sizeof(int), 1, stdin);
	int N = 0;

	N = (int)(log2(realSize));
	int k = pow(2, N);
	if (realSize == k) { N = realSize; }
	else
	{
		N++;
		N = pow(2, N);
	}


	double *A = new double[N*N];
	double *B = new double[N*N];
	double *C = new double[N*N];
	double *C_new = new double[realSize*realSize];

	for (int i = 0; i<N; i++) {
		for (int j = 0; j<N; j++) {

			A[i*N + j] = 0.0;
			B[i*N + j] = 0.0;
			C[i*N + j] = 0.0;

		}

	}


	for (int i = 0; i<realSize; i++) {
		for (int j = 0; j<realSize; j++) {
			fread(&A[i*N + j], sizeof(double), 1, stdin);
		}
	}

	for (int i = 0; i < realSize; i++) {
		for (int j = 0; j < realSize; j++) {

			fread(&B[i*N + j], sizeof(double), 1, stdin);

		}
	}

	double time = omp_get_wtime();
	StrassenTask& a = *new(task::allocate_root()) StrassenTask(N, A, B, C);
	task::spawn_root_and_wait(a);

	time = omp_get_wtime() - time;
	cout << time;


	for (int i = 0; i < realSize; i++) {
		for (int j = 0; j < realSize; j++) {
			C_new[i*realSize + j] = C[i*N + j];

		}

	}



	FILE * file_out = fopen("matr.out", "wb");
	fwrite(&realSize, sizeof(realSize), 1, file_out);
	fwrite(&time, sizeof(time), 1, file_out);
	fwrite(C_new, sizeof(*C_new), realSize*realSize, file_out);


	fclose(matr_in);
	fclose(file_out);
	system("pause");
	delete[] A;
	delete[] B;
	delete[] C;
	delete[] C_new;
	return 0;

}