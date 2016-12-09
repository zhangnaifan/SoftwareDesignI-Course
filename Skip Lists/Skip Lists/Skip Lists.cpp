#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <stack>

using namespace std;

using value_t = int;

class node {
	//define nodes that build our skip lists
	friend class skipLists;
	int key;
	value_t val;
	vector<node*> next;
public:
	node() = default;
	node(int k, value_t v) : key(k), val(v) {}
	node(int k, value_t v, int num, node* prev_header) : key(k), val(v), next(num,prev_header) {}
};

class skipLists {
	node *header = NULL;
	double prob = 0.5;
	int maxLevel = 16;
	int currentLevel = -1;
public:
	skipLists() = default;
	skipLists(double p, int n = 65536) : prob(p), maxLevel(int(log(n) / log(1 / p))) {}

	bool found(int) const;//return true if the key is found in our skip lists

	ostream& display(ostream&) const;//to display all layers of our skip lists

	value_t get_value(int) const;//return the value of a given key, -1 if the key is not found

	int get_level() const;//used to determine the number of layers of a new node

	void get_pre(int,vector<node*>&)const;//to providea given node's roote upside down

	void insert(int, value_t);//insert a new node with a key and a value

	void remove(int);//remove a node from our existed skip lists
};

bool skipLists::found(int key)const
{
	node* pn = header;
	for (int i = currentLevel; i >= 0; --i)
	{
		while (pn->next[i] && pn->next[i]->key <= key)
			pn = pn->next[i];
		if (pn->key == key)
			return true;
	}
	return false;
}

ostream& skipLists::display(ostream& os) const
{
	if (header)
		for (int i = currentLevel; i >= 0; --i)
		{
			for (node* pn = header; pn; pn = pn->next[i])
				os << pn->key << " ";
			os << endl;
		}
	else
		os << "NO DATA!!!";
	return os;
}

value_t skipLists::get_value(int key) const
{
	node* pn = header;
	if (header)
	{
		for (int i = currentLevel; i >= 0; --i)
			while (pn && pn->key <= key)
				pn = pn->next[i];
		if (pn->key == key)
			return pn->val;
	}
	return -1;
}

int skipLists::get_level() const 
{
	int level = 0;
	while (level < maxLevel && rand() / (double)RAND_MAX > prob)
		++level;
	return level;
}

void skipLists::get_pre(int key, vector<node*>& pre)const
{
	stack<node*> tmp;
	node* pn = header;
	for (int i = currentLevel; i >= 0; --i)
	{
		while (pn->next[i] && pn->next[i]->key < key)
			pn = pn->next[i];
		tmp.push(pn);
	}
	for (; !tmp.empty(); tmp.pop())
		pre.push_back(tmp.top());
}

void skipLists::insert(int key, value_t val)
{
	if (!header)
	{
		currentLevel = get_level();
		header = new node(key, val, currentLevel + 1, NULL);
	}
	else if (found(key))
	{
		cout << "The key " << key << " has existed!\nInsertion failed.\n";
		return;
	}
	else if (key < header->key)//we will create a new header
	{
		int level = get_level();
		level = level < currentLevel ? currentLevel : level;
		header = new node(key, val, currentLevel + 1, header);
		if (level > currentLevel)
		{
			for (int i = currentLevel+1; i <= level; ++i)
				header->next.push_back(NULL);
			currentLevel = level;
		}
	}
	else//we need to get the pre-nodes
	{
		int level = get_level();
		vector<node*> pre;
		get_pre(key, pre);
		node* nnode = new node(key, val);
		if (level <= currentLevel)//current level is OK
			for (int i = 0; i <= level; ++i)
			{
				nnode->next.push_back(pre[i]->next[i]);
				pre[i]->next[i] = nnode;
			}
		else//more levels are required to build
		{
			for (int i = 0; i <= currentLevel; ++i)
			{
				nnode->next.push_back(pre[i]->next[i]);
				pre[i]->next[i] = nnode;
			}
			for (int i = currentLevel + 1; i <= level; ++i)
			{
				header->next.push_back(nnode);
				nnode->next.push_back(NULL);
			}
			currentLevel = level;
		}
	}
}

void skipLists::remove(int key)
{
	if (!found(key))
		cout << "The key " << key << " is not found in skip lists!\nRemove failed.\n";
	else if (header->key == key)//remove the header
	{
		if (!header->next[0])//only header exists
		{
			delete header;
			header = NULL;
			currentLevel = -1;
		}
		else //our new header will be header->next[0]
			//in fact, we delete header->next[0], and simply change its key and value
		{
			node* nheader = header->next[0];
			for (; currentLevel >= 0 && !header->next[currentLevel]; --currentLevel, header->next.pop_back());
			for (int i = currentLevel; i >= 0; --i)
				if (header->next[i]->key == nheader->key)
					header->next[i] = nheader->next[i];
			header->key = nheader->key;
			delete nheader;
		}
	}
	else
	{
		vector<node*> pre;
		get_pre(key, pre);
		node* removed = pre[0]->next[0];
		for (int i = 0, n=removed->next.size()-1; i <= n; ++i)
			pre[i]->next[i] = pre[i]->next[i]->next[i];
		delete removed;
	}
}

void test()
{
	srand(unsigned(time(NULL)));
	skipLists sl;
	for (int i = 0; i < 20; ++i)
		sl.insert(i, 20);
	for (int i = 3; i < 20; i += 3)
		sl.remove(i);
	sl.display(cout) << endl;
}

int main()
{
	test();
	return 0;
}