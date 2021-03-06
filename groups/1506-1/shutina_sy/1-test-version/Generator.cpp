// Generator.cpp: ���������� ����� ����� ��� ����������� ����������.
//
#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <chrono>
#include <ctime>

using namespace std;

int n_tests[] = { 1,10,256,500,600,900,1000,1500,2000,2500,3000,3500,40000,4500,5000,5500,6000,6500,7000,10000 };

int main(int argc, char* argv[])
{
	int n;
	int a, b;
	double t= 0;
	if (argc > 2) {		
		n = n_tests[atoi(argv[1])];
		freopen(argv[2], "wb", stdout);
	}
	else {		
		if (argc > 1) {
			n = n_tests[atoi(argv[1])];
			freopen("array.in", "wb", stdout);
		}
	}	

	fwrite(&n, sizeof(n), 1, stdout);

	double* mass = new double[n];

	srand((double)time(NULL));
	for (int i = 0; i < n; i++) {
		mass[i] = (double)(rand() % 20001)/100-100;
	}
	fwrite(mass, sizeof(*mass), n, stdout);
	fwrite(&t, sizeof(t), 1, stdout);
	delete[] mass;

	return 0;
}

