#include <iostream>

using namespace std;

using key_t = int;

class node {
	friend class tri_matrix;
	friend class row_head;
	int i, j;
	key_t v;
	node *pre, *next;
public:
	node() = default;
	node(int x, int y, key_t val, node* pr = nullptr, node* ne = nullptr) : i(x), j(y), v(val), pre(pr), next(ne) {}
	void insert(int x, int y, key_t val)
	{ 
		next = new node(x, y, val, this, next);
		if (next->next)
			next->next->pre = next;
	}
	void remove()
	{
		if (next)
		{
			node* tmp = next;
			next = next->next;
			if (next)
				next->pre = this;
			delete tmp;
		}
	}
};

class row_head {
	friend class tri_matrix;
	int i = -1;
	node* head;
	row_head* next_row;
public:
	row_head(node* nhead = nullptr, row_head* next = nullptr) : head(nhead), next_row(next) { if (nhead) i = nhead->i; }
	void insert(node* nhead) { next_row = new row_head(nhead, next_row); }
	void remove() 
	{
		if (next_row)
		{
			row_head* tmp = next_row;
			next_row = next_row->next_row;
			delete tmp;
		}
	}
};

class tri_matrix {
	node *head = nullptr;
	row_head *row = nullptr;
public:
	node* found(int, int)const;
	void insert(int, int, key_t);
	void remove(int, int);
	key_t get_val(int, int)const;
	ostream& display(ostream&)const;
	void reverse();
};

key_t tri_matrix::get_val(int x, int y) const
{
	node* nd = found(x, y);
	if (!nd)
		return -1;
	return nd->v;
}

node* tri_matrix::found(int x, int y)const
{
	node* nd;
	row_head* rh = row;
	for (; rh && rh->i < x; rh = rh->next_row);//search by row
	if (!rh || rh->i != x)return nullptr;
	for (nd = rh->head; nd && nd->j < y; nd = nd->next);//search by column
	if (!nd || nd->i != x || nd->j != y)
		return nullptr;
	else
		return nd;
}

void tri_matrix::insert(int x, int y, key_t val)
{
	if (!row || row->i > x)
	{
		head = new node(x, y, val,nullptr,head);
		row = new row_head(head,row);
		return;
	}
	row_head* r = row;
	for (; r->next_row && r->next_row->i <= x; r = r->next_row);
	if (r->i == x)
	{
		if (r->head->j > y)
		{
			node* nnode =	new node(x, y, val, r->head->pre, r->head);
			if (r->head->pre)
			{
				r->head->pre->next = nnode;
				r->head->pre = r->head->pre->next;
				r->head = nnode;
				r->i = nnode->i;
			}
			else
			{
				head = nnode;
				r->head = head;
				r->i = head->i;
			}
		}
		else
		{
			node* nd = r->head;
			for (; nd->next && nd->next->i == r->i && nd->next->j <= y; nd = nd->next);
			if (nd->j == y)
				nd->v = val;
			else
				nd->insert(x, y, val);
		}
	}
	else
	{
		node* rend = r->head;
		for (; rend->next && rend->next->i == r->i; rend = rend->next);
		rend->insert(x, y, val);
		r->insert(rend->next);
	}
}

ostream& tri_matrix::display(ostream& os)const
{
	os << "Shown as a list: ";
	for (node* nd = head; nd; nd = nd->next)
		os << "(" << nd->i << "," << nd->j << "," << nd->v << ") ";
	os << endl << "Shown as layers: \n";
	for (row_head* r = row; r; r = r->next_row,cout<<endl)
		for (node* nd = r->head; nd && nd->i == r->head->i;nd=nd->next)
			os << "(" << nd->i << "," << nd->j << "," << nd->v << ") ";
	return os;
}

void tri_matrix::remove(int x, int y)
{
	node* nd = found(x, y);
	if (!nd)
	{
		cout << "NOT FOUND!\n";
		return;
	}
	row_head* r = row,*pre;
	for (; r && r->i < x; r = r->next_row)
		if (r->i != x)
			pre = r;
	node* tmp = nd;
	row_head* t = r;
	if (nd == r->head && (!nd->next || nd->next->i != r->i))
	{
		if (nd == head)
		{
			head = head->next;
			if (head)
				head->pre = nullptr;
			delete tmp;
			row = row->next_row;
			delete t;
		}
		else
		{
			pre->remove();
			nd->pre->remove();
		}
	}
	else
	{
		if (nd == head)
		{
			head = head->next;
			head->pre = nullptr;
			delete tmp;
			r->i = head->i;
			r->head = head;
		}
		else
		{
			if (nd == r->head)
			{
				r->head = nd->next;
				r->i = nd->next->i;
			}
			nd->pre->remove();
		}
	}
}

void tri_matrix::reverse()
{
	tri_matrix* ntri = new tri_matrix;
	while (head)
	{
		ntri->insert(head->j, head->i, head->v);
		remove(head->i, head->j);
	}
	for (node* nd = ntri->head; nd; nd = nd->next)
	{
		insert(nd->i, nd->j, nd->v);
	}
}

void test()
{
	tri_matrix m;
	for (int k = 0; k < 5;++k)
	{
		cout << "Input(i,j,i+j):";
		int i, j; cin >> i >> j ;
		m.insert(i, j, i+j);
		m.display(cout) << endl;
		cout << "Delete(i,j):";
		cin >> i >> j;
		m.remove(i, j);
		m.display(cout) << endl;
		m.reverse();
		m.display(cout) << endl;
	}
}

int main()
{
	test();
	return 0;
}