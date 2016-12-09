#include <iostream>
#include <vector>
using namespace std;

class Graph {
	int V;//amount of vertices
	vector<vector<int>> mat;//adjacent matrix
private:
	int count = 0;//used to indicate the order of visited vertices
	vector<int> first, last;//first time to visit and the last time to visit
	void dfs(int);
	void dfs_2(int, vector<int>&);
public:
	//adjacent list used to initialize adjacent matrix
	Graph(vector<vector<int>>& init) : V(init.size()), first(init.size(),0), last(init.size()), 
		mat(init.size(),vector<int>(init.size(),0))
	{
		for (unsigned i = 0; i < init.size(); ++i)
			for (unsigned j = 0; j < init[i].size(); ++j)
				++mat[i][init[i][j]];
	}
	//return Strongly Connected Component
	vector<vector<int>> scc();
};

//to store the fisrt and the last time visited order of each vertex
void Graph::dfs(int root)
{
	first[root] = ++count;
	for (int i = 0; i < V; ++i)
		if (mat[root][i] && !first[i])
			dfs(i);
	last[root] = ++count;
}

//dfs the transpose of mat, each SCC is a traverse from the vertex with the larget last order while not visited yet
void Graph::dfs_2(int root, vector<int>& vec)
{
	first[root] = ++count;
	last[root] = -1;
	vec.push_back(root);
	for (int i = 0; i < V; ++i)
		if (mat[i][root] && !first[i])//notice that mat[i][root] is the transpose of mat[root][i]
			dfs_2(i, vec);
}

vector<vector<int>> Graph::scc()
{
	vector<vector<int>> _scc;
	//store first, last
	while (count < V + V)
	{
		int root = 0;
		for (; first[root]; ++root);
		dfs(root);
	}
	//dfs the transpose of mat
	for (count = 0, first.assign(first.size(),0); count < V;)
	{
		//select the vertex with the lagest last among those not visited
		int root = 0, max = -1;
		for (int i = 0; i < V; ++i)
			if (last[i] > max)
			{
				max = last[i];
				root = i;
			}
		vector<int> tmp;
		dfs_2(root, tmp);
		_scc.push_back(tmp);
	}
	return _scc;
}
//
//int main()
//{
//	vector<vector<int>> init{ {1},{4,2,5},{3,6},{2,7},{0,5},{6},{5,7},{7} };
//	Graph eight_points(init);
//	vector<vector<int>> scc = eight_points.scc();
//	for (auto i : scc)
//	{
//		for (auto j : i)
//			cout << j << " ";
//		cout << endl;
//	}
//	return 0;
//}
