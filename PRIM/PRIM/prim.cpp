#include<iostream>
#include<vector>
#define BIG_NUM 1000000000
#define left(cur) (cur*2)
#define right(cur) (cur*2+1)
using namespace std;

struct edge {
	int from, to, weight;
	edge(int f=-1, int t=-1, int w=BIG_NUM) :from(f), to(t), weight(w) {}
	friend ostream& operator << (ostream& os, edge &ed)
	{
		os << "(" << ed.from << "--" << ed.to << "," << ed.weight << ")";
		return os;
	}
};

class Prim_MST {
	int V;//顶点数
	vector<edge*> MST;
	//存放最小生成树中的边，
	//同时也是最小边堆（每次将选好的边移至堆数组的尾部）

	vector<vector<edge>> adj;//图的邻接链表
	vector<edge> closest;//存放余下点到已确定点集的最小边
	vector<bool> added;//判断某点是否已加入最小生成树
private:
	int count = 0;//记录已加入点个数
	void heapify();//调整堆
public:
	Prim_MST(int v,vector<vector<int>> &init) :V(v), adj(V), closest(V), added(V,false)
	{
		added[0] = true; ++count;
		for (int i = 0; i < V; ++i)
		{
			closest[i].from = i;
			MST.push_back(&closest[i]);
		}
		for (auto i : init)
			adj[i[0]].push_back(edge(i[0], i[1], i[2])), adj[i[1]].push_back(edge(i[1], i[0], i[2]));
		for (auto i : adj[0])
			closest[i.to].to = 0, closest[i.to].weight = i.weight;
		heapify();
	}
	vector<edge> get_MST();//返回最小生成树的边集合
};

void Prim_MST::heapify()
{
	int n = V - count;
	for (int i = n / 2; i > 0; --i)
	{
		edge* tmp = MST[i];
		int j;
		for (j = i; left(j) <= n && MST[j]->weight > MST[left(j)]->weight || 
			right(j) <= n && MST[j]->weight > MST[right(j)]->weight;)
			if (right(j) > n || MST[right(j)]->weight > MST[left(j)]->weight)
			{
				MST[j] = MST[left(j)];
				j = left(j);
			}
			else
			{
				MST[j] = MST[right(j)];
				j = right(j);
			}
		MST[j] = tmp;
	}
}

vector<edge> Prim_MST::get_MST()
{
	while (count < V)
	{
		int n = V - count;
		added[MST[1]->from] = true;
		for (auto i : adj[MST[1]->from])
		{
			if (!added[i.to] && i.weight < closest[i.to].weight)
			{
				closest[i.to].to = i.from;
				closest[i.to].weight = i.weight;
			}
		}
		edge *tmp = MST[1];
		MST[1] = MST[n];
		MST[n] = tmp;
		++count;
		heapify();
	}
	return closest;
}

int main()
{
	vector<vector<int>>init{{0,1,4},{1,2,8},{2,3,7},{3,4,9},{4,5,10},{5,6,2},{6,7,1},{7,0,8},
	{1,7,11},{2,5,4},{8,2,2},{8,7,7},{8,6,6} };
	Prim_MST mst(9, init);
	vector<edge> tmp = mst.get_MST();
	for (unsigned i = 1; i < tmp.size(); ++i)
		cout << tmp[i] << " ";
	return 0;
}