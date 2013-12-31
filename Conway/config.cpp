#include "stdafx.h"
#include "config.h"
#include "cmath"
using namespace std;

const int dic[81][2]={{0, 0},{-1, 0},{1, 0},{0, -1},{0, 1},{-1, -1},{-1, 1},{1, -1},{1, 1},
					  {-2, 0},{-2, -1},{-2, 1},{2, 0},{2, -1},{2, 1},{0, -2},{-1, -2},{1, -2},
					  {0, 2},{-1, 2},{1, 2},{-2, -2},{-2, 2},{2, -2},{2, 2},{-3, 0},{-3, -1},{-3, 1},
					  {-3, -2},{-3, 2},{3, 0},{3, -1},{3, 1},{3, -2},{3, 2},{0, -3},{-1, -3},{1, -3},
					  {-2, -3},{2, -3},{0, 3},{-1, 3},{1, 3},{-2, 3},{2, 3},{-3, -3},{-3, 3},{3, -3},
					  {3, 3},{-4, 0},{-4, -1},{-4, 1},{-4, -2},{-4, 2},{-4, -3},{-4, 3},{4, 0},{4, -1},
					  {4, 1},{4, -2},{4, 2},{4, -3},{4, 3},{0, -4},{-1, -4},{1, -4},{-2, -4},{2, -4},
					  {-3, -4},{3, -4},{0, 4},{-1, 4},{1, 4},{-2, 4},{2, 4},{-3, 4},{3, 4},{-4, -4},{-4, 4},{4, -4},{4, 4}};
unsigned long Node::count = 0;

Node::Node(Node* parent, bool lr): p(parent), l(NULL), r(NULL), id(parent ? parent->id * 2 + lr : 1), isLeaf(true), occ(0)
{
	count ++;
	for(int i = 0; i < 16; i ++)
	{
		app[i] = 0;
		prob[i] = 0;
	}
}


void Node::insert(unsigned long long con, int grid)
{
	if(isLeaf)
	{
		if(id == con)
		{
			occ++;
			app[grid]++;
			/*cout << hex << id << ", " << grid << endl;
			system("pause");*/
		}
		else
		{
			if(id > con / 2)
			{
				cout << "Error " << id << " : " << con << endl;
				system("pause");
			}
			else
			{
				isLeaf = false;
				l = new Node(this, 0);
				r = new Node(this, 1);
			}
		}
	}
	if(!isLeaf)
	{
		unsigned long long idx = con;
		while(idx / 2 != id) idx >>= 1;
		if(idx & 1) r->insert(con, grid);
		else l->insert(con, grid);
	}
}

unsigned long long Config::largo[65536][17] = {0};
double Config::dict[65536][16] = {0};
unsigned long Config::count = 0;
unsigned long Config::coverage = 0;
Config::Config(void)
{
	count ++;
	while(1)
	{
		int cnt = rand() % 381 + 10;
		if(cnt <= 200)
		{
			for(int i = 0; i < WID; i ++)
			{
				for(int j = 0; j < WID; j ++)
				{
					fig[i][j] = 0;
				}
			}
			while(cnt)
			{
				int r = rand() % WID;
				int c = rand() % WID;
				if(!fig[r][c])
				{
					fig[r][c] = 1;
					cnt --;
				}
			}
		}
		else
		{
			cnt = 400 - cnt;
			for(int i = 0; i < WID; i ++)
			{
				for(int j = 0; j < WID; j ++)
				{
					fig[i][j] = 1;
				}
			}
			while(cnt)
			{
				int r = rand() % WID;
				int c = rand() % WID;
				if(fig[r][c])
				{
					fig[r][c] = 0;
					cnt --;
				}
			}
		}	
		update(5);
		bool flag = false;
		for(int i = 0; i < WID; i ++)
		{
			for(int j = 0; j < WID; j ++)
			{
				if(fig[i][j] == 1)
				{
					flag = 1;
					break;
				}
			}
		}
		if(flag) break;
	}
	memcpy(origin, fig, WID * WID * sizeof(bool));
}

void Config::update(int cnt)
{
	for(int i = 0; i < cnt; i ++)
	{
		bool tmp[20][20];
		for(int j = 0; j < WID; j ++)
		{
			for(int k = 0; k < WID; k ++)
			{
				int scnt = 0;
				for(int l = -1; l < 2; l ++)
				{
					for(int m = -1; m < 2; m ++)
					{
						scnt += (l || m) * get(j + l, k + m);
					}
				}

				if(fig[j][k] == 1)
				{
					if(scnt == 2 || scnt == 3) tmp[j][k] = 1;
					else tmp[j][k] = 0;
				}
				else
				{
					if(scnt == 3) tmp[j][k] = 1;
					else tmp[j][k] = 0;
				}
			}
		}
		memcpy(fig, tmp, WID * WID * sizeof(bool));
	}
}

void Config::save()
{
	for(int i = 0; i < WID; i ++)
	{
		for(int j = 0; j < WID; j ++)
		{
			savecfg(i, j);
		}
	}
}

void Config::savecfg(int i, int j)
{
	unsigned long long subcon = 0;
	for(int k = -1; k < 3; k ++)
	{
		for(int l = -1; l < 3; l ++)
		{
			subcon += subcon + get(i + k, j + l);
		}
	}
	int grid = 0;
	for(int k = 0; k < 2; k ++)
	{
		for(int l = 0; l < 2; l ++)
		{
			if(i + k >= 0 && i + k < 20 && j + l >= 0 && j + l < 20)
			grid += grid + origin[i + k][j + l];
		}
	}
	largo[subcon][grid + 1] ++;
	largo[subcon][0]++;
}

void Config::record(string fileName)
{
	ofstream out(fileName.c_str());
	for(int i = 0; i < 65536; i ++)
	{
		if(largo[i][0])
		{
			double UB = 0;
			for(int j = 0; j < 16; j ++)
			{
				double prob = (largo[i][j + 1] + 0.0) / largo[i][0];
				if(prob == 0) out << -1 << ", ";
				else out << -log(prob) << ", ";
			}
		}
		else for(int j = 0; j < 16; j ++)
		{
			if(j == 0) out << 0 << ", ";
			else out << -1 << ", ";
		}
		out << endl;
	}
	out.close();
}

void Config::generate(string file)
{
	ifstream in(file.c_str());
	int i = 0;
	while(!in.eof())
	{
		for(int j = 0; j < 16; j ++)
		{
			double lp;
			char tmp;
			in >> lp;
			in >> tmp;
			dict[i][j] = lp;
		}
		i ++;
	}
	cout << "Data Read:" << i << endl;
	in.close();
}

void Config::lookup()
{
	bool tmp[WID][WID] = {0};
	for(int i = 0; i < WID * 2 - 1; i ++)
	{
		for(int j = 0; j <= i; j ++)
		{
			int k = i - j; // want to determine tmp[j][k]
			if(j >= WID || k >= WID) continue;
			bool local[6][6] = {0};
			for(int l = -3; l < 3; l ++)
			{
				for(int m = -3; m < 3; m ++)
				{
					local[l + 3][m + 3] = get(j + l, k + m);
				}
			}
			bool gue[4][4] = {0};
			for(int l = -2; l < 2; l ++)
			{
				for(int m = -2; m < 2; m ++)
				{
					if(l < 0 || l + m < 0)
					{
						bool vert = 0;
						if(l + j >= 0 && l + j < WID)
						{
							if(m + k >= 0 && m + k < WID)
							{
								vert = tmp[l + j][m + k];
							}
						}
						gue[l + 2][m + 2] = vert;
					}
				}
			}
			double likelihood[2] = {0};
			double maxp[2] = {0};
			for(int brute = 0; brute < 32; brute ++)
			{
				int bp = brute;
				bool dest = brute & 1;
				for(int l = 0; l < 2; l ++)
				{
					for(int m = -1; m < 2; m ++)
					{
						if(l + m >= 0)
						{
							gue[l + 2][m + 2] = bp & 1;
							bp >>= 1;
						}
					}
				}
				double lprob = 0;
				for(int l = 0; l < 3; l ++)
				{
					for(int m = 0; m < 3; m ++)
					{
						unsigned subcon = 0;
						unsigned grid = 0;
						for(int n = 0; n < 4; n ++)
						{
							for(int o = 0; o < 4; o ++)
							{
								subcon += subcon + local[l + n][m + o];
							}
						}
						for(int n = 0; n < 2; n ++)
						{
							for(int o = 0; o < 2; o ++)
							{
								grid += grid + gue[l + n][m + o];
							}
						}
						if(dict[subcon][grid]< 0)
						{
							lprob = -1;
							break;
						}
						else lprob += dict[subcon][grid];
					}
					if(lprob < 0)
						break;
				}
				double cp = exp(-lprob);
				/*cout << cp << " to " << dest << endl;
				for(int i = 0; i < 4; i ++)
				{
					for(int j = 0; j < 4; j ++)
					{
						cout << gue[i][j] << " ";
					}
					cout << endl;
				}*/
				if(lprob >=0)
				{
					if(cp >= maxp[dest])
						maxp[dest] = cp;
					likelihood[dest] += cp;
				}
			}
			bool maxlikely = false;
			double gap = maxp[0] / maxp[1];
			if(gap < 1)
			{
				gap = 1 / gap;
				maxlikely = true;
			}
			if(gap >= 3) tmp[j][k] = maxlikely;
			else tmp[j][k] = (likelihood[1] > likelihood[0] * BENCH);
			/*for(int l = 0; l < 6; l ++)
			{
				for(int m = 0; m < 6; m ++)
				{
					cout << local[l][m] << " ";
				}
				cout << endl;
			}
			cout << "likelihood : " << likelihood[0] << " vs " << likelihood[1] << endl;
			cout << "certainty : " << gap << " " << maxlikely << endl;
			cout << "j: " << j << "k: " << k << " b: " << tmp[j][k] << endl;
			system("pause");*/
		}
	}
	memcpy(fig, tmp, WID * WID * sizeof(bool));
}

int Config::check()
{
	int cnt = 0;
	int oones = 0;
	int fones = 0;
	for(int i = 0; i < WID; i ++)
	{
		for(int j = 0; j < WID; j ++)
		{
			oones += origin[i][j];
			fones += fig[i][j];
			cnt += (fig[i][j] != origin[i][j]);
		}
	}
	/*cout << "Errors : " << oones << " : " << fones << " : " << cnt << endl;
	for(int i = 0; i < WID; i ++)
	{
		for(int j = 0; j < WID; j ++)
		{
			cout << origin[i][j];
		}
		cout << " *** ";
		for(int j = 0; j < WID; j ++)
		{
			cout << fig[i][j];
		}
		cout << endl;
	}
	bool fevo[WID][WID];
	update(STEP);
	memcpy(fevo, fig, WID * WID * sizeof(bool));
	memcpy(fig, origin, WID * WID * sizeof(bool));
	update(STEP);
	oones = 0;
	fones = 0;
	cnt = 0;
	for(int i = 0; i < WID; i ++)
	{
		for(int j = 0; j < WID; j ++)
		{
			oones += fig[i][j];
			fones += fevo[i][j];
			cnt += fig[i][j] != fevo[i][j];
		}
	}
	cout << "Evolution Errors : " << oones << " : " << fones << " : " << cnt << endl;
	for(int i = 0; i < WID; i ++)
	{
		for(int j = 0; j < WID; j ++)
		{
			cout << fig[i][j];
		}
		cout << " *** ";
		for(int j = 0; j < WID; j ++)
		{
			cout << fevo[i][j];
		}
		cout << endl;
	}
	system("pause");*/
	return cnt;
}

Config::~Config(void)
{
}
