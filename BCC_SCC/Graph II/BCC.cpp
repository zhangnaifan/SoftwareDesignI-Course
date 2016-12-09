#include <iostream>
#include <vector>
#include <stack>
using namespace std;

struct edge {
	int from, to;
	edge(int u, int v) : from(u), to(v) {}
	bool operator != (edge right)
	{
		if (from == right.from && to == right.to || from == right.to && to == right.from)
			return false;
		return true;
	}
	friend ostream& operator << (ostream &os, edge const& e)
	{
		os << e.from << "--" << e.to << " ";
		return os;
	}
};

class Undirected {
	int V;//������
	vector<vector<int>> adj;//�ڽ�����
private:
	int count = 0;//���Լ���������
	stack<edge> s;//���Դ�ű�
	vector<bool> visited;//��Ƕ����Ƿ���ʹ�
	vector<int> dfn, low;//�����ţ���������ĳ�ڵ�������������ӵĽڵ�����С�������
	vector<vector<edge>> _bcc;//˫��ͨ����
	void dfs(int,int);//��������
public:
	Undirected(vector<vector<int>> &init) : V(init.size()),adj(init),visited(V,false),dfn(V),low(V) {}
	vector<vector<edge>> bcc();//����˫��ͨ����
};

void Undirected::dfs(int root, int parent)
{
	int children = 0;
	visited[root] = true;
	dfn[root] = low[root] = ++count;
	for (auto i : adj[root])
	{
		if (!visited[i])//(root, i)��DFS������������
		{
			++children;
			s.push(edge(root, i));

			dfs(i, root);//DFS

			if (low[i] < low[root])//�������б�ͨ��root�����ȣ�root�����νӵ㣬����low[root]
				low[root] = low[i];

			else if (parent == -1 && children > 1 ||
				parent != -1 && low[i] >= dfn[root])//root���νӵ㣬����Ϊ����ȷ��һ��BCC�����ű�
			{
				edge first_edge(root, i);
				if (s.top() != first_edge)//���root���������űߣ��򽫸�BCC����_bcc
				{
					vector<edge> tmp;
					for (; s.top() != first_edge; s.pop())
					{
						tmp.push_back(s.top());
					}
					s.pop();
					tmp.push_back(first_edge);
					_bcc.push_back(tmp);
				}
				else//ֱ�������ű�
					s.pop();
			}
		}
		else if (i != parent && dfn[i] < dfn[root])//(root, i)��DFS�������Ļ��˱�
		{
			if (dfn[i] < low[root])//����low[root]
				low[root] = dfn[i];
			s.push(edge(root,i));
		}
	}
}

vector<vector<edge>> Undirected::bcc()
{
	while (count < V)
	{
		int root = 0;
		for (; visited[root]; ++root);
		dfs(root, -1);
		//���DFS���ڵ㲻���νӵ㣬��s���в����Ԫ��
		if (!s.empty())
		{
			vector<edge> tmp;
			for (; !s.empty(); s.pop())
				tmp.push_back(s.top());
			_bcc.push_back(tmp);
		}
	}
	return _bcc;
}

//int main()
//{
//	vector<vector<int>> init({ { 1,2,3 },{ 0,3,2 },{ 0,1,3 },{ 0,1,2,4 },
//	{ 3,7,9,10,6,5 },{ 4,6 },{ 5,4 },{ 8,9,4 },{ 7 },{ 4,7 },
//	{ 4,11,12 },{ 10,13 },{ 10,13 },{ 11,12,14 },{13,15,20},{14,16,19,20,22},
//	{18,17,15},{18,16},{17,16},{15},{22,14,15,21},{22,20},{15,20,21} });
//	Undirected my_graph(init);
//	vector<vector<edge>> bcc = my_graph.bcc();
//	for (auto i : bcc)
//	{
//		for (auto j : i)
//			cout << j;
//		cout << endl;
//	}
//	return 0;
//}