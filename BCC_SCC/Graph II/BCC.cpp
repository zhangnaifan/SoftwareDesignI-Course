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
	int V;//顶点数
	vector<vector<int>> adj;//邻接链表
private:
	int count = 0;//用以计算先深编号
	stack<edge> s;//用以存放边
	vector<bool> visited;//标记顶点是否访问过
	vector<int> dfn, low;//先深编号，先深树中某节点的子孙所能连接的节点中最小先深序号
	vector<vector<edge>> _bcc;//双连通分量
	void dfs(int,int);//先深搜索
public:
	Undirected(vector<vector<int>> &init) : V(init.size()),adj(init),visited(V,false),dfn(V),low(V) {}
	vector<vector<edge>> bcc();//返回双连通分量
};

void Undirected::dfs(int root, int parent)
{
	int children = 0;
	visited[root] = true;
	dfn[root] = low[root] = ++count;
	for (auto i : adj[root])
	{
		if (!visited[i])//(root, i)是DFS生成树的树边
		{
			++children;
			s.push(edge(root, i));

			dfs(i, root);//DFS

			if (low[i] < low[root])//如果后代有边通往root的祖先，root不是衔接点，更新low[root]
				low[root] = low[i];

			else if (parent == -1 && children > 1 ||
				parent != -1 && low[i] >= dfn[root])//root是衔接点，以它为界限确定一个BCC或者桥边
			{
				edge first_edge(root, i);
				if (s.top() != first_edge)//如果root不是连着桥边，则将该BCC存入_bcc
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
				else//直接抛弃桥边
					s.pop();
			}
		}
		else if (i != parent && dfn[i] < dfn[root])//(root, i)是DFS生成树的回退边
		{
			if (dfn[i] < low[root])//更新low[root]
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
		//如果DFS根节点不是衔接点，则s还有残余的元素
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