#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <locale>

using namespace std;
//A function which gets a word from the stream and returns the stream to the call
istream& getword(istream& is, string& word)
{
	locale here("");//error correction - read in '

	char ch;
	while (is.get(ch))
	{
		if (isalpha(ch, here))
			break;
	}

	//2.Quit function early if no alpha word is found

	if (!is)
		return is;

	//Store only alphas that follow, until non-alpha occurs

	string buffer;
	buffer += ch;
	while (is.get(ch))
	{
		if (isalpha(ch, here))
			buffer += ch;
		else
			break;
	}

	//Dont eat the last character that follows word - considered more professional
	if (is)
		is.unget();//restores last read non-alpha back to the stream

	//Remove bad or fail
	if (is.eof())
		is.clear(ios::eofbit);//if eor, clear other flags and set eof
								//bad bit = 0 , failbit = 0 , eofbit =1
	//update word
	word = std::move(buffer);
	return is;
}


int countOccurrences(vector<string> v, string w)
{
	vector<string>::iterator it;
	int counter = 0;
	for (it = v.begin(); it < v.end(); it++)
	{
		if (*it == w)
			counter++;
	}
	return counter;
}

void removeDuplicates(vector<string>& v)
{
	sort(v.begin(), v.end());
	v.erase(unique(v.begin(), v.end()), v.end());
}

void longestWord(vector<string> v)
{
	vector<string>::iterator it;
	int length = 0;
	string word;
	string lword;
	for (it = v.begin(); it < v.end(); it++)
	{
		word = *it;
		if (word.length() > length)
		{
			length = word.length();
			lword = word;
		}
	}
	
	cout << "Longest word: " << lword << " Length: " << length << endl;
}

int averageWordLength(vector<string> v)
{
	vector<string>::iterator it;
	int totalWord = 0;
	int wordLength = 0;
	string word;
	for (it = v.begin(); it < v.end(); it++)
	{
		word = *it;
		totalWord++;
		wordLength += word.length();
	}
	return (wordLength / totalWord);
}


int main(int argc, char* argv[])
{
	//1. Read in the filename and open an input filestream
	ifstream inFile("Numbers1.txt");
	//Test for success
	if (!inFile)
	{
		cerr << "Input file opening failed.\n";
		return EXIT_FAILURE;
	}

	//2. Get the words from the file and store in a vector
	vector<string> v;
	string word;
	while (getword(inFile, word))
	{
		v.push_back(word);
		cout << word << endl;
	}
	//Finished reading in file
	cout << "Finished reading in file!\n";

	cout << "Count Occurences of word:" << countOccurrences(v, "word") << endl;

	longestWord(v);

	cout << "Average word length: " << averageWordLength(v) << endl;
}

