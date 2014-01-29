// Conway.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "config.h"
using namespace std;

void train();
void test();

int _tmain(int argc, _TCHAR* argv[])
{
	srand(time(NULL));
	//train();
	test();
	return 0;
}

void train()
{
	/* Training */
	int percent = 0;
	cout << "Now Training:" << setw(4) << percent << "%";
	cout << "\b";
	while(1)
	{
		Config cfg;
		cfg.update(STEP);
		cfg.save();
		int cov = (Config::count * 25 >> 17);
		if(cov != percent)
		{
			cout << "\b\b\b\b" << setw(4) << cov;
			percent = cov;
		}
		if(percent == 100) break;
	}
	cout << endl << "Finished!" << endl;
	Config::record("result.csv");
	system("pause");
}

void test()
{
	Config::generate("result.csv");
	int percent = 0;
	cout << "Now Testing:" << setw(4) << percent << "%";
	cout << "\b";
	unsigned long error = 0;
	while(1)
	{
		Config cfg;
		cfg.update(STEP);
		cfg.lookup();
		error += cfg.check();

		int cov = (Config::count >> 10);
		if(cov != percent)
		{
			cout << "\b\b\b\b" << setw(4) << cov;
			percent = cov;
		}
		if(percent == 100) break;
	}
	cout << endl << "Finished!" << endl;
	cout << "Error rate:" << (error + 0.0) / (400 * 1024) << endl;
	system("pause");
}