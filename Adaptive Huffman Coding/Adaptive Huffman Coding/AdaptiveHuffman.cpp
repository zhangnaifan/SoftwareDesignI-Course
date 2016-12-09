#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;
struct node {
	unsigned count = 0;
	unsigned father = -1, left = -1, right = -1;
};

class huffmanTree {
	node tree[512];//存储字符
	unsigned NYT = 256;//空节点
	vector<unsigned> order;
	//节点的阶：阶高的在树的上部或者同一层的左侧
	//出现次数越多，阶越高

	unsigned sibling(unsigned)const;
	//可能用以与当前节点进行交换的兄弟节点，
	//其出现次数在插入之前相同，插入之后不符合哈夫曼树的性质

	void insert(unsigned);//插入一个新字符
	void adjust(unsigned);//自底向上调整
	void path_to_stream(unsigned, stringstream&)const;//将给定字符的哈夫曼编码写入字符串流
	void char_to_stream(char, stringstream&)const;//将给定字符的ASCII码写入字符串流
	void stream_to_file(stringstream&, string)const;//将字符串流以ASCII码形式写入文件流
	void exchange(unsigned,unsigned);//与兄弟节点进行交换
public:
	void encode(string,string);//编码
	void decode(string,string);//解码
};

void huffmanTree::encode(string ifname,string ofname)
{
	ifstream fin(ifname);
	stringstream ss;

	for (char ch; (ch=fin.get())!=EOF;)
		if (tree[ch].count)
		{
			path_to_stream(ch, ss);
			adjust(ch);
		}
		else
		{
			path_to_stream(NYT, ss);
			char_to_stream(ch, ss);
			insert(ch);
			adjust(ch);
			++NYT;
		}

	path_to_stream(NYT,ss);
	char_to_stream(EOF, ss);
	stream_to_file(ss, ofname);

	fin.close();
}

void huffmanTree::decode(string ifname, string ofname)
{
	ifstream fin(ifname);
	ofstream fout(ofname);
	streampos pos = fin.tellg();  // save current position
	fin.seekg(0, ios::end);
	auto length = fin.tellg();
	fin.seekg(pos);  // restore saved position

	char ch = fin.get();
	insert(ch); adjust(ch); ++NYT; fout << ch;

	stringstream ss;

	for (decltype(length) i = 1; ch = fin.get(), i < length; i += 1)
		for (int i = 0; i < 8; ++i, ch <<= 1)
			if (ch < 0)
				ss << '1';
			else
				ss << '0';
	fin.close();

	unsigned cur = 256;
	
	while (ss >> ch)
	{
		if (ch == '0')
			cur = tree[cur].left;
		else
			cur = tree[cur].right;
		if (cur == NYT)
		{
			char tmp = 0;
			for (int i = 0; i < 8; ++i)
			{
				ss >> ch;
				tmp <<= 1;
				if (ch == '1')
					tmp += 1;
			}
			if (tmp == EOF)
			{
				fout.close(); return;
			}
			fout << tmp; insert(tmp); adjust(tmp); ++NYT;
			cur = 256;
		}
		else if (cur < 256)
		{
			fout << char(cur);
			adjust(cur);
			cur = 256;
		}
	}
}

void huffmanTree::insert(unsigned ch)
{
	order.push_back(NYT);
	order.push_back(ch);

	tree[ch].father = NYT;

	tree[NYT].left = NYT + 1;
	tree[NYT].right = ch;

	tree[NYT + 1].father = NYT;
}

unsigned huffmanTree::sibling(unsigned c)const
{
	for (unsigned i = 0, count = tree[c].count, father = tree[c].father; order[i] != c; ++i)
		if (tree[order[i]].count == count - 1 && order[i] != father)
			return order[i];
	return 256;
}

void huffmanTree::adjust(unsigned c)
{
	++tree[c].count;
	while (c != 256)
	{
		unsigned right_sibling = sibling(c);
		if (right_sibling != 256)
			exchange(c, right_sibling);
		c = tree[c].father;
		++tree[c].count;
	}
}

void huffmanTree::exchange(unsigned c,unsigned r)
{
	unsigned c_father = tree[c].father, r_father = tree[r].father;
	bool is_c_left = (c == tree[c_father].left), is_r_left = (r == tree[r_father].left);
	if (is_c_left)
		tree[c_father].left = r;
	else
		tree[c_father].right = r;
	tree[r].father = c_father;
	if (is_r_left)
		tree[r_father].left = c;
	else
		tree[r_father].right = c;
	tree[c].father = r_father;

	unsigned pos1, pos2, i;
	for (i = 0; order[i] != r; ++i); pos2 = i;
	for (; order[i] != c; ++i); pos1 = i;
	order[pos1] = r;
	order[pos2] = c;
}

void huffmanTree::path_to_stream(unsigned ch, stringstream& ss)const
{
	vector<bool> s;
	for (; ch != 256; ch = tree[ch].father)
		if (ch == tree[tree[ch].father].left)
			s.push_back(false);
		else
			s.push_back(true);

	for (int i = s.size() - 1; i >= 0;--i)
		if (s[i])
			ss << '1';
		else
			ss << '0'; 
}

void huffmanTree::char_to_stream(char ch, stringstream&ss)const
{
	for (int i = 0; i < 8; ++i, ch <<= 1)
		if (ch < 0)
			ss << '1';
		else
			ss << '0';
}

void huffmanTree::stream_to_file(stringstream&ss, string ofname)const
{
	ofstream fout(ofname);
	unsigned i = 0; char ch = 0,tmp;
	for (; ss >> tmp; ++i)
	{
		if (i == 8)
		{
			fout << ch;
			ch = i = 0;
		}
		ch <<= 1;
		if (tmp == '1')
			ch += 1;
	}
	for (; i < 8; ++i)
		ch <<= 1;
	fout << ch;
	fout.close();
}

int main()
{
	string source("D:\\01_stuff\\source.txt");
	string encoded("D:\\01_stuff\\encoded.txt");
	string decoded("D:\\01_stuff\\decoded.txt");
	huffmanTree encoder, decoder;
	encoder.encode(source,encoded);
	decoder.decode(encoded, decoded);
	return 0;
}