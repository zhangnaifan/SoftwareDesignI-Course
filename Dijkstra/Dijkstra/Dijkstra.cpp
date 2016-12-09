#include<iostream>
#include<vector>
#define BIG_NUM 1000000000
#define left(cur) (cur*2)
#define right(cur) (cur*2+1)
using namespace std;

struct edge {
	int from, to, dist;
	edge(int f=-1, int t = -1, int w = BIG_NUM) :from(f), to(t), dist(w) {}
	friend ostream& operator << (ostream& os, edge &ed)
	{
		os << "(" << ed.from << "->" << ed.to << "," << ed.dist << ")";
		return os;
	}
};

class Dijkstra {
	int V;
	vector<edge*> pq;//优先队列，维护最小边
	vector<vector<edge>> adj;//邻接链表
	vector<edge> closest;//余下点的最小边
	vector<bool> added;//标记是否已加入点集
private:
	int count = 0;//已加入点集数
	void heapify();
public:
	Dijkstra(int v, vector<vector<int>> &init) :V(v), adj(V), closest(V), added(V, false)
	{
		added[0] = true; ++count;
		for (int i = 0; i < V; ++i)
		{
			closest[i].from = i;
			pq.push_back(&closest[i]);
		}
		for (auto i : init)
			adj[i[0]].push_back(edge(i[0], i[1], i[2]));
		closest[0].to = closest[0].dist = 0;
		for (auto i : adj[0])
			closest[i.to].to = 0, closest[i.to].dist = i.dist;
		heapify();
	}
	vector<edge> shortest_path();
};

void Dijkstra::heapify()
{
	int n = V - count;
	for (int i = n / 2; i > 0; --i)
	{
		edge* tmp = pq[i];
		int j;
		for (j = i; left(j) <= n && pq[j]->dist > pq[left(j)]->dist ||
			right(j) <= n && pq[j]->dist > pq[right(j)]->dist;)
			if (right(j)>n || pq[right(j)]->dist > pq[left(j)]->dist)
			{
				pq[j] = pq[left(j)];
				j = left(j);
			}
			else
			{
				pq[j] = pq[right(j)];
				j = right(j);
			}
		pq[j] = tmp;
	}
}

vector<edge> Dijkstra::shortest_path()
{
	while (count < V)
	{
		int n = V - count;
		added[pq[1]->from] = true;
		for (auto i : adj[pq[1]->from])
		{
			if (!added[i.to] && i.dist + closest[i.from].dist < closest[i.to].dist)
			{
				closest[i.to].to = i.from;
				closest[i.to].dist = i.dist + closest[i.from].dist;
			}
		}
		edge *tmp = pq[1];
		pq[1] = pq[n];
		pq[n] = tmp;
		++count;
		heapify();
	}
	return closest;
}

class Floyd {
	vector<vector<int>> mat;
public:
	Floyd(int v, vector<vector<int>> &init): mat(v,vector<int>(v,BIG_NUM))
	{
		for (auto i : init)
			mat[i[0]][i[1]] = i[2];
		for (int k = 0; k < v; ++k)
			for (int i = 0; i < v; ++i)
				for (int j = 0; j < v; ++j)
					if (mat[i][j] > mat[i][k] + mat[k][j])
						mat[i][j] = mat[i][k] + mat[k][j];
	}
};

int main()
{
	vector<vector<int>>init{ {0,1,3},{0,4,5}, {1,2,6},{1,4,2},{2,3,2},{3,0,3},{4,1,1},{4,2,4},{4,3,6} };
	Dijkstra dijk(5, init);
	vector<edge> tmp = dijk.shortest_path();
	for (unsigned i = 0; i < tmp.size(); ++i)
		cout << tmp[i] << " ";
	return 0;
}