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

int Config::largo[1048576][16][5] = {0};
double Config::dict[1048576][16][4] = {0};
double Config::dict1[65536][16] = {0};
int Config::largo1[65536][17] = {0};
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
		for(int l = -2; l < 3; l ++)
		{
			subcon += subcon + get(i + k, j + l);
		}
	}
	int grid = 0;
	for(int k = 0; k < 2; k ++)
	{
		for(int l = -1; l < 1; l ++)
		{
			if(i + k >= 0 && i + k < 20 && j + l >= 0 && j + l < 20)
			grid += grid + origin[i + k][j + l];
		}
	}
	int pred = 0;
	for(int k = 0; k < 2; k ++)
	{
		if(i + k >= 0 && i + k < 20 && j + 1 >= 0 && j + 1 < 20)
			pred += pred + origin[i + k][j + 1];
	}
	largo[subcon][grid][0] ++;
	largo[subcon][grid][pred + 1]++;

	subcon = 0;
	for(int k = -1; k < 3; k ++)
	{
		for(int l = -1; l < 3; l ++)
		{
			subcon += subcon + get(i + k, j + l);
		}
	}
	grid = 0;
	for(int k = 0; k < 2; k ++)
	{
		for(int l = 0; l < 2; l ++)
		{
			if(i + k >= 0 && i + k < 20 && j + l >= 0 && j + l < 20)
			grid += grid + origin[i + k][j + l];
		}
	}
	largo1[subcon][0]++;
	largo1[subcon][grid + 1]++;
}

void Config::record(string fileName)
{
	ofstream out(fileName.c_str());
	out << " ";
	for(int i = 0; i < 1048576; i ++)
	{
		for(int j = 0; j < 16; j ++)
		{
			if(largo[i][j][0] < 5)
			{
				for(int k = 0; k < 4; k ++)
				{
					out << -(int)(k != 0) << " ";
				}
			}
			else
			{
				for(int k = 0; k < 4; k ++)
				{
					if(largo[i][j][k + 1] == 0) out << -1 << " ";
					else out << -log((largo[i][j][k + 1] + 0.0) / largo[i][j][0]) << " ";
				}
			}
		}
	}
	for(int i = 0; i < 65536; i ++)
	{
		if(largo1[i][0] == 0)
		{
			for(int k = 0; k < 16; k ++)
			{
				out << -(int)(k != 0) << " ";
			}
		}
		else
		{
			for(int k = 0; k < 16; k ++)
			{
				if(largo1[i][k+1] == 0) out << -1 << " ";
				else out << -log((largo1[i][k + 1] + 0.0) / largo1[i][0]) << " ";
			}
		}
	}
	out.close();
}

void Config::generate(string file)
{
	ifstream in(file.c_str());
	int i = 0;
	while(i < 1048576)
	{
		for(int j = 0; j < 16; j ++)
		{
			for(int k = 0; k < 4; k ++)
			{
				in >> dict[i][j][k];
			}
		}
		i ++;
	}
	i = 0;
	while(i < 65536)
	{
		for(int j = 0; j < 16; j ++)
		{
			in >> dict1[i][j];
		}
		i ++;
	}
	cout << "Data Read:" << i << endl;
	in.close();
}

void Config::lookup()
{
	bool tmp[WID][WID] = {0};
	for(int i = 0; i < WID; i = i + 2)
	{
		for(int j = 0; j < WID; j = j + 2)
		{
			double prob[16];
			for(int k = 0; k < 16; k ++)
			{
				prob[k] = -1;
			}

			int hsubcon = 0, hgrid = 0;
			for(int k = -1; k < 3; k ++)
			{
				for(int l = -3; l < 2; l ++)
				{
					hsubcon += hsubcon + get(i + k, j + l);
				}
			}
			//cout << "\nHsubcon: " << hsubcon << " ";

			for(int k = 0; k < 2; k++)
			{
				for(int l = -2; l < 0; l ++)
				{
					if(k + i < WID && j + l >= 0)
						hgrid += hgrid + tmp[i + k][j + l];
					else hgrid *= 2;
				}
			}
			//cout << "Hgrid: " << hgrid << " ";

			int vsubcon = 0, vgrid = 0;
			for(int k = -1; k < 3; k ++)
			{
				for(int l = -3; l < 2; l ++)
				{
					vsubcon += vsubcon + get(i + l, j + k);
				}
			}			
			//cout << "\nVsubcon: " << vsubcon << " ";

			for(int k = 0; k < 2; k++)
			{
				for(int l = -2; l < 0; l ++)
				{
					if(k + j < WID && i + l >= 0)
						vgrid += vgrid + tmp[i + l][j + k];
					else vgrid *= 2;
				}
			}		
			//cout << "Vgrid: " << vgrid << " ";

			int fillIn = 0;
			double tprob = -1;
			for(int a1 = 0; a1 < 2; a1 ++)
			{
				for(int a2 = 0; a2 < 2; a2++)
				{
					if(dict[vsubcon][vgrid][2 * a1 + a2] == -1) continue;
					double vprob1 = dict[vsubcon][vgrid][2 * a1 + a2];
					for(int a3 = 0; a3 < 2; a3 ++)
					{
						if(dict[hsubcon][hgrid][2 * a1 + a3] == -1) continue;
						double hprob1 = dict[hsubcon][hgrid][2 * a1 + a3];
						if(vprob1 + hprob1 >= 10) //very unlikely
							continue;
						for(int a4 = 0; a4 < 2; a4 ++)
						{
							tmp[i][j] = a1;
							tmp[i][j + 1] = a2;
							tmp[i+1][j] = a3;
							tmp[i+1][j+1] = a4;
							int index = 8 * a1 + 4 * a2 + 2 * a3 + a4;
							prob[index] = vprob1 + hprob1;
							int subcon = 0;							
							for(int k = -1; k < 3; k ++)
							{
								for(int l = -1; l < 3; l ++)
								{
									subcon += subcon + get(i + k, j + l);
								}
							}
							if(dict1[subcon][index] == -1) continue;
							prob[index] += 3 * dict1[subcon][index];
							int hsubcon1 = 0, hgrid1 = 0;
							for(int k = -1; k < 3; k ++)
							{
								for(int l = -2; l < 3; l ++)
								{
									hsubcon1 += hsubcon1 + get(i + k, j + l);
								}
							}
							for(int k = 0; k < 2; k++)
							{
								for(int l = -1; l < 1; l ++)
								{
									if(k + i < WID && j+l >=0)
										hgrid1 += hgrid1 + tmp[i + k][j + l];
									else hgrid1 *= 2;
								}
							}
							//cout << "\n" << index << " Hsubcon1: " << hsubcon1 << " Hgrid1: " << hgrid1 << " ";
							int vsubcon1 = 0, vgrid1 = 0;
							for(int k = -1; k < 3; k ++)
							{
								for(int l = -2; l < 3; l ++)
								{
									vsubcon1 += vsubcon1 + get(i + l, j + k);
								}
							}
							for(int k = 0; k < 2; k++)
							{
								for(int l = -1; l < 1; l ++)
								{
									if(k + j < WID && i + l >= 0)
										vgrid1 += vgrid1 + tmp[i + l][k + j];
									else vgrid1 *= 2;
								}
							}
							
							//cout << "Vsubcon1: " << vsubcon1 << " Vgrid1: " << vgrid1 << " ";
							if(dict[hsubcon1][hgrid1][2 * a2 + a4] == -1) continue;
							if(dict[vsubcon1][vgrid1][2 * a3 + a4] == -1) continue;
							prob[index] += dict[hsubcon1][hgrid1][2 * a2 + a4];
							prob[index] += dict[vsubcon1][vgrid1][2 * a3 + a4];
							if(index == 0 || prob[index] < tprob)
							{
								fillIn = index;
								tprob = prob[index];
							}
							//cout << " prob: " << prob[index];
						}
					}
				}
			}
			
			//system("pause");
			for(int k = 1; k > -1; k --)
			{
				for(int l = 1; l > -1; l --)
				{
					tmp[i + k][j + l] = fillIn % 2;
					fillIn /= 2;
				}
			}
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
