#include <algorithm>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;
using type = int;
using it = vector<type>::iterator;
const int bound = 10;
void isort(it beg, it end)
{
	for (auto i = beg + 1; i < end; ++i)
	{
		type key = *i;
		auto j = i;
		while (j > beg && *(j - 1) > key)
			*j = *(j - 1), --j;
		*j = key;
	}
}
it median(it beg, it end)
{
	auto mid = beg + (end - beg) / 2;
	if (*beg < *(end - 1)) 
	{
		if (*mid < *beg)	return beg;
		if (*mid > *(end - 1))	return end-1;
		return mid;
	}
	if (*mid < *(end - 1))	return end - 1;
	if (*mid > *beg)	return beg;
	return mid;
}
void my_swap(it left, it right)
{
	auto tmp = *left;
	*left = *right;
	*right = tmp;
}
it partion(it beg, it end, it pivot)
{
	auto left = beg, right = end-1;
	auto key = *pivot;
	while (left < right) {
		for (; left < right && *left < key; ++left);
		for (; left < right && *right >= key; --right);
		my_swap(left, right);
	}
	return left;
}
void qsort(it beg, it end)
{
	if (is_sorted(beg, end))	return;
	if (end - beg < bound)
		isort(beg, end);
	else {
		//三数取中法
		auto pivot = median(beg, end);
		auto mid = partion(beg, end, pivot);
		qsort(beg, mid);
		qsort(mid + 1, end);
	}
}

void qsort_normal(it beg, it end) {
	auto pivot = median(beg, end);
	auto mid = partion(beg, end, pivot);
	qsort(beg, mid);
	qsort(mid + 1, end);
}

void test_inc(void f(it,it)) {
	srand(time(0));
	vector<int> test_case;
	for (int i = 1; i < 100000; ++i) {
		test_case.push_back(i);
	}
	auto start = clock();
	f(test_case.begin(), test_case.end());
	auto end = clock();
	cout << "100000个正序数排列用时" << (end - start) << endl;
}

void test_dec(void f(it, it)) {
	srand(time(0));
	vector<int> test_case;
	for (int i = 100000; i > 0; --i) {
		test_case.push_back(i);
	}
	auto start = clock();
	f(test_case.begin(), test_case.end());
	auto end = clock();
	cout << "100000个逆序数排列用时" << (end - start) << endl;
}

void test_rand(void f(it, it)) {
	srand(time(0));
	vector<int> test_case;
	for (int i = 100000; i > 0; --i) {
		test_case.push_back(rand());
	}
	auto start = clock();
	f(test_case.begin(), test_case.end());
	auto end = clock();
	cout << "100000个随机数排列用时" << (end - start) << endl;
}

void test_0_1(void f(it, it)) {
	srand(time(0));
	vector<int> test_case;
	for (int i = 100000; i > 0; --i) {
		test_case.push_back(rand()%2);
	}
	auto start = clock();
	f(test_case.begin(), test_case.end());
	auto end = clock();
	cout << "100000个0或1的数排列用时" << (end - start) << endl;
}

void test_all_same(void f(it, it)) {
	srand(time(0));
	vector<int> test_case(100000, 1);
	auto start = clock();
	f(test_case.begin(), test_case.end());
	auto end = clock();
	cout << "100000个相同的数排列用时" << (end - start) << endl;
}

void test_partially_sorted(void f(it, it)) {
	srand(time(0));
	vector<int> test_case;
	for (int i = 0; i < 5; ++i) {
		for (int j = 0; i < 10000; ++i)
			test_case.push_back(rand());
		for (int i = 10000; i < 20000; ++i)
			test_case.push_back(i);
	}
	auto start = clock();
	f(test_case.begin(), test_case.end());
	auto end = clock();
	cout << "100000个部分有序的数排列用时" << (end - start) << endl;
}

int main()
{
	test_inc(qsort_normal); test_inc(qsort);
	test_dec(qsort_normal); test_dec(qsort);
	test_rand(qsort_normal); test_rand(qsort);
	test_partially_sorted(qsort_normal); test_partially_sorted(qsort);
	test_all_same(qsort_normal); test_all_same(qsort);
	test_0_1(qsort_normal); test_0_1(qsort);
	return 0;
}