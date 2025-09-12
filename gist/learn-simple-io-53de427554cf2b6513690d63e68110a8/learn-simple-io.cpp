/*
Anton Yashchenko
May 11, 2017
Simple IO Example
*/

#include <iostream>
#include <limits>
#include <string>

using namespace std;

int main()
{
	//read in an integer and validate
	cout << "Please enter an integer: ";
	int i = 42;
	cin >> i;
	if (cin)
		cout << "i = " << i << endl;
	else
		cout << " Failed to read in an integer. \n";
	//clear the error from the cin
	cin.clear();

	//reading in a character
	cout << "Please enter an character: ";
	char ch;
	if (cin >> ch)
		cout << "ch = " << ch << endl;
	else
		cout << " Failed to read in an character. \n";

	//read in a word
	cout << "Enter a single word: ";
	string s;
	cin >> s;
	if (cin)
		cout << "s = " << s << endl;

	//ignores last item in the input stream
	cin.ignore();

	//read in ASCII value of char
	cout << "Please enter a character: ";
	cin.get(ch);
	cout << "ch = " << ch << ", ASCII = " << int(ch) << endl;

	//read in a line including whitespace
	cin.ignore();
	cout << "Please enter multiple words on a single line: ";
	getline(cin, s);
	cout << "s = \"" << s << "\"" << endl;


	//CLEARS STREAM FULLY
	while (!(cin >> i))
	{
		cerr << "Failed! Try again: ";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	cout << "i = " << i << endl;

}