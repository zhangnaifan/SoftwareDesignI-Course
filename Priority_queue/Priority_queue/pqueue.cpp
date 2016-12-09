#include <iostream>
#include <vector>

using namespace std;

class node {
	friend class pqueue;
	int key;
	int val;
public:
	node() = default;
	node(int k, int v) : key(k), val(v) {}
};

class pqueue {
	vector<node> body{ node(-1,-1) };//the vector that store all the nodes
private:
	void swap(int, int);//used to swap two nodes in the body
public:
	bool empty() const{ return body.size() == 1; }//return true if our pqueue is empty, false otherwise
	void insert(node);//to insert a node into our pqueue
	int get_max()const;//return the highest priority item
	int pop_max();//to delete the highest priority item
	ostream& display(ostream&)const;//to show our pqueue
};

void pqueue::swap(int p1, int p2)
{
	node tmp = body[p1];
	body[p1] = body[p2];
	body[p2] = tmp;
}

void pqueue::insert(node nnode)
{
	body.push_back(nnode);
	for (int i = body.size() - 1; i > 0 && body[i] > body[i / 2]; i /= 2)
		swap(i, i / 2);
}

int pqueue::get_max()const
{
	if (body.size() == 1)
		return -1;
	return body[1].val;
}

int pqueue::pop_max()
{
	if (body.size() == 1)
		return -1;
	int ret = body[1].val;
	body[1] = *body.rbegin();
	body.pop_back();
	for (int i = 1; i < body.size();) {
		int next = i;
		if (i * 2 < body.size() && body[i * 2].key > body[i].key) 
			swap(i, i * 2), next = i * 2;
		if (i * 2 + 1 < body.size() && body[i * 2 + 1].key > body[i].key)
			swap(i, i * 2 + 1), next = i * 2 + 1;
		if (next == i)break;
		i = next;
	}
	return ret;
}

ostream& pqueue::display(ostream& os)const
{
	for (int i = 1, n = body.size() - 1; i <= n;++i)
		os << "(" << body[i].key << " " << body[i].val << ") ";
	return os;
}

void test()
{
	pqueue pq;
	for (int i = 10; i >= 1; --i)
		pq.insert(node(i, i*i));
	pq.display(cout) << endl;
	for (int i = 1; i <= 5;++i)
		cout<<pq.pop_max()<<" ";
	cout << endl;
	pq.display(cout) << endl;
}

int main()
{
	test();
	return 0;
}