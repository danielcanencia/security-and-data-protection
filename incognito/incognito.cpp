#include "hierarchy.h"
#include "tree.h"

using namespace std;


vector<Tree> graphGeneration(const vector<int>& qids,
			     const vector<int>& nodeMax,
			     int numAttr) {
	vector<Tree> trees;
	vector<vector<int>> C;


	C = getPermutations(numAttr, qids);

	for (const auto& entry : C) {
		for (const auto& val : entry) {
			cout << to_string(val) + ", ";
		}
		cout << endl;
	}

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

	/*
	System.out.print("Insert K: ");
	int K;
	getline(cin, K);
	*/
	int K = 2;

	// Read csv data file
	string headers;
	vector<int> qids;
	vector<vector<string>> dataset, transposedDataset;
	vector<vector<vector<string>>> hierarchies_set;

	try {
		hierarchies_set = read_directory(fs::path(argv[1]), dataset, qids, headers);
		// [0][x] => B0 (menos generica)
		// [i][x] => Bi
		// ...

		transposedDataset = transpose(dataset);
		for (const auto& entry : dataset) {
			for (const auto& val : entry) 
				cout << val + ", ";
			cout << endl;
		}
		cout << "---------------------------" << endl;
		for (const auto& hierarchy : hierarchies_set) {
			for (const auto& entry : hierarchy) {
				for (const auto& val : entry) { 
					cout << val + ". ";
				}
				cout << "|" << endl;
			}
			cout << endl;
		}
		cout << "Qids: ";
		for (const auto& qid : qids) cout << qid;
		cout << endl;
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

	cout << "NodeMax: ";
	for (const auto& entry : nodeMax) cout << entry;
	cout << endl;

	// Generate all posible graphs containing qids
	// defined by qid variable
	vector<Tree> graphs = graphGeneration(qids, nodeMax, 1);
	cout << "Roots: " << endl;
	/*for (auto li: graphs) {
		for (Node entry : li.getRoots()) {
			entry.print();
			cout << endl;
		}
	}*/


	// Main Algorithm
	for (int i=1; i < (int)qids.size(); i++) {
		// Check graph/edge k-anonymity
		//cout << graphs[i].checkKAnonymity(dataset) << endl;

		for (int gsize=0; gsize < (int)graphs.size(); gsize++) {
			Tree g = graphs[gsize];
			vector<Node> nodesQueue = g.getRoots();
			cout << "Roots: ";
			for (Node entry : nodesQueue) {
				entry.print();
				cout << endl;
			}

			// Main Loop
			while (!nodesQueue.empty()) {
				Node node = nodesQueue[0];
				nodesQueue.erase(nodesQueue.begin());

				if (!node.marked()) {
					// Not marked
					cout << "Generalization for ";
					node.print();
					if (node.getKAnonymity(hierarchies_set,
							       transposedDataset,
							       dataset, 
							       g.getQids(), K)) {
						cout << "IT ITS KANON" << endl;
						g.markGeneralizations(node);
					}
					else {
						cout << "Node GENS: ";
				 		if (g.addGeneralizations(node, nodesQueue))
							cout << "No children" << endl;
						sort(nodesQueue.begin(), nodesQueue.end());
						cout << "Queue: ";
						for (const auto& entry : nodesQueue)
							entry.print();
						cout << endl;
					}
				}
			}

		}

		// Generate graphs
		graphs = graphGeneration(qids, nodeMax, i+1);

	}


	return 0;
}

