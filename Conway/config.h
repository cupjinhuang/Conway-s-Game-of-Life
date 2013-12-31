#pragma once
#define WID 20
#define THRE 0.08
#define DIV 0.5
#define STEP 1
#define SAMPLE 1
#define TINYOCC 1e-5
#define DEPTH 50
#define BENCH 0.142

using std::string;
using std::ofstream;
using std::ifstream;

struct Node
{
	Node* l;
	Node* r;
	Node* p;
	const unsigned long long id;
	unsigned long occ;
	unsigned long app[16];
	double prob[16];
	bool isLeaf;
	static unsigned long count;

	Node(Node*, bool);

	void insert(unsigned long long, int);

	~Node(){}
};

class Config
{
	bool origin[WID][WID];
	bool fig[WID][WID];
public:
	static double dict[65536][16];
	static unsigned long long largo[65536][17];
	static unsigned long coverage;
	static unsigned long count;
	/* static functions for training */
	static void record(string);

	/*static functions for testing */
	static void generate(string);

	/* basics */
	Config(void);
	void update(int);
	inline bool get(int i, int j)
	{
		if(i >= 0 && i < WID)
		{
			if(j >= 0 && j < WID)
			{
				return fig[i][j];
			}
		}
		return false;
	}

	/* functions for training */
	void save();
	void savecfg(int, int);
	
	/* functions for testing */
	void lookup();
	int check();

	~Config(void);
};

