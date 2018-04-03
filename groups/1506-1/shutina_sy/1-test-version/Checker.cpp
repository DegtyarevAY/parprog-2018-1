// Checker.cpp: ���������� ����� ����� ��� ����������� ����������.
//
#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<cmath>
#include<string>
#include<algorithm>
#include <iostream>
using namespace std;


enum verdict { NO = 1, AC, WA, CE, ML, TL, RE, IL, PE, DE };

/*
// Checker ����� ������������� ��� ��� ��� ��������:
AC = Accepted = ������� ����� ���������� ��������� �� ������ �����
WA = Wrong Answer = ������� ����� ������������ ��������� �� ������ �����
*/

class result
{
private:
	FILE *bur;
public:
	enum ext_cls { NO = 1, VERDICT, MESSAGE, TIME, MEMORY };
	result(bool read = false)
	{
		if (read) bur = fopen("result.txt", "r"); else bur = fopen("result.txt", "w");
	}
	~result() { fclose(bur); }
	void write_type(ext_cls t) { fwrite(&t, sizeof(t), 1, bur); }
	// �������� ����������� �������, ��� ������� �������� ���� �� ��������� verdict
	void write_verdict(verdict v)
	{
		write_type(ext_cls::VERDICT); fwrite(&v, sizeof(v), 1, bur);
	}
	// �������� ��������� �� checker'a ������������.
	// ��������, ��� ������� ������, ��� ��������.
	// ������������ ������ ��������� ����� � ����� ����������
	void write_message(string str)
	{
		write_type(ext_cls::MESSAGE); int l = str.size(); fwrite(&l, sizeof(l), 1, bur);
		fwrite(&str[0], sizeof(str[0]), l, bur);
	}
	// �������� ����������� ������� ����� ������ ��������� ���������,	
	// x ����� ����������� 100 �� = 10 ^ (-7) ���
	void write_time(long long x)
	{
		write_type(ext_cls::TIME); fwrite(&x, sizeof(x), 1, bur);
	}
	// �������� ����������� �������, ������ ������������� ���������� ���������
	void write_memory(unsigned long long x)
	{
		write_type(ext_cls::MEMORY); fwrite(&x, sizeof(x), 1, bur);
	}
} checker_result;


int main(int argc, char* argv[])
{
	if (argc > 2) {
		freopen(argv[1], "rb", stdin);
		freopen(argv[2], "rb", stdout);
	}
	else {		
		// ��������� ���� ������� ������, ���� �������� ������ � ����� ���������
		freopen("array.in", "rb", stdin);
		freopen("array.out", "rb", stdout);		
	}

	int n;
	// ��������� ����������� 
	fread(&n, sizeof(n), 1, stdout);
	fread(&n, sizeof(n), 1, stdin);
	// �������� ������ ��� ������� ������ ���� � ������ ���������
	double * ans = new double[n], *res = new double[n];
	/*double ans_time, res_time;*/

	// ��������� ����� ������ ��������� ��������� � ������ ���������

	long long res_time;
	
	fread(res, sizeof(*res), n, stdout);
	fread(&res_time, sizeof(res_time), 1, stdout);
	// ���������  ������ 
	fread(ans, sizeof(*ans), n, stdin);
	sort(ans, ans + n);
	// ��������� ������, ��� ������� ����� �������� �������
	double diff = 0.0;
	for (int i = 0; i < n; i++)
		diff += (ans[i] - res[i]) * (ans[i] - res[i]);
	// ���������, ��� ������ ����, ����� ��������, ��� ������� ���������, ����� - �����������.	
	if (diff < 1e-6)
	{
		checker_result.write_message("AC. Numbers are equal.");
		checker_result.write_verdict(verdict::AC);		
	}
	else
	{
		checker_result.write_message("WA. Output is not correct.");
		checker_result.write_verdict(verdict::WA);		
	}
	// ���������� ����� � ���������� ����������� (��������� �� 100 �� = 10 ^ (-7) ���).
	
	checker_result.write_time(res_time * 1e7);

	delete[] ans;
	delete[] res;
	return 0;
}

