#include <stack>
#include <vector>
#include <iostream>
using namespace std;
using vec = vector<int>::iterator;

class node {
	friend class thtree;
	int key;
	bool ltag, rtag;
	node *left, *right;
	node(int key, node* left=nullptr, node* right=nullptr) : 
		key(key),left(left),right(right) {}
};

class thtree {
	node *header = new node(-1);
	void visit(node*&, node*&, node*&);
	void build(node*&,vec, vec, vec, vec);
	void do_traverse(node*, ostream&)const;
public:
	thtree(vector<int>&, vector<int>&);
	ostream&pre_traverse(ostream&)const;
	void pre_thread();
	void post_thread();
};

void thtree::build(node*&curr, vec p_begin, vec p_end, vec m_begin, vec m_end)
{
	curr = new node(*p_begin);
	vec mid = find(m_begin, m_end, *p_begin) + 1;
	vec last = p_begin + (mid - m_begin);
	if (++p_begin != last)
		build(curr->left, p_begin, last, m_begin, mid);
	if (last != p_end)
		build(curr->right, last, p_end, mid, m_end);
}

thtree::thtree(vector<int>&pre, vector<int>&mid)
{
	header->ltag = header->rtag = false;
	vec p_begin = pre.begin(), p_end = pre.end(), m_begin = mid.begin(), m_end = mid.end();
	if (pre.size() != mid.size() || p_begin == p_end)
		return;
	header->ltag = true;
	build(header->left, p_begin, p_end, m_begin, m_end);
}

void thtree::do_traverse(node*curr, ostream& os)const
{
	while (curr != header)
	{
		os << curr->key << " ";
		while (curr->ltag)
		{
			curr = curr->left;
			os << curr->key << " ";
		}
		curr = curr->right;
	}
}

ostream& thtree::pre_traverse(ostream& os)const
{
	if (!header->left)
		os << "No data in thtree!\n";
	else
		do_traverse(header->left, os);
	return os;
}

void thtree::pre_thread()
{
	if (!header->left)
	{
		cout << "No data!\n";
		return;
	}
	node *pre = header, *cur = header->left;
	pre->ltag = true;
	stack<node*>s; 
	while (!s.empty() || cur)
	{
		for (; cur->left; cur->ltag = true, pre = cur, cur = cur->left)
			if (cur->right)
				s.push(cur->right);
			else
			{
				cur->rtag = false;
				cur->right = cur->left;
			}
		cur->ltag = false;
		cur->left = pre;
		if (cur->right)
		{
			cur->rtag = true;
			pre = cur;
			cur = cur->right;
		}
		else if (!s.empty())
		{
			cur->rtag = false;
			cur->right = s.top();
			pre = cur;
			cur = s.top();
			s.pop();
		}
		else
		{
			cur->rtag = false;
			cur->right = header;
			cur = nullptr;
		}
	}
}

void thtree::visit(node*& pre, node*&now, node*&cur)
{
	cout << cur->key << " ";
	pre = now;
	now = cur;
	if (!pre->right)
	{
		pre->rtag = false;
		pre->right = now;
	}
	if (!now->left)
	{
		now->ltag = false;
		now->left = pre;
	}
}

void thtree::post_thread()
{
	node *pre = header, *now = header, *cur = header->left;
	stack<node*>s;
	while (cur || !s.empty())
	{
		for (; cur->left; cur = cur->left)
		{
			cur->ltag = true; 
			s.push(cur);
		}
		if (cur->right)
		{
			cur->rtag = true;
			s.push(cur);
			cur = cur->right;
		}
		else
		{
			visit(pre, now, cur);
			while (!s.empty() && (!s.top()->right || s.top()->right == now))
			{
				cur = s.top(); s.pop();
				visit(pre, now, cur);
			}
			if (s.empty())
				cur = nullptr;//end
			else
			{
				s.top()->rtag = true;
				cur = s.top()->right;
			}
		}
	}
}

int main()
{
	vector<int> p{1,2,4,5,6,7,3,8,9},
	m{ 4,5,2,7,6,1,3,8,9 };
	thtree t(p, m);
	t.pre_thread();
	t.pre_traverse(cout) << endl;
	return 0;
}