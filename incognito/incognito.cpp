#include "incognito.h"

vector<Graph> graphGeneration(const vector<int>& qids,
			     map<int, int> nodeMax,
			     int numAttr) {
	vector<Graph> graphs;
	vector<vector<int>> C;


	C = getPermutations(numAttr, qids);

	// Generate trees for permutations of $perm qids
	for (int perm=0; perm < (int)C.size(); perm++) {
		vector<int> CMaxValue;
		for (const auto& entry : C[perm]) {
			CMaxValue.emplace_back(nodeMax[entry]);
		}

		// Generate Graph
		GraphData graphData;
		// C[perm] => curr qid indexes 
		// CMaxValue => max qid indexes
		Graph graph(CMaxValue, C[perm], graphData);
		graphs.emplace_back(graph);
	}
	return graphs;
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

void printGraphKAnon(Graph graph, const vector<GraphNode>& kNodes,
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


map<string, vector<string>> generateGeneralizationMap(
	vector<vector<string>> hierarchy, vector<int> qids) {

	map<string, vector<string>> genMap;

	for (const auto& entry : hierarchy) {
		genMap[entry[0]] = entry;
	}

	return genMap;
}

map<int, map<string, vector<string>>> generateGeneralizationsMap(
	map<int, vector<vector<string>>> hierarchies,
	vector<int> qids) {

	map<int, map<string, vector<string>>> gensMap;
	for (const auto& qid : qids)
		gensMap[qid] = generateGeneralizationMap(hierarchies[qid], qids);

	return gensMap;	
}


/*
map<int, map<string, string>> generateGeneralizationMap(
	map<int, vector<vector<string>>> hierarchies,
	vector<int> data, vector<int> qids) {

	map<int, map<string, string>> gens;
	for (size_t i=0; i < qids.size(); i++) {
		const int qid = qids[i];
		map<string, string> qidMap;
		for (size_t j = 0; j < hierarchies[qid][0].size(); j++)
			qidMap[hierarchies[qid][0][j]] = hierarchies[qid][data[i]][j];
		gens[qid] = qidMap;
	}

	return gens;
}*/

vector<vector<string>> generateAnonymizedDataset(
	vector<vector<string>> dataset, map<int, vector<vector<string>>> hierarchies_map,
	vector<Graph> graphs, vector<int> qids) {

	//cout << "Generating Anonymized Table: " << endl;
	const GraphNode node = graphs.back().getFinalKAnon();
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
		for (size_t j=0; j < dataset[0].size(); j++) {
			auto it = find(qids.begin(), qids.end(), j);
			if (it != qids.end()) {
				row.emplace_back(gens[j][dataset[i][j]]);
				continue;
			}
			row.emplace_back(dataset[i][j]);
		}
		result.emplace_back(row);
	}

	return result;
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
	vector<string> qidNames = {
		"Age", "Education", "Marital-status",
		"Native-country", "Occupation", "Race", "Relationship",
		"Salary", "Sex", "Workclass"
	};

	//vector<string> qidNames = {"Birthdate", "Sex", "Zipcode"};
	/*vector<string> qidNames = {
		"Age", "Education"
	};*/

	// Read csv data file
	vector<string> headers;
	vector<int> qids;
	vector<vector<string>> dataset, transposedDataset;
	map<int, vector<vector<string>>> hierarchies_map;
	map<int, vector<vector<string>>> transposedHierarchyMap;

	try {
		hierarchies_map = read_directory(fs::path(argv[1]),
					dataset, headers, K, qidNames,
					qids, true);
		sort(qids.begin(), qids.end());
		for (const auto& qid : qids) {
			transposedHierarchyMap[qid] = transpose(hierarchies_map[qid]);
		}
		//hierarchies_map = read_directory(fs::path(argv[1]),
		//			dataset, qids, headers, K);
		transposedDataset = transpose(dataset);
	} catch (const char* e) {
		cout << e << endl; 
		return -1;
	}

	// Levels per hierchary. Useful to construct node
	// and edges tables
	map<int, int> nodeMax;
	for (const int& qid : qids) {
		nodeMax[qid] = hierarchies_map[qid].size() - 1;
	}

	// Generate a generalization map
	map<int, map<string, vector<string>>> gensMap =
		generateGeneralizationsMap(transposedHierarchyMap, qids);

	// Print a hierarchy level helper
	// printHelper(headers, qids, nodeMax, hierarchies_map);

	// Generate all posible graphs containing qids
	// defined by qid variable
	vector<Graph> graphs = graphGeneration(qids, nodeMax, 1);

	// Main Algorithm
	vector<Graph> rGraphs;
	for (size_t i=1; i < qids.size() + 1; i++) {
		for (size_t gsize=0; gsize < graphs.size(); gsize++) {
			Graph g = graphs[gsize];
			set<GraphNode> nodesQueue = g.getRoots();
			vector<GraphNode> kanon;

			// Main Loop
			while (!nodesQueue.empty()) {
				GraphNode node = *(nodesQueue.begin());
				nodesQueue.erase(nodesQueue.begin());

				if (!node.marked()) {
					// Not marked
					if (node.getKAnonymity(hierarchies_map,
							       transposedDataset,
								   gensMap,
							       g.getQids(), K)) {
						g.markGeneralizations(node);
					}
					else {
				 		g.addGeneralizations(node, nodesQueue);
					}
				}
			}

			// Print Graph Solution
			//printGraphKAnon(g, kanon, headers, qids);
			rGraphs.emplace_back(g);
		}


		// Generate graphs
		if (i <= qids.size()) 
			graphs = graphGeneration(qids, nodeMax, i+1);
	}

	// Construct anonymized dataset
	vector<vector<string>> result = generateAnonymizedDataset(dataset,
		hierarchies_map, rGraphs, qids);

	// 3. Write anonymized table
	// Changed headers for non alterated ones
	writeAnonymizedTable(fs::path(argv[1]), headers, result, K);


	// GCP Analysis
	// 1. Create a hierarchy tree for every qid
	vector<Tree> trees;
	for (const int& val : qids) {
		trees.emplace_back(Tree(transposedHierarchyMap[val]));
	}

	// 2. Create equivalence classes or clusters
	vector<vector<vector<string>>> clusters = createClusters(result, qids);

	// 3. Especify weights, if any (Already entered by user)
	vector<double> weights(qids.size(), 1.0/qids.size());

	// 4. Calculate NCP for every qid value included in every cluster
	// Convert list into map
	map<int, Tree> treeMap;
	for (size_t i=0; i < qids.size(); i++) {
		treeMap[qids[i]] = trees[i];
	}
	// 5. Calculate NCP
	vector<long double> cncps = calculateNCPS(
		clusters, weights, qids, {}, treeMap);

	// 6. Calculate GCP
	printAnalysis(clusters, dataset.size(), qids, cncps);

	return 0;
}

