#include "incognito.h"

using namespace std;


vector<Tree> graphGeneration(const vector<int>& qids,
			     map<int, int> nodeMax,
			     int numAttr) {
	vector<Tree> trees;
	vector<vector<int>> C;


	C = getPermutations(numAttr, qids);

	// Generate trees for permutations of $perm qids
	for (int perm=0; perm < (int)C.size(); perm++) {
		vector<int> CMaxValue;
		for (const auto& entry : C[perm]) {
			CMaxValue.emplace_back(nodeMax[entry]);
		}

		// Generate Tree
		TreeData treeData;
		// C[perm] => curr qid indexes 
		// CMaxValue => max qid indexes
		Tree tree(CMaxValue, C[perm], treeData);
		trees.emplace_back(tree);
	}
	return trees;
}

void printHelper(vector<string> headers, vector<int> qids,
		 map<int, int> nodeMax,
		 map<int, vector<vector<string>>> hMap) {

	// Print Solutions
	cout << endl;

	// Print qids and their hierarchy levels
	cout << "------- Qids hierarchy levels ------" << endl;
	for (const int& qid : qids) {
		cout << headers[qid] + " >> " << endl;
		for (int i=0; i < nodeMax[qid] + 1; i++) {
			cout << "\tHierarchy Level " + to_string(i) + " { ";
			size_t j = 0;
			for (;j < hMap[qid][i].size() - 1; j++)
				cout << hMap[qid][i][j] + ", ";
			cout << hMap[qid][i][j];
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

	// Read input
	// K
	string line;
	cout << "Insert K: ";
	getline(cin, line);
	int K = stoi(line);

	// Qids
	/*set<string> qid_set;
	cout << "Number of qids: ";
	getline(cin, line);
	const int nqids = stoi(line);
	for (int i=0; i < nqids; i++) {
		cout << "Enter qid " << i << ": ";
		string qid;
		getline(cin, qid);
		qid_set.insert(qid);
	}
	if ((int)qid_set.size() != nqids) {
		cout << "Input Error: Qids should be unique.";
		cout << "Check if you repeated some of them" << endl; 
		return 1;
	}*/
	//vector<string> qidNames(qid_set.begin(), qid_set.end());
	/*vector<string> qidNames = {
		"Age", "Education", "Marital-status",
		"Native-country", "Occupation", "Race", "Relationship",
		"Salary", "Sex", "Workclass"
	};*/
	vector<string> qidNames = {"Birthdate", "Sex", "Zipcode"};
	/*vector<string> qidNames = {
		"Age", "Education"
	};*/

	// Read csv data file
	vector<string> headers;
	vector<int> qids;
	vector<vector<string>> dataset, transposedDataset;
	map<int, vector<vector<string>>> hierarchies_map;

	try {
		hierarchies_map = read_directory(fs::path(argv[1]),
					dataset, headers, K, qidNames,
					qids, true);
		sort(qids.begin(), qids.end());
		//hierarchies_map = read_directory(fs::path(argv[1]),
		//			dataset, qids, headers, K);
		transposedDataset = transpose(dataset);
	} catch (const char* e) {
		cout << e << endl; 
		return -1;
	}

	for (const auto& a : hierarchies_map[2]) {
		for (const auto& b : a) {
			cout << b + ", ";
		}
		cout << endl;
	}

	// Levels per hierchary. Useful to construct node
	// and edges tables
	map<int, int> nodeMax;
	for (const int& qid : qids) {
		nodeMax[qid] = hierarchies_map[qid].size() - 1;
	}

	// Print a hierarchy level helper
	// printHelper(headers, qids, nodeMax, hierarchies_map);

	// Generate all posible graphs containing qids
	// defined by qid variable
	vector<Tree> graphs = graphGeneration(qids, nodeMax, 1);

	// Main Algorithm
	vector<Tree> rGraphs;
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
					if (node.getKAnonymity(hierarchies_map,
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
			rGraphs.emplace_back(g);
			/*if (i == qids.size() && gsize == graphs.size())
				rGraph = g;*/
		}


		// Generate graphs
		if (i <= qids.size()) 
			graphs = graphGeneration(qids, nodeMax, i+1);
	}

	// Construct anonymized dataset
	cout << "Generating Anonymized Table: " << endl;
	const Node node = rGraphs.back().getFinalKAnon();
	vector<int> data = node.getData();

	map<int, map<string, string>> gens;
	for (size_t i=0; i < qids.size(); i++) {
		const int qid = qids[i];
		map<string, string> qidMap;
		for (size_t j = 0; j < hierarchies_map[qid][0].size(); j++)
			qidMap[hierarchies_map[qid][0][j]] = hierarchies_map[qid][data[i]][j];
		gens[qid] = qidMap;
	}

	vector<vector<string>> result;
	for (size_t i=0; i < dataset.size(); i++) {
		vector<string> row;
		for (size_t j=0; j < qids.size(); j++)
			row.emplace_back(gens[qids[j]][dataset[i][qids[j]]]);
		result.emplace_back(row);
	}

	cout << "Result: " << endl;
	for (const auto& a : result) {
		for (const auto& b : a)
			cout << b + ", ";
		cout << endl;
	}

	//vector<vector<string>> result = createResult(nodes[nodeIdx], hierarchies_map, qids);

	// Create clusters
	vector<vector<vector<string>>> clusters;

	// Write data to file


	return 0;
}

