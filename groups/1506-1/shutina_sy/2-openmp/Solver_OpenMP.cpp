#define _CRT_SECURE_NO_WARNINGS

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <omp.h>
#include <math.h>
#include <vector>
using namespace std;

void radixPass(int num_byte, long n, double *source, double *dest, int *count) {
	// ��������� ����������
	double *sp;
	int s, c, i, *cp;
	unsigned char *bp;

	// ��� 3: ���-�� ��������� <i
	s = 0; 	// ��������� ����������, �������� ����� �� ������ ������
	cp = count;
	for (i = 256; i > 0; --i, ++cp) {
		c = *cp;
		*cp = s;
		s += c;
	}

	// ��� 4: ������������� ����������
	bp = (unsigned char *)source + num_byte;
	sp = source;
	for (i = n; i > 0; --i, bp += sizeof(double), ++sp) {
		cp = count + *bp;
		dest[*cp] = *sp;
		++(*cp);
	}
}

// ������� ��� ���������� ������� ��� ����������� ���������� ����� � ��������� ������
void floatRadixLastPass(int num_byte, int n, double *source, double *dest, int *count) {
	double *sp;
	int s, c, i, *cp;
	unsigned char *bp;

	int numNeg = 0; // ���������� ������������� �����
	for (i = 128; i < 256; i++)
		numNeg += count[i];

	// ������ 128 ��������� count ��������� � ������������� ������.
	// ����������� ����� ������� �����, ������� �� numNeg 
	s = numNeg;
	cp = count;
	for (i = 0; i < 128; ++i, ++cp) {
		c = *cp;
		*cp = s;
		s += c;
	}

	// ���������, ���������� ��������� ������������ ������������� �����.
	s = count[255] = 0;                // ������������� ����� ������������� �� ������ �������
	cp = count + 254;
	for (i = 254; i >= 128; --i, --cp) {
		*cp += s;
		s = *cp;
	}

	bp = (unsigned char *)source + num_byte;
	sp = source;
	for (i = n; i > 0; --i, bp += sizeof(double), ++sp) {
		cp = count + *bp;
		if (*bp < 128)
			dest[(*cp)++] = *sp;
		else
			dest[--(*cp)] = *sp;
	}
}

// ������� ��������.
// data-����������� ������, counters-������ ��� ���������, N-����� ��������� � data
void createCounters(double *data, int *counters, int n) {
	// i-� ������ count ����������, ������� � ������ counters+256*i
	memset(counters, 0, 256 * sizeof(double) * sizeof(int));

	unsigned char *bp = (unsigned char*)data;
	unsigned char *dataEnd = (unsigned char*)(data + n);
	int i;

	while (bp != dataEnd) {
		// ����������� ���������� ���� �� ��������� *bp
		// i - ������� ������ ���������
		for (i = 0; i < sizeof(double); i++)
			counters[256 * i + *bp++]++;
	}
}

// ����������� ���������� ����� � ��������� ������
void doubleRadixSort(double* in, int n) {
	double *out = new double[n];
	unsigned char i;

	int *counters = new int[sizeof(double) * 256], *count;
	createCounters(in, counters, n);

	for (i = 0; i < sizeof(double) - 1; i++) {
		count = counters + 256 * i; // count - ������ ��������� ��� i-�� �������

		if (count[0] == n) continue; //**

		radixPass(i, n, in, out, count);
		swap(in, out);
	}
	count = counters + 256 * i;
	floatRadixLastPass(i, n, in, out, count);

	delete in;
	in = out;
	delete[] counters;
}


void MergeAndSort(const std::vector<double> vec1, const std::vector<double> vec2, double* mas)
{
	int i = 0, j = 0;
	int size1 = vec1.size(), size2 = vec2.size();

	while (i < size1 && j < size2) {
		mas[i + j] = vec1[i];
		mas[i + j + 1] = vec2[j];
		++i; ++j;

	}

	while (i < size1) {
		mas[size2 + i] = vec1[i];
		i++;

	}
	while (j < size2) {
		mas[size1 + j] = vec2[j];
		j++;

	}

	i = 1;
	while (i < size1 + size2 - 1) {
		if (mas[i] > mas[i + 1]) {
			double k = mas[i];			
			mas[i] = mas[i + 1];
			mas[i + 1] = k;

		}
		++i;

	}	
}
enum elemType {
	EVEN,
	ODD

};


void SelectElements(elemType type, const double* arr1, int size1, const double* arr2, int size2, std::vector<double>& result) {
	int i, j;
	if (type == EVEN) i = 0, j = 0;
	else i = 1, j = 1;
	result.reserve(size1 + size2);
	while (i < size1 && j < size2) {
		if (arr1[i] <= arr2[j])
		{
			result.push_back(arr1[i]);
			i += 2;
		}
		else
		{
			result.push_back(arr2[j]);
			j += 2;
		}

	}
	if (i >= size1)
		while (j < size2) {
			result.push_back(arr2[j]);
			j += 2;

		}
	else
		while (i < size1) {
			result.push_back(arr1[i]);
			i += 2;

		}	
}

void Sort_Parallel_OpenMP(int nThreads, int n, double* arr) {
	int step;
	std::vector<double>* temp = new std::vector<double>[nThreads];
	int *shift = new int[nThreads];
	memset(shift, 0, nThreads * 4);
	int *count = new int[nThreads];
	memset(count, 0, nThreads * 4);
#pragma omp parallel  shared(arr, step, shift, count, temp) num_threads(nThreads)
	{
		int tid, index;
		tid = omp_get_thread_num();

		shift[tid] = tid*(n / nThreads);
		count[tid] = (tid == nThreads - 1) ? n - tid * (n / nThreads) : n / nThreads;
		doubleRadixSort(arr + shift[tid], count[tid]);
#pragma omp barrier

		step = 1;
		while (step <= log(nThreads) / log(2.0)) {
			index = (int)pow(2, step - 1);
			if (tid % (index * 2) == 0) {
				SelectElements(EVEN, arr + shift[tid], count[tid], arr + shift[tid + index], count[tid + index], temp[tid]);

			}
			else if (tid % index == 0) {
				SelectElements(ODD, arr + shift[tid], count[tid], arr + shift[tid - index], count[tid - index], temp[tid]);

			}

#pragma omp barrier
			if (tid % (index * 2) == 0) {
				MergeAndSort(temp[tid], temp[tid + index], arr + shift[tid]);
				count[tid] += count[tid + index];
				temp[tid].clear(); temp[tid].shrink_to_fit();
				temp[tid + index].clear(); temp[tid + index].shrink_to_fit();
			}
#pragma omp single
			{
				step++;
			}

		}
	}
	delete[] temp;
	delete[] count;
	delete[] shift;
}

int main(int argc, char* argv[]) {
	int nThreads;

	if (argc > 3) {
		freopen(argv[1], "rb", stdin);
		freopen(argv[2], "wb", stdout);
		nThreads = atoi(argv[3]);
	}
	else {
		if (argc > 1) {
			nThreads = atoi(argv[1]);
			freopen("array.in", "rb", stdin);
			freopen("array.out", "wb", stdout);
		}
		else return 1;
	}
	int n;
	fread(&n, sizeof(n), 1, stdin);

	double* arr = new double[n];
	fread(arr, sizeof(*arr), n, stdin);
	
	double start_time = omp_get_wtime();	
	Sort_Parallel_OpenMP(nThreads,n,arr);
	double time = omp_get_wtime() - start_time;
	
	fwrite(&n, sizeof(n), 1, stdout);
	fwrite(arr, sizeof(*arr), n, stdout);
	fwrite(&time, sizeof(time), 1, stdout);
	
	delete[] arr;

	return 0;
}


