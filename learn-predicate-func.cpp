/*
Name: Anton Yashchenko
Date: June 2, 2017
Resource: Chapter 10.3.1 Predicate Function, See A2 for list of algorithms
*/

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool byDescending(int lhs, int rhs)
{
	return lhs > rhs;
}

int main()
{
	//1. Reference variables
	int value = 1024;
	int& reference = value;

	cout << reference << endl;
	reference = 1025;
	cout << reference << " and " << value << endl;

	//2. Pointers
	int *p = &value;
	cout << p << " and " << *p << endl;
	*p = 1028;
	cout << *p << " and " << value << endl;

	//3. Vector
	vector<int> v = { 10,2,3,4,5,6,4,8,7 };

	for (auto x : v)
		cout << x << ", ";
	cout << endl;

	sort(v.begin(), v.end()); //default sort is ascending

	for (auto x : v)
		cout << x << ", ";
	cout << endl;

	//takes true or false value for each comparison

	sort(v.begin(), v.end(), byDescending);


}