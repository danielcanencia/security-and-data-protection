#include "evaluate.h"

tuple<vector<vector<string>>, vector<vector<vector<string>>>> datafly(
                                vector<vector<string>> dataset,
								map<int, vector<vector<string>>> hierarchies,
								vector<int> qids, const int K) {
    vector<vector<string>> qidsDataset, result;
    vector<vector<vector<string>>> clusters;

	// Obtain a subset of dataset containing only qids
	for (size_t i=0; i < dataset.size(); i++) {
		vector<string> aux;
		for (const int& idx : qids) {
			aux.emplace_back(dataset[i][idx]);
		}
		qidsDataset.emplace_back(aux);
	}

	// 1. Create a hierarchy tree for every qid
	vector<Tree> trees;
	for (const int& val : qids) {
		trees.emplace_back(Tree(hierarchies[val]));
	}

	int idx;
	// 2&3. Calculate frequencies & Check if KAnonimity is satisfied
	while (!isKAnonSatisfied(qidsDataset, K)) {
		// 3.1 Find qid with the most distinct values
		idx = findMostDistinctQid(qidsDataset);

		// 3.2 Generalize all qid values in freq
		try {
			generalizeQid(qidsDataset, idx, trees[idx]);
		} catch (const char* e) {
			cout << e << endl;
			return make_tuple(qidsDataset, clusters);
		}
	}
	cout << "END" << endl;

	// 4. Supress records which are not K-Anonymous (< K times)
	supressRecords(qidsDataset, K);


	// Update original dataset with generalized values
    result = dataset;
	for (size_t i=0; i < dataset.size(); i++) {
		for (size_t j=0; j < qids.size(); j++) {
			result[i][qids[j]] = qidsDataset[i][j];
		}
    }

    // Create equivalence classes or clusters
    clusters = createClusters(result, qids);
	return make_tuple(result, clusters);
}
