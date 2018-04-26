#include "../../include/resultStruct.h"
#include "../../include/checkerStruct.h"
#include <string>
#include <iostream>
#include <istream>
#include <fstream>
using namespace std;

// ������������ ��� �������������� � ����������� �������� 
//////////////////////////////////////////////////////////////////////////////////////////// 
/* // Checker ����� ������������� ��� ��� ��� ��������: 
AC = Accepted = ������� ����� ���������� ��������� �� ������ ����� 
WA = Wrong Answer = ������� ����� ������������ ��������� �� ������ ����� 
PE = Presentation Error = ������ ������� �������� ������ 

// ��������� �������� checker �� ����� ������������� 
NO = No verdict = ������� ����������� 
CE = Compilation Error = ������ ���������� 
ML = Memory Limit Exceeded = ��������� ����������� �� ������ 
TL = Time Limit Exceeded = ��������� ����������� �� ������� ������ 
RE = Runtime Error = ������ ������� ���������� ��������� 
IL = Idle Limit Exceeded = ��������� ����� ������� (�����������) ��������� 
DE = Deadly Error = ������ ����������� ������� */ 

//enum verdict { NO = 1, AC, WA, CE, ML, TL, RE, IL, PE, DE }; 

class result {
private: 
	ofstream res;
public:
	void Open(string str) {
		if (str == "") {
			res.open("CheckRes111.dat", ios::binary, ios::trunc);
		}
		else
			res.open(str, ios::binary, ios::trunc);
	}

	void write_verdict(verdict v)	{ 
		res.write((char*)&v, sizeof(v));
	}

	void WriteMessage(string str) {
		size_t len = str.length() + 1;
		res.write((char*)&len, sizeof(len)); // ������ �����
		res.write((char*)str.c_str(), len);  // ������ ������
	}

	void Close() {
		res.close();
	}
} checker_result;



void TakeCheckRes(string CheckResName, ifstream& Seq, ifstream& Ref) {
	checker_result.Open(CheckResName);
	//while (1) {
		ResultOut DataRef;
		ResultOut DataSeq;
		DataRef.Read(Ref);
		DataSeq.Read(Seq);
		/*if (Seq.eof()) break;
		else {*/
			if (abs(DataSeq.result - DataRef.result)*abs(DataSeq.result - DataRef.result) < 1) {
				checker_result.WriteMessage("The solution is correct");
			}				
			else {
				checker_result.WriteMessage("The solution is uncorrect");
			}
				
		//}
	//}
	checker_result.Close();
}

int main(int argc, char* argv[])
{
	string CheckResName = "";
	ofstream CheckRes;		//Write
	ifstream SeqRes;		//Read
	ifstream RefRes;

	if (argc == 4) {
		CheckResName = argv[1];
		string SeqResName = argv[2];
		string RefResName = argv[3];

		SeqRes.open(SeqResName, ios::binary);
		RefRes.open(RefResName, ios::binary);
	}
	else {
		cout << "Invalid count of arguments command line" << endl;
		SeqRes.open("SeqRes.ans", ios::binary);
		RefRes.open("RefRes.ans", ios::binary);
	}

	if (!SeqRes.is_open() || !RefRes.is_open())
		cout << "Do not open file" << endl;
	else
		TakeCheckRes(CheckResName, SeqRes, RefRes);

	SeqRes.close();
	RefRes.close();
	return 0;
}

