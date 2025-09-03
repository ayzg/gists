#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;
//By convention the order of input args follows these rules:
//- .exe is always first and always present
//-the input filename is always provided
//-the input filename is before the output, if one is provided
//-any switches that operate on the input may come before the input
//filename

int main(int argc, char* argv[])
{
	cout << argc << endl;
	for (int i = 0; i < argc; ++i)
	{
		cout << argv[i] << " "; \
	}
	cout << endl;

	//MOVE THE ARGUMENTS (ommitting .exe file) to a C++ container
	vector<string> v;
	for (int i = 1; i < argc; ++i)
		v.push_back(argv[i]);
	//return 0

	//4.create filenames for file objects
	string infilename, outfilename;
	if (v.size() == 2)
	{
		infilename = v.front();
		outfilename = v.back()
	}
	else if (v.size() == 1)
	{


	}
}