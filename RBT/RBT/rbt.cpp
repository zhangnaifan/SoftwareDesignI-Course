#include <iostream>
#include <vector>
#define RED 1
#define BLACK 0
using namespace std;

struct node {
	bool color = RED;
	int key;
	node *left = nullptr, *right = nullptr;
	node(int k) : key(k) {}
};

class RBT {
	node *root = nullptr;//根节点索引
private:
	void traverse(node*);//遍历
	bool isRED(node*);//判断点是否连着红边
	void rotate_left(node*&);//左旋平衡
	void rotate_right(node*&);//右旋平衡
	void put(int, node*&);//插入新节点

	void move_RED_left(node *&);//左移平衡操作
	void move_RED_right(node *&);//右移平衡操作
	void delete_min(node *&);//删除最小节点
	void balance(node* &);//平衡操作
	void remove(int, node*&);//删除操作
	node* minimum(node*);//返回最小键值索引

	int get(node*, int);//返回给定节点的键值
public :
	void put(int);//插入
	void traverse();//遍历
	void remove(int);//删除
	void delete_min();//删除键值最小元素
	void get(int);//查找
};

int RBT::get(node* nd, int key)
{
	if (nd)
	{
		if (nd->key == key)
			return nd->key;
		else if (nd->key < key)
			return get(nd->right, key);
		return get(nd->left, key);
	}
	return -1111;
}

void RBT::get(int key)
{
	int ret = get(root, key);
	if (ret == -1111)
	{
		cout <<"Key "<< key << " Not Found!\n";
	}
	else
		cout << "key " << key << " is " << ret << endl;
}

node* RBT::minimum(node* nd)
{
	for (; nd->left; nd = nd->left);
	return nd;
}

void RBT::move_RED_left(node* &nd)
{
	nd->color = BLACK, nd->left->color = nd->right->color = RED;
	if (isRED(nd->right->left))
	{
		rotate_right(nd->right);
		rotate_left(nd);
	}
}

void RBT::move_RED_right(node* &nd)
{
	nd->color = BLACK, nd->left->color = nd->right->color = RED;
	//if (!isRED(nd->left->left))
	//{
	//	rotate_right(nd);
	//}
}

void RBT::balance(node* &nd)
{
	if (isRED(nd->right))
		rotate_left(nd);
	if (!isRED(nd->left) && isRED(nd->right))
		rotate_left(nd);
	if (isRED(nd->left) && isRED(nd->left->left))
		rotate_right(nd);
	if (isRED(nd->left) && isRED(nd->right))
		nd->left->color = nd->right->color = BLACK, nd->color = RED;
}

void RBT::delete_min()
{
	/*if (!isRED(root->left) && !isRED(root->right))
		root->color = RED;*/
	delete_min(root);
	if (root)	root->color = BLACK;
}

void RBT::delete_min(node* &nd)
{
	if (!nd->left)//到了树底，直接删除
	{
		delete nd;
		nd = nullptr;
		return;
	}
	if (!isRED(nd->left) && !isRED(nd->left->left))//如果nd->left是2-结点，调整树使之成为3-节点
		move_RED_left(nd);
	delete_min(nd->left);
	balance(nd);
}

void RBT::remove(int key)
{
	/*if (!isRED(root->left) && !isRED(root->right))
		root->color = RED;*/
	remove(key, root);
	if (root)	root->color = BLACK;
}

void RBT::remove(int key, node* &nd)
{
	if (key < nd->key)
	{
		if (!isRED(nd->left) && !isRED(nd->left->left))
			move_RED_left(nd);
		remove(key, nd->left);
	}
	else
	{
		if (isRED(nd->left))
			rotate_right(nd);
		if (nd->key == key && !nd->right)
		{
			delete nd;
			nd = nullptr;
			return;
		}
		if (!isRED(nd->right) && !isRED(nd->right->left))
			move_RED_right(nd);
		if (nd->key == key)
		{
			nd->key = minimum(nd->right)->key;
			delete_min(nd->right);
		}
		else
			remove(key, nd->right);
	}
	balance(nd);
}

bool RBT::isRED(node *nd)
{
	if (!nd)	return false;
	return		nd->color == RED;
}

void RBT::rotate_left(node*& x)
{
	node* y = x->right;
	x->right = y->left;
	y->left = x;
	y->color = x->color;
	x->color = RED;
	x = y;
}

void RBT::rotate_right(node*&x)
{
	node* y = x->left;
	x->left = y->right;
	y->right = x;
	y->color = x->color;
	x->color = RED;
	x = y;
}

void RBT::put(int key)
{
	put(key, root);
	root->color = BLACK;
}

void RBT::put(int key, node* &nd)
{
	if (!nd)
		nd = new node(key);
	else if (nd->key > key)
		put(key, nd->left);
	else if (nd->key < key)
		put(key, nd->right);
	
	if (!isRED(nd->left) && isRED(nd->right))
		rotate_left(nd);
	if (isRED(nd->left) && isRED(nd->left->left))
		rotate_right(nd);
	if (isRED(nd->left) && isRED(nd->right))
		nd->left->color = nd->right->color = BLACK, nd->color = RED;
}

void RBT::traverse()
{
	traverse(root);
}

void RBT::traverse(node*nd)
{
	if (nd)
	{
		cout << nd->key << " " << nd->color << endl;
		traverse(nd->left);
		traverse(nd->right);
	}
}

int main()
{
	RBT my_rbt;
	vector<int> order{ 2,1,3,7,6,5,4 };
	for (auto i : order)
		my_rbt.put(i);
	my_rbt.traverse();
	cout << endl;
	my_rbt.remove(5);
	my_rbt.remove(4);
	my_rbt.traverse();

	my_rbt.get(3);
	my_rbt.get(4);
	return 0;
}