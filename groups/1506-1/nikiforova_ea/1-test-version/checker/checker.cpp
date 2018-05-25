#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include "sol.h"

#include <ctime>

using namespace std;
using namespace cv;

/*
// Checker ����� ������������� ��� ��� ��� ��������:
AC = Accepted = ������� ����� ���������� ��������� �� ������ �����
WA = Wrong Answer = ������� ����� ������������ ��������� �� ������ �����
PE = Presentation Error = ������ ������� �������� ������
// ��������� �������� checker �� ����� �������������
NO = No verdict = ������� �����������
CE = Compilation Error = ������ ����������
ML = Memory Limit Exceeded = ��������� ����������� �� ������
TL = Time Limit Exceeded = ��������� ����������� �� ������� ������
RE = Runtime Error = ������ ������� ���������� ���������
IL = Idle Limit Exceeded = ��������� ����� �������(�����������) ���������
DE = Deadly Error = ������ ����������� �������
*/
enum verdict { NO = 1, AC, WA, CE, ML, TL, RE, IL, PE, DE };


class Result
{
public:
	Result(const string& result_path)
	{
		result_checker.open(result_path);
	}

	~Result()
	{
		result_checker.close();
	}

private:
	ofstream result_checker;
public:
	enum class ext_cls { NO = 1, VERDICT, MESSAGE, TIME, MEMORY };
	void write_type(ext_cls param)
	{
		result_checker << static_cast<int>(param) << endl;
	}
	void write_verdict(verdict v)
	{
		result_checker << v;
	}
	void write_message(string message)
	{
		result_checker << message << "\n";
	}
};
bool viewer(string filename, Mat& output)
{
	ifstream ifs(filename, ios::binary);
	if (ifs.is_open())
	{
		int len, wid, type;

		ifs.read((char*)(&len), sizeof(int));

		if (len == 0)
		{
			return true;
		}
		ifs.read((char*)(&wid), sizeof(int));
		ifs.read((char*)(&type), sizeof(int));

		output.release();
		output.create(len, wid, type);
		ifs.read((char*)(output.data), output.elemSize() * output.total());
		return true;
	}
	else return false;
}
bool compareres(Mat fir, Mat sec)
{
	int k = 0;
	for (int i = 0; i < fir.rows; i++)
	{
		for (int j = 0; j < fir.cols; j++)
		{
			if (fir.at<uchar>(i, j) != sec.at<uchar>(i, j)) k = 1;			
		}
	}
	if (k == 1) return false;
	else return true;
}

int main(int argc, char* argv[])
{
	int numtest = atoi(argv[1]);		//����� �����
	string inputf = argv[2];            //��� �������� ����� (������ �� ����, ���������� ����)
	string outpf = argv[3];				//��� ��������� ����� (������ �� ����, ���������� ����, ����� ��� ����� � �������� ��������)
	string dir= "test2\\";
	string filename = dir + inputf;
	Result checker(dir);
	Mat basic;
	viewer(filename + ".ans", basic);
	Mat aftertest;
	filename = dir + outpf;
	viewer(filename + ".ans", aftertest);

	if (compareres(aftertest, basic)==false)
	{
		checker.write_message("Images doesn't match");
		cout << "Images doesn't match";
	}
	else
	{
		checker.write_message("Images are equal");
		cout << "Images are equal";
	}
}

