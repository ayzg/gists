#include <iostream>
#include <locale>
#include <sstream> 
#include <iomanip>
#include <regex>
#include <string>
#include <algorithm>
#include <vector>
#include <functional> 

using namespace std;
#include <filesystem> 
using namespace std::experimental::filesystem;


ostringstream summary;
unsigned long long fileCount = 0, sumCount = 0, totalFileSize = 0;

class ExtRecord {
public:
	path folder;
	unsigned count = 1;
	unsigned sizeV;
	ExtRecord(path ext, unsigned size) : folder(ext), sizeV(size) {};
};

inline bool operator == (ExtRecord const& ext, path const& path) {
	return ext.folder == path;
}

inline bool operator < (ExtRecord const& lhs, ExtRecord const& rhs) {
	return lhs.folder < rhs.folder;
}

inline bool operator > (ExtRecord const& lhs, ExtRecord const& rhs) {
	return rhs < lhs;
}

inline bool ascFileSize(ExtRecord const& lhs, ExtRecord const& rhs) {
	return lhs.sizeV > rhs.sizeV;
}

inline bool descFileSize(ExtRecord const& lhs, ExtRecord const& rhs) {
	return rhs.sizeV > lhs.sizeV;
}

ostream& operator << (ostream& os, ExtRecord const& extension) {
	os << extension.folder << " " << extension.count << " " << extension.sizeV << endl;
	return os;
}

void printExt(ExtRecord const& ext, unsigned& first, unsigned& second, unsigned& third) {
	cout << setw(first) << right << ext.folder << " : " << setw(second) << right << ext.count << " : " << setw(third) << right << ext.sizeV << endl;
}

void printDashes() { cout << "-" << " " << "-" << " " << "-" << endl; }

void print(vector<ExtRecord> const& files, regex const& exp) {
	unsigned col1 = 0, col2 = 0, col3 = 0;
	unsigned extT = 0, countT = 0, sizeT = 0;

	for (auto f : files) {
		if (regex_match(f.folder.string(), exp)) {
			if (f.folder.string().size() > col1)
				col1 = f.folder.string().size();
			if (to_string(f.count).length() > col2)
				col2 = to_string(f.count).length();
			if (to_string(f.sizeV).length() > col3)
				col3 = to_string(f.sizeV).length();
			extT++;
			countT += f.count;
			sizeT += f.sizeV;
		}
	}
	col1 += 7, col2 += 7, col3 += 7;

	cout << setw(col1) << right << "Ext" << " : " << setw(col2) << right << "#" << " : " << setw(col3) << right << "Total" << endl;
	printDashes();

	for (auto f : files) {
		if (regex_match(f.folder.string(), exp))
			printExt(f, col1, col2, col3);
	}
	printDashes();
	cout << setw(col1) << right << extT << " : " << setw(col2) << right << countT << " : " << setw(col3) << right << sizeT << endl;

	++fileCount;
	sumCount += countT;
	totalFileSize += sizeT;
	summary << setw(15) << right << countT << " : " << setw(20) << right << sizeT << endl;
}

void scan(vector<ExtRecord>& files, path const& f, regex const& ext) {
	vector<ExtRecord> currentFolder;
	directory_iterator d(f), e;
	while (d != e) {
		if (!is_directory(d->status()) && regex_match(d->path().extension().string(), ext)) {
			if (find(files.begin(), files.end(), d->path().extension()) != files.end()) {
				(*find(files.begin(), files.end(), d->path().extension())).count++;
				(*find(files.begin(), files.end(), d->path().extension())).sizeV += file_size(d->path());
			}
			else {
				ExtRecord current(d->path().extension(), file_size(d->path()));
				files.push_back(current);

			}
		}
		++d;
	}
}

void rscan(vector<ExtRecord>& files, path const& f, regex const& ext) {

	for (recursive_directory_iterator d(f), e; d != e; ++d) 
	{
		if (!is_directory(d->status()) && regex_match(d->path().extension().string(), ext)) {
			if (find(files.begin(), files.end(), d->path().extension()) != files.end()) {
				(*find(files.begin(), files.end(), d->path().extension())).count++;
				(*find(files.begin(), files.end(), d->path().extension())).sizeV += file_size(d->path());
			}
			else {
				ExtRecord current(d->path().extension(), file_size(d->path()));
				files.push_back(current);
			}
		}

	}
}

int main(int argc, char * argv[]) {

	//Locale and copyright
	summary.imbue(locale(""));
	cout.imbue(locale(""));

	vector<ExtRecord> files;
	vector<ExtRecord> currentfiles;

	//Switches
	/*
	c : c++ files
	j : java files
	m : multimedia files
	x : scan using regex
	R: reverse order listing
	s: sort by size
	h: show help
	v: verbose - list files as they are scanned
	*/


	//Initialize regex switches
	regex c_switch("c"),j_switch("j"),m_switch("m"), x_switch("x"),rec_switch("r"), rev_switch("R"),
		sort_switch("s"), v_switch("v"), help_switch("h"),path_check("C:\*");

	//If argc is 1, default scan
	if (argc == 1) {
		cout << "All files: " << current_path() << endl;
		rscan(files, current_path(), (regex)".*");
		sort(files.begin(), files.end());
		print(files, (regex)".*");
		return EXIT_SUCCESS;
	}

	//Determine path if argc is 
	path myPath;

	if (argc == 2)// If only two arguments, source is default path
		myPath = current_path();
	else {
		if (regex_search(argv[1], x_switch)) //Check for regex switch
		{
			if (argc == 4)//Check if path provided for regex
				myPath = argv[3];
			else // Set default path to source
				myPath = current_path();
		}
		else // NO regex
			myPath = argv[2];
	}

	if (argc == 3) {
		myPath = argv[2];
	}

	if (argc == 4)
	{
		myPath = argv[3];
	}



	//If path provided in argv[1] before switches, perform default scan or specified path
	if (regex_search(argv[1], path_check)) {
		cout << "Path detected, no switches: Initating default sort on specified folder." << endl;
		cout << "All files: " << argv[1] << endl;
		rscan(files, argv[1], (regex)".*");
		print(files, (regex)".*");
		return EXIT_SUCCESS;
	}
	
	//Print help documentation if requested. End program.
	if (regex_search(argv[1], help_switch)) {
		cout << "Usage: fileusage.exe [-rRsv(c|j|m|x)] [regularexpression] [folder\n\n";
		return EXIT_SUCCESS;
	}

	//Initiate regex extensions
	regex c_ext(R"r(\.(c|cpp|h|hpp|hxx)$)r"),java_ext(R"r(\.(class|java)$)r"), multi_ext(R"r(\.(avi|mpeg|mp4|3gp|wmv|mkv)$)r")
		,x_ext;

	// regex extension for scan 
	string scanExtensions = "\\.(";
	if (regex_search(argv[1], c_switch))
		scanExtensions += "c|cpp|h|hpp|hxx|";
	if (regex_search(argv[1], j_switch))
		scanExtensions += "class|java|";
	if (regex_search(argv[1], m_switch))
		scanExtensions += "cs|vb|jsl|";
	if (regex_search(argv[1], x_switch)) {
		string temp = argv[2];
		scanExtensions += temp.substr(2, temp.length() - 2);
		temp = temp.substr(2, temp.length() - 2);
		x_ext = R"r(\.)r" + temp;
	}
	else
	{
		scanExtensions = scanExtensions.substr(0, scanExtensions.length() - 2);
	}

	scanExtensions += ")$";

	if (!regex_search(argv[1], c_switch) && !regex_search(argv[1], j_switch) && !regex_search(argv[1], m_switch))
	{
		scanExtensions = ".*";
	}

	// Recursive vs Non-recursive scan
	if (regex_search(argv[1], rec_switch))
	{
		if (argc == 2)
		{
			rscan(files, myPath, (regex)".*");
	}
	else
	{
		scan(files, myPath, (regex)".*");
	}




	// Sorting and Reverse order
	if (regex_search(argv[1], rev_switch) && regex_search(argv[1], sort_switch))
		sort(files.begin(), files.end(), descFileSize);
	else if (regex_search(argv[1], rev_switch))
		sort(files.begin(), files.end(), greater<ExtRecord>());
	else if (regex_search(argv[1], sort_switch))
		sort(files.begin(), files.end(), ascFileSize);
	else
		sort(files.begin(), files.end());


	// Print Filtered Search Result
	cout << "All detected files based on all active switches: " << endl;
	summary << setw(15) << right << "Detected Files" << ":";
	print(files, (regex)scanExtensions);
	cout << endl;




}