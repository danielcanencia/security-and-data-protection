#include "hierarchy.h"
#include "tree.h"

using namespace std;


vector<Tree> graphGeneration(const vector<int>& qids,
			     const vector<int>& nodeMax,
			     int numAttr) {
	vector<Tree> trees;

	vector<vector<int>> C;
	C = getPermutations(numAttr, qids);

	// Generate trees for permutations of $perm qids
	for (int perm=0; perm < (int)C.size(); perm++) {
		vector<int> CMaxValue;
		for (const int& entry : C[perm]) {
			CMaxValue.emplace_back(nodeMax[entry]);
		}

		// Generate Tree
		TreeData treeData;
		// C[perm] => curr qid indexes 
		// CMaxValue => max qid indexes
		Tree tree(CMaxValue, C[perm], treeData);
		trees.emplace_back(tree);
		tree.printNodesTable();
		tree.printEdgesTable();
	}
	return trees;
}

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
				for (const auto& val : entry) { 
					cout << val + ". ";
				}
				cout << "|" << endl;
			}
			cout << endl;
		}
	} catch (char* e) {
		cout << e << endl; 
		return -1;
	}

	// Levels per hierchary. Useful to construct node
	// and edges tables
	vector<int> nodeMax;
	for (const auto& entry : hierarchies_set) {
		nodeMax.emplace_back(entry.size() - 1);
	}
	//nodeMax = vector<int> ({1, 2});


	// Generate all posible graphs containing qids
	// defined by qid variable
	vector<Tree> graphs = graphGeneration(qids, nodeMax, 1);

	// Main Algorithm
	for (int i=1; i < (int)qids.size(); i++) {

		for (int perm=0; perm < (int)qids.size(); perm++) {
			// Logic
		}
		graphs = graphGeneration(qids, nodeMax, i);
		cout << "PERM "<< to_string(i + 1) << endl;
	}

	return 0;
}

