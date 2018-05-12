#include "Parallel_Sort.h"
#include <iostream>
#include <math.h>
#include <vector>
using namespace std;

void Parallel_Sort::radixPass(int num_byte, long n, double *source, double *dest, int *count) {
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
void Parallel_Sort::floatRadixLastPass(int num_byte, int n, double *source, double *dest, int *count) {
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
void Parallel_Sort::createCounters(double *data, int *counters, int n) {
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
void Parallel_Sort::doubleRadixSort(double* in, int n) {
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