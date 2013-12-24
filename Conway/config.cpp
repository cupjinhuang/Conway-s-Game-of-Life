#include "stdafx.h"
#include "config.h"
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

Node::Node(Node* parent, bool lr): p(parent), l(NULL), r(NULL), id(parent ? parent->id * 2 + lr : 1), isLeaf(true), occ(0), app(0), prob(-1)
{
	count ++;
}

void Node::update()
{
	if(l)
	{
		l->update();
		r->update();
	}
	else
	{
		double prob = 0;
		if(occ) prob = (app + 0.0) / occ;
		if(prob > THRE && prob < 1- THRE)
		{
			if(occ > 20)
			{
				l = new Node(this, 0);
				r = new Node(this, 1);
				isLeaf = false;
			}
		}
	}
	occ = 0;
	app = 0;
}

Node* Config::head = new Node(NULL, 1);
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
	Node* iter = head;
	unsigned long cfg = 1;
	int depth = 0;
	while(!iter->isLeaf)
	{
		if(get(i + dic[depth][0], j + dic[depth][1])) iter = iter->r;
		else iter = iter->l;
		depth ++;
	}
	iter->occ ++;
	iter->app += origin[i][j];
}

void Config::record(string fileName)
{
	unsigned long error = 0;
	unsigned long total = 0;
	unsigned long bad = 0;
	ofstream out(fileName.c_str());
	traverse(head, out, error, total, bad);
	cout << "Bad : Error : Total = " << bad << " : " << error << " : " << total;
	out.close();
}

void Config::traverse(Node* np, ofstream& out, unsigned long& error, unsigned long& total, unsigned long& bad)
{
	if(np->isLeaf)
	{
		double prob = 0;
		if(np->occ >= 5) prob = (np->app + 0.0) / np->occ;
		bool flag = false;
		if(prob > THRE && prob <= 1 - THRE && np->occ > 100)
		{
			flag = true;
		}
		total += np->occ;
		out << np->id << ", " << prob << ", ";
		if(prob <= DIV)
		{
			error += np->occ - np->app;
			if(flag) bad += np-> occ - np-> app;
		}
		else
		{
			error += np->app;
			if(flag) bad += np->app;
		}
	}
	else
	{
		out << np-> id << ", " << -1 << ", ";
		traverse(np->l, out, error, total, bad);
		traverse(np->r, out, error, total, bad);
	}
}

void Config::generate(string file)
{
	ifstream in(file.c_str());
	while(!in.eof())
	{
		unsigned long long id;
		double prob;
		char tmp;
		in >> id;
		in >> tmp;
		in >> prob;
		in >> tmp;
		Node* n = head;
		int i = 0;
		unsigned long long idl = id;
		while(1)
		{
			idl >>= 1;
			if(idl == 0) break;
			i ++;
		}
		while(1)
		{
			i --;
			if(n->id == id)
			{
				if(prob != -1)
				{
					n->prob = prob;
				}
				else
				{
					n->l = new Node(n, 0);
					n->r = new Node(n, 1);
					n->isLeaf = false;
				}
				break;
			}
			else
			{
				if(n->isLeaf)
				{
					cout << "Error: " << id << " " << n->id << endl;
					system("pause");
				}
				unsigned long long ids = id >> i;
				if(ids >= 2 * n-> id + 1)
				{
					n = n->r;
				}
				else 
				{
					n = n->l;
				}
			}
		}
	}
	in.close();
	/**/
}

void Config::lookup()
{
	bool tmp[WID][WID];
	for(int i = 0; i < WID; i ++)
	{
		for(int j = 0; j < WID; j ++)
		{
			Node* n = head;
			int k = 0;
			while(1)
			{
				if(n->isLeaf)
				{
					tmp[i][j] = n->prob >= DIV;
					break;
				}
				else
				{
					if(get(i + dic[k][0], j + dic[k][1]))
					{
						n = n->r;
					}
					else n = n->l;
					k ++;
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
	system("pause");
	/**/
	return cnt;
}

Config::~Config(void)
{
}
