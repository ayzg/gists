/*
Coder: Anton Yashchenko
Date: 5/12/2017
Description: Regular expressions doing string comparisons and raw string.
*/

#include <iostream>
#include <string>
#include <regex>

using namespace std;

int main()
{


	//regex expression containing "Hello,World!"
	regex reg("Hello, World!");
	//regex_match returns true if the string literal matches the string expression
	if (regex_match("Hello, World!", reg))
		cout << "Found a match\n";

	// "|" or compare whole words
	regex reg_or("(\\s*)([0-9]+)(\\s*)(in)(\\s*)");
	//regex reg_or("gr(a|e)y")
	if (regex_match(" 122in", reg_or) && regex_match("8  in", reg_or))
		cout << "Found a matchdsfsdfs\n";

	// "?" you may or may not have the previous letter
	regex reg_question("\\.txt$");
	if (regex_search("test.txt", reg_question))
		cout << "Found a matchSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS\n";

	// "*" you may have 0 to many of the previous letter
	regex reg_splat("ab*c");
	if (regex_match("ac", reg_splat) && regex_match("abbbbbbbbbc", reg_splat))
		cout << "Found a match\n";

	// "+" you may have 1 to many of the previous letter
	regex reg_plus("ab*c");
	if (regex_match("abc", reg_plus) && regex_match("abbbbbbbbbc", reg_plus))
		cout << "Found a match\n";

	// icase means case insensitivity - can be mixed and matched with other commands
	regex reg_icase("colou?r", regex::icase);
	if (regex_match("coLoUR", reg_icase) && regex_match("Color", reg_icase))
		cout << "Found a match\n";

	//You can also break up the regex into sections
	//                $1  $2 $3
	regex colorExp("(colo)(u)(r)", regex::icase);
	// regex_replace changes certain parts of a string expression if they match conditions
	string colors = "Colours, colours, color, colourize, Colunizer";
	colors = regex_replace(colors, colorExp, string("$1$3"));
	cout << colors << endl;

	// finding extensions
	cout << "Finding extensions\n";
	string filename = "main.cpp";
	// "$" at the end of a regex means that it compares the end of the string
	regex extensions("\\.(cpp|c|h|hpp)$");
	if (regex_search(filename, extensions))
		cout << filename << "is a c++ file \n";




}