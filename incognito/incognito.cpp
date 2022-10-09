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

		//tree.printNodesTable();
		//tree.printEdgesTable();
	}
	return trees;
}

void printHelper(vector<string> headers, vector<int> qids,
		 vector<int> nodeMax,
		 vector<vector<vector<string>>> hierarchies_set) {

	// Print Solutions
	cout << endl;
	cout << "******* Solution *******" << endl;

	// Print qids and their hierarchy levels
	cout << "--------Qids hierarchy levels-------" << endl;
	for (const int& qid : qids) {
		cout << headers[qid] + " >> " << endl;
		for (int i=0; i < nodeMax[qid] + 1; i++) {
			cout << "\tHierarchy Level " + to_string(i) + " ";
			cout << "{ ";
			for (const string& value : hierarchies_set[qid][i])
				cout << value + ", ";
			cout << " }" << endl;
		}
		cout << endl;
	}

	cout << "------------------------------------" << endl;
}

void printGraphKAnon(Tree graph, const vector<Node>& kNodes,
		     const vector<string>& headers,
		     const vector<int>& qid) {
	// Print every set of qids that is kanon and/or is
	// a direct generalization of one

	cout << "Graph with qids: { ";
	const vector<int>& qids = graph.getQids();
	cout << headers[qids[0]];
	for (size_t i=1; i < qids.size(); i++) {
		cout << ", " + headers[qids[i]];
	}
	cout << " }" << endl;

	// Sets of keys for the node
	graph.printAllKAnon();
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
	vector<string> headers;
	vector<int> qids;
	vector<vector<string>> dataset, transposedDataset;
	vector<vector<vector<string>>> hierarchies_set;

	try {
		hierarchies_set = read_directory(fs::path(argv[1]), dataset, qids, headers);
		transposedDataset = transpose(dataset);
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


	// Generate all posible graphs containing qids
	// defined by qid variable
	vector<Tree> graphs = graphGeneration(qids, nodeMax, 1);

	// Print a hierarchy level helper
	printHelper(headers, qids, nodeMax, hierarchies_set);

	// Main Algorithm
	for (size_t i=1; i < qids.size() + 1; i++) {
		for (size_t gsize=0; gsize < graphs.size(); gsize++) {
			Tree g = graphs[gsize];
			vector<Node> nodesQueue = g.getRoots();
			vector<Node> kanon;
			
			// Main Loop
			while (!nodesQueue.empty()) {
				Node node = nodesQueue[0];
				nodesQueue.erase(nodesQueue.begin());

				if (!node.marked()) {
					// Not marked
					if (node.getKAnonymity(hierarchies_set,
							       transposedDataset,
							       g.getQids(), K)) {
						g.markGeneralizations(node);
					}
					else {
				 		g.addGeneralizations(node, nodesQueue);
						sort(nodesQueue.begin(), nodesQueue.end());
					}
				}
			}
			// Print Graph Solution
			printGraphKAnon(g, kanon, headers, qids);

		}
	

		// Generate graphs
		if (i <= qids.size()) 
			graphs = graphGeneration(qids, nodeMax, i+1);
	}



	
	return 0;
}

