#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<int> get_next(string s)
{
	vector<int> next;
	if (s.empty())
		return next;
	next.push_back(-1);
	for (int i = 1, j = -1; i < s.size();)
	{
		if (j == -1 || s[j] == s[i-1])
			next.push_back(++j), ++i;
		else
			j = next[j];
	}
	return next;
}

vector<int> match(string s, string t)
{
	vector<int> matched;
	vector<int> next = get_next(t);
	unsigned j = 0, m = t.size(), i = 0, n = s.size();
	while (i<n)
	{
		if (j == m)
		{
			i -= m;
			matched.push_back(i++);
			j = 0;
		}
		else if (j == -1 || s[i] == t[j])
			++i, ++j;
		else
			j = next[j];
	}
	if (j == m)
		matched.push_back(i - m);
	return matched;
}

void test()
{
	string s, t;
	cout << "请输入主串："; cin >> s;
	cout << "请输入模式串："; cin >> t;
	vector<int> n = get_next(t);

	cout << "next数组如下：";
	for (auto c : n)
		cout << c << " ";
	cout << endl;

	cout << "匹配结果如下：";
	vector<int> m = match(s, t);
	for (auto i : m)
		cout << i << " ";
	cout << endl;
}

int main()
{
	test();
	return 0;
}