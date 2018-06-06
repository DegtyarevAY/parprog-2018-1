#include <string>
#include <fstream>

int main(int argc, char * argv[])
{
	std::string filename;
	std::ofstream textfile;
	std::ifstream binfile;

	int N;
	int tmp;
	double time;

	if (argc <= 1)
		return 0;
	else
		filename = argv[1];


	if (filename.find(".") == -1)
		filename += ".";

	binfile.open(filename, std::ios::binary);
	if (!binfile.is_open())
		return 0;

	if (filename.find(".\\") == 0)
		filename.erase(0, 2);

	filename.replace(filename.find('.'), 1, "_");
	textfile.open(filename + ".txt");
	//textfile.open(filename.substr(0, filename.find('.')) + "_out.txt");

	
	binfile.read((char*)(&N), sizeof(int));
	textfile << N << "\n";
	

	for (size_t i = 0; i < 2*N; ++i)
	{
		if (i % 2 == 0)
		{
			binfile.read((char*)(&tmp), sizeof(int));
			textfile << "(" << tmp <<", ";
		}
		if (i % 2 == 1)
		{
			binfile.read((char*)(&tmp), sizeof(int));
			textfile << tmp << ")\n";
		}

	}
	binfile.read((char*)(&time), sizeof(double)); // ���������� �������
	textfile << "Time: "<< time<<" seconds";

	textfile.close();
	binfile.close();
}