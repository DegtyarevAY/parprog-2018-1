// Shutina_14.cpp: ���������� ����� ����� ��� ����������� ����������.
//
//Shutina_14.cpp: ���������� ����� ����� ��� ����������� ����������.

#define _CRT_SECURE_NO_WARNINGS

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <omp.h>
using namespace std;

// ������� radixPass ��������� � �������� ����������
// ����� ����� num_byte,
// ����� ��������� n, 
// �������� ������ source, 
// ������ dest, ���� ����� ������������ �����, ��������������� �� ����� num_byte
// ������ ��������� count, ��������������� �������� �������.

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
	for (i = 128; i<256; i++)
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
		for (i = 0; i<sizeof(double); i++)
			counters[256 * i + *bp++]++;
	}
}

// ����������� ���������� ����� � ��������� ������
void floatRadixSort(double* &in, int n) {
	double *out = new double[n];
	unsigned char i;

	int *counters = new int[sizeof(double) * 256], *count;
	createCounters(in, counters, n);

	for (i = 0; i<sizeof(double) - 1; i++) {
		count = counters + 256 * i; // count - ������ ��������� ��� i-�� �������

		if (count[0] == n) continue; //**

		radixPass(i, n, in, out, count);
		swap(in, out);
	}
	count = counters + 256 * i;
	floatRadixLastPass(i, n, in, out, count);

	delete in;
	in = out;
	delete counters;
}

int main(int argc, char* argv[]) {
	if (argc > 2) {
		freopen(argv[1], "rb", stdin);
		freopen(argv[2], "wb", stdout);
	}
	else {
		freopen("array.in", "rb", stdin);
		freopen("array.out", "wb", stdout);
	}

	int n;
	fread(&n, sizeof(n), 1, stdin);

	double* arr = new double[n];
	fread(arr, sizeof(*arr), n, stdin);

	double start_time = omp_get_wtime();

	floatRadixSort(arr, n);

	double time = omp_get_wtime() - start_time;

	fwrite(&n, sizeof(n), 1, stdout);
	fwrite(arr, sizeof(*arr), n, stdout);
	fwrite(&time, sizeof(time), 1, stdout);

	delete[] arr;

	return 0;
}



