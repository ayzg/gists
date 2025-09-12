#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <locale>


using namespace std;


/*
Practice Coding Exercises
Using command line arguments read in the file called Horoscope.txt
*/
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

	if (!is)
		return is;

	string buffer;
	buffer += ch;
	while (is.get(ch))
	{
		if (isalpha(ch, here))
			buffer += ch;
		else
			break;
	}

	if (is)
		is.unget();

	if (is.eof())
		is.clear(ios::eofbit);

	word = std::move(buffer);

	return is;
}

int main(int argc, char* argv[])
{
	//1. Read in the filename and open an input filestream
	ifstream inFile(argv[1]);
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
		v.push_back(word);
	//Finished reading in file
	cout << "Finished reading in file!\n";
}

int StringLength(char* string)
{
	return strlen(string);
}

string BackwardString(string input)
{
	string output;
	output = input;

	reverse(output.begin(), output.end());

	return output;
}

int WordCounter(char* str)
{
	bool inSpaces = true;
	int numWords = 0;

	while (*str != NULL)
	{
		if (isspace(*str))
		{
			inSpaces = true;
		}
		else if (inSpaces)
		{
			numWords++;
			inSpaces = false;
		}

		++str;
	}

	return numWords;
}

void SentenceCapitalizer(char* str)
{
	bool endOfSent;
	while (*str != NULL)
	{
		if (*str == '.')
		{
			endOfSent = true;
		}
		else if (endOfSent)
		{
			toupper(*str);
			endOfSent = false;
		}
		str++;
	}
}

void CharacterAnalysis(char* str)
{
	int iVowels = 0, iConsonants=0, iPunctuation=0, iUppercase=0, iLowercase=0, iDigits=0;
	vector<char> vVowels = { 'a', 'e', 'i', 'o', 'u' ,'A','E','I','O','U'};
	vector<char> vConsonants = { 'B','C','D','F','G','H','J','K','L','M','N','P','Q','R','S','T','V','W','X','Y','Z' };
	vector<char> vPunctuation = { ',','.','\'','?','!',':',';' };

	
	while (*str != NULL)
	{

		//Punctuation
		for (int i = 0; i < vPunctuation.size; i++)
		{
			if (*str == vPunctuation[i])
			{
				iPunctuation++;
				break;
			}
		}

		//Uppercase
		if (isupper(*str))
			iUppercase++;
		else if (islower(*str))
			iLowercase++;

		//Digits
		if (isdigit(*str) != 0)
			iDigits++;

		//Vowels
		for (int i = 0; i < vVowels.size; i++)
		{
			if (*str == vVowels[i])
			{
				iVowels++;
				break;
			}
		}

		//Constants
		for (int i = 0; i < vConsonants.size; i++)
		{
			if (*str == vConsonants[i])
			{
				iConsonants++;
				break;
			}
		}
		//iterate
		str++;
	}

	//Output info
	cout << "Vowels:" << iVowels << endl;
	cout << "Consonants:" << iConsonants << endl;

}