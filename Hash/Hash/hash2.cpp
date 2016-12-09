#include <ctime>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct node_2 {
	string key;
	int val;
	node_2(string k="",int v=-1) : key(k),val(v) {}
};

int hash_function(string s, int M,int R)
{
	int ret = 0;
	for (auto i : s)
	{
		ret = (ret*R + i) % M;
	}
	return ret;
}

int hash_function_1(vector<string> vec, int M, int R)
{
	int ret = 0;
	for (auto i : vec)
		ret = (ret*R + hash_function(i,M,R)) % M;
	return ret;
}

class hash_2 {
	int N;
	int M;
	vector<node_2> index;
public:
	hash_2(int m,int n) : M(m),N(n), index(n) {}
	void add(string, int);
	void remove(string);
	void show();
};

void hash_2::show()
{
	for (auto i : index)
	{
		if (!i.key.empty())
			cout << i.key << endl;
	}
}

void hash_2::add(string key, int val)
{
	int pos = hash_function(key, M, 2);
	int i;
	for (i = 1; i >= 0 && i < N && index[pos].key != ""; i *= -2);
	if (i < 0 || i >= N)
	{
		cout << "Add fail!\n";
		return;
	}
	if (i == pos)
		index[pos] = node_2(key, val);
	else
		index[pos + i] = node_2(key, val);
}

void hash_2::remove(string key)
{
	int pos = hash_function(key, M, 2);
	int i;
	for (i = 1; i >= 0 && i < N && index[pos].key != ""; i *= -2);
	if (i < 0 || i >= N)
	{
		cout << "Key doesn't exist!\n";
		return;
	}
	if (i == pos)
		index[pos] = node_2();
	else
		index[pos + i] = node_2();
}

int main()
{
	srand(time(0));
	hash_2 ha(13, 40);
	for (int i = 0; i < 10; ++i)
		ha.add(to_string(rand() % 50),0);
	ha.show();
	cout << "--------------\n";
	for (int i = 0; i < 5; ++i)
		ha.remove(to_string(i));
	ha.show();
	return 0;
}