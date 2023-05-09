#include "generalize.h"

/*
  Full-Domain Generalization
*/
void generalizeQid(vector<vector<string>> &dataset, const int qid, Tree tree) {

  for (size_t i = 0; i < dataset.size(); i++) {
    dataset[i][qid] = tree.getNextGen(dataset[i][qid]);
  }
}

void supressRecords(vector<vector<string>> &dataset, vector<int> qids,
                    const int K) {
  // Calculate frequencies and indexes
  map<string, tuple<int, vector<int>>> records =
      evaluateFrequencyByIdx(dataset, qids);

  // Suppress records with freqs < K
  vector<int> deletions;
  for (const auto &[key, tuple] : records) {
    int freq = get<0>(tuple);
    vector<int> idxs = get<1>(tuple);

    if (freq < K) {
      deletions.insert(deletions.begin(), idxs.begin(), idxs.end());
    }
  }

  sort(deletions.begin(), deletions.end());
  int n = 0;
  for (const auto &i : deletions) {
    dataset.erase(dataset.begin() + i - n);
    n++;
  }
}
