#include "hierarchy.h"
#include "tree.h"

using namespace std;

int main(int argc, char** argv) {

	if (argc != 2) {
		cout << "\nInvalid arguments.\n"
			"Use ./incognito [data directory] [k]\n\n"
			"* Make sure your data directory meets the following structure:\n"
			"  (check dataset folder for an example)\n\n"
			" |-- [data directory]\n"
			"      |\n"
			"      |-- csv input file\n"
			"      |-- hierarchies\n"
			"           |\n"
			"           |-- hierarchy tables as csv files\n"
			"           |-- ....\n"
		<< endl;
		return -1;
	}

	// Read csv data file
	string headers;
	vector<int> qids;
	vector<vector<string>> dataset;
	vector<string**> hierarchies_set;
	hierarchies_set = read_directory(fs::path(argv[1]), dataset, qids, headers);

	// Catch throws !!!!!!


	// Construct Ci and Ei (nodes and edges)
	//Tree tree(hierarchies_set);
	return 0;
}
