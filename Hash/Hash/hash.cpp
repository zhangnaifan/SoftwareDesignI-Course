#include <ctime>
#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;

struct node {
	int key;
	int val;
	node* next;
	node (int k,int v,node* ne=nullptr) : key(k), val(v), next(ne) {}
};

class my_hash {
	int M;//������
	vector<node*> index;//�ڽ�����
	vector<int> total;//index[i]�����ɵ�Ԫ�ظ���
private:
	void insert(int,int, int);
	void remove(int, int);
public:
	my_hash(int sz) : total(sz,0),index(sz,nullptr), M(sz) {}
	void add(int, int);//���Ԫ��
	void remove(int);//ɾ��Ԫ��
	void show();//��ӡ��ϣ��
};

void my_hash::show()
{
	for (auto i : total)
		cout << i << endl;
}

void my_hash::insert(int i, int key, int val)
{
	index[i] = new node(key, val, index[i]);
	++total[i];
}

void my_hash::add(int key, int val)
{
	int t = key % M;
	insert(t, key, val);
}

void my_hash::remove(int i,int key)
{
	node *nd, *pre=nullptr;
	for (nd = index[i]; nd && nd->key != key; nd = nd->next)
	{
		pre = nd;
	}
	if (nd && nd->key == key)
	{
		--total[i];
		if (pre)
		{
			pre->next = nd->next;
			delete nd;
		}
		else
		{
			index[i] = nd->next;
			delete nd;
		}
	}
}

void my_hash::remove(int key)
{
	int t = key % M;
	remove(t, key);
}

//int main()
//{
//	srand(time(0));
//	my_hash mh(77);
//	for (int i = 0; i < 1000; ++i)
//		mh.add(rand() % 1000, 0);
//	mh.show();
//
//	cout << "----------------\n";
//
//	for (int i = 0; i < 100; ++i)
//		mh.remove(i);
//	mh.show();
//
//	return 0;
//}
