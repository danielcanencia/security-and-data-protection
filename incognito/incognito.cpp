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

	} catch (char* e) {
		cout << e << endl; 
		return -1;
	}

	// Sort qid set
	sort(qids.begin(), qids.end());
	
	// Levels per hierchary. Useful to construct node
	// tables
	vector<int> nodeMax;
	for (const auto& entry : hierarchies_set) {
		nodeMax.emplace_back(entry.size() - 1);
	}
	Tree tree(vector<int> { 1,2 });
	//Tree tree(nodeMax);	


	// Main Algorithm
	//  - Construct Ci and Ei (nodes and edges)
	vector<vector<int>> C;
	C = tree.getPermutations(1);

	for (const auto& entry : C) {
		for (const auto& val : entry) {
			cout << to_string(val) + ",";
		}
		cout << endl;
	}

	/*
	for (int i=1; i < (int)qids.size(); i++) {
		// Algorithm	
		C[i-1] = tree.getPermutations(i);
	}*/


	return 0;
}

