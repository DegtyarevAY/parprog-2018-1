#pragma once
#include <omp.h> 
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <ctime>
using namespace cv;
using namespace std;

void linearsPARALLEL(Mat source, Mat result,int CountOfProc)
{
	//unsigned int start_time = clock();
	for (int i = 0; i < result.rows; ++i)
		for (int j = 0; j < result.cols; ++j)
			result.at<uchar>(i, j) = source.at<uchar>(i, j);
	int *hist;
	hist = new int[256];
	memset(hist, 0, 256 * sizeof(int));
	int min = INT_MAX;
	int max = INT_MIN;
	int tid;
	omp_set_num_threads(CountOfProc);
#pragma omp parallel shared(hist,source) private(tid)
	{
		int n = omp_get_max_threads();
		tid = omp_get_thread_num();
		//cout << "hi,baby, i'm thread " << tid << endl;
		if (tid == 0) cout << "Set threads: " << omp_get_num_threads() << endl << "Opportunity: " << n << endl;

#pragma omp for schedule(guided)
		for (int i = 0; i < source.rows; i++)
		{//cout << "hi,baby, i'm thread " << tid << endl;
			for (int j = 0; j < source.cols; j++)
			{
				hist[source.at<uchar>(i, j)]++;
			}
		}
	}
		painthist(source);
	


	int i = 0;
	while (i < 256 && hist[i] == 0) i++;
	min = i;
	i = 255;
	while (i > 0 && hist[i] == 0) i--;
	max = i;
#pragma omp parallel shared(hist,result) private(tid)
	{
		int n = omp_get_max_threads();
		tid = omp_get_thread_num();
		if (tid == 0) cout << "Set threads: " << omp_get_num_threads() << endl << "Opportunity: " << n << endl;
		#pragma omp for schedule(guided)
		for (int i = 0; i <result.rows; i++)
		{//cout << "hi,baby, i'm thread " << tid << endl;
			for (int j = 0; j < result.cols; j++)
			{
				result.at<uchar>(i, j) = round(255 * (result.at<uchar>(i, j) - min) / (max - min));
			}
		}
	}
	//unsigned int end_time = clock(); // �������� �����
	//unsigned int search_time = end_time - start_time; // ������� �����
	//cout << search_time << endl;
	painthist(result);

}


