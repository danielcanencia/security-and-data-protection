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
	vector<vector<vector<string>>> hierarchies_set;

	try {
		hierarchies_set = read_directory(fs::path(argv[1]), dataset, qids, headers);
		// [0][x] => B0 (menos generica)
		// [i][x] => Bi
		// ...

		for (const auto& hierarchy : hierarchies_set) {
			for (const auto& entry : hierarchy) {
				cout << "Qid's index: ";
				for (const auto& val : entry) { 
					cout << val + ". ";
				}
				cout << "|" << endl;
			}
			cout << endl;
		}
		for (const auto& entry : qids) {
			cout << entry << endl;
		}
		cout << endl;	
	} catch (char* e) {
		cout << e << endl; 
		return -1;
	}

	/*for (const int& entry : qids) {
		cout << entry << endl;
	}*/
	// Construct Ci and Ei (nodes and edges)
	//Tree tree(hierarchies_set);
	return 0;
}
