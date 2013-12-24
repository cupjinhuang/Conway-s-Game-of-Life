// Conway.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "config.h"
using namespace std;

void train();
void test();

int _tmain(int argc, _TCHAR* argv[])
{
	//train();
	test();
	return 0;
}

void train()
{
	srand(time(NULL));
	/* Training */
	int percent = 0;
	cout << "Now Generating Tree:";
	for(int depth = 0; depth < DEPTH; depth ++)
	{
		cout << endl << "Round " << depth << ", Node Count " << Node::count << setw(4) << percent << "%";
		cout << "\b";
		while(1)
		{
			Config cfg;
			cfg.update(STEP);
			cfg.save();
			int cov = (Config::count * 25 >> 10);
			if(cov != percent)
			{
				cout << "\b\b\b\b" << setw(4) << cov;
				percent = cov;
			}
			if(percent == 100) break;
		}
		Config::head->update();
		Config::count = 0;
	}
	cout << endl << "Tree Generated!" << endl;
	percent = 0;
	cout << "Now Training:" << setw(4) << percent << "%";
	cout << "\b";
	while(1)
	{
		Config cfg;
		cfg.update(STEP);
		cfg.save();
		int cov = (Config::count * 25 >> 15);
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
	/* Testing */
	Config::generate("result.csv");
	cout << "Tree Generated: " << Node::count << endl;
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
	cout << "Error rate:" << ((error / 400) / 1024.0) << "%" << endl;
	system("pause");
}