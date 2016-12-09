#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

struct date {
	int year, month, day;
	date(int y=2016, int m=1, int d=1) :year(y), month(m), day(d) {}
};

class linear_sort {
	int bucket_upper;
	int count_upper;
	int year_upper;
	vector<date> a_date;
	vector<int> a_int;
	vector<double> a_double;
public:
	linear_sort(vector<date>&ini, int i):a_date(ini),year_upper(i){}
	linear_sort(vector<int>&ini, int i) :a_int(ini), count_upper(i) {}
	linear_sort(vector<double>&ini, int i):a_double(ini),bucket_upper(i){}
};

void count_sort(vector<int> &a, int upper) {
	vector<int> c(upper, 0);
	vector<int> b(a.size());
	for (auto i : a)	++c[i];
	for (int i = 1; i < c.size(); ++i)	c[i] += c[i - 1];
	for (int i = a.size() - 1; i >= 0; --i) {
		b[--c[a[i]]] = a[i];
	}
	copy(b.begin(), b.end(), a.begin());
}

void aux_count_sort_year(vector<date>&a, int upper) {
	vector<int> c(upper, 0);
	vector<date> b(a.size());
	for (auto i : a)	++c[i.year];
	for (int i = 1; i < c.size(); ++i)	c[i] += c[i - 1];
	for (int i = a.size() - 1; i >= 0; --i) {
		b[--c[a[i].year]] = a[i];
	}
	copy(b.begin(), b.end(), a.begin());
}

void aux_count_sort_month(vector<date>&a, int upper) {
	vector<int> c(upper, 0);
	vector<date> b(a.size());
	for (auto i : a)	++c[i.month];
	for (int i = 1; i < c.size(); ++i)	c[i] += c[i - 1];
	for (int i = a.size() - 1; i >= 0; --i) {
		b[--c[a[i].month]] = a[i];
	}
	copy(b.begin(), b.end(), a.begin());
}

void aux_count_sort_day(vector<date>&a, int upper) {
	vector<int> c(upper, 0);
	vector<date> b(a.size());
	for (auto i : a)	++c[i.day];
	for (int i = 1; i < c.size(); ++i)	c[i] += c[i - 1];
	for (int i = a.size() - 1; i >= 0; --i) {
		b[--c[a[i].day]] = a[i];
	}
	copy(b.begin(), b.end(), a.begin());
}

void radix_sort(vector<date>&a,int year_upper) {
	aux_count_sort_day(a, 31);
	aux_count_sort_month(a, 13);
	aux_count_sort_year(a, year_upper);
}

void isort(vector<double>&a) {
	for (int i = 1; i < a.size(); ++i) {
		double val = a[i], j;
		for (j = i; j > 0 && a[j - 1]>val; --j)	a[j] = a[j - 1];
		a[j] = val;
	}
}

void bucket_sort(vector<double>&a, int upper) {
	vector<vector<double>> bucket(10*upper);
	for (auto i : a)	bucket[(int)(i * 10)].push_back(i);
	for (int i = 0; i < bucket.size(); ++i)	sort(bucket[i].begin(), bucket[i].end());
	auto t = a.begin();
	for (auto i : bucket) {
		copy(i.begin(), i.end(), t);
		t += i.size();
	}
}

void test_count_sort() {
	srand(time(0));
	vector<int> test_case;
	for (int i = 0; i < 10000; ++i)	test_case.push_back(rand() % RAND_MAX);
	auto start = clock();
	count_sort(test_case, RAND_MAX);
	//for (auto i : test_case) cout << i << " ";
	auto end = clock();
	cout << "10000个随机数计数排序用时" << end - start << endl;
}

void test_radix_sort() {
	srand(time(0));
	vector<date> test_case;
	for (int i = 0; i < 10000; ++i)	test_case.push_back(date(rand() % 200+1900,rand()%12+1,rand()%30+1));
	auto start = clock();
	radix_sort(test_case, 2200);
	//for (auto i : test_case) cout << i.year << "-" << i.month << "-" << i.day << " ";
	auto end = clock();
	cout << "10000个日期基数排序用时" << end - start << endl;
}

void test_bucket_sort() {
	srand(time(0));
	vector<double> test_case;
	for (int i = 0; i < 10000; ++i)	test_case.push_back((double)rand()/RAND_MAX);
	auto start = clock();
	bucket_sort(test_case, 1);
	//for (auto i : test_case) cout << i << " ";
	auto end = clock();
	cout << "10000个0到1的小数桶排序用时" << end - start << endl;
}

int main() {
	test_count_sort();
	test_radix_sort();
	test_bucket_sort();
	return 0;
}