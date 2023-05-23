#include "evaluate.h"

bool readyForSuppression(vector<vector<string>> dataset, vector<int> qids,
                         const int tableSize, const int K,
                         const long double suppThreshold) {
  // Create dataset containing only qids
  vector<vector<string>> records;
  // Obtain a subset of dataset containing only qids
  for (size_t i = 0; i < dataset.size(); i++) {
    vector<string> aux;
    for (const int &idx : qids) {
      aux.emplace_back(dataset[i][idx]);
    }
    records.emplace_back(aux);
  }
  int count = 0;
  for (const auto &freq : calculateFreqs(records)) {
    if (freq < K)
      count += freq;
  }

  // Percentage of tuple that can be suppress: (loss * dataset.size() / 100)
  return (count <= (suppThreshold * tableSize) / 100);
}

tuple<vector<vector<string>>, vector<vector<vector<string>>>>
datafly(vector<vector<string>> dataset,
        map<int, vector<vector<string>>> hierarchies, vector<int> qids,
        vector<int> confAtts, const long double suppThreshold, const int K) {
  vector<vector<string>> qidsDataset, result;
  vector<vector<vector<string>>> clusters;

  qidsDataset = dataset;

  // 1. Create a hierarchy tree for every qid
  vector<Tree> trees;
  for (const int &val : qids) {
    trees.emplace_back(Tree(hierarchies[val]));
  }

  int idx;
  // 2&3. Calculate frequencies & Check if KAnonimity is satisfied
	while (!isKAnonSatisfied(qidsDataset, K)) {
    // Check if table is ready for suppression
    if (readyForSuppression(qidsDataset, qids, dataset.size(), K,
                            suppThreshold)) {
      // 4. Supress records which are not K-Anonymous (< K times)
      supressRecords(qidsDataset, qids, K);
      break;
    }

    // 5. Find qid with the most distinct values
    idx = findMostDistinctQid(qidsDataset, qids);

    // 6. Generalize all qid values in freq
    try {
      generalizeQid(qidsDataset, qids[idx], trees[idx]);
    } catch (const char *e) {
      cout << e << endl;
      return make_tuple(qidsDataset, clusters);
    }
  }

  // Create equivalence classes or clusters
  clusters = createClusters(qidsDataset, qids);
  return make_tuple(qidsDataset, clusters);
}
