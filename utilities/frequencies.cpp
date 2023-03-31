#include "frequencies.h"

vector<tuple<string, int>>
concatRecordsByIdx(const vector<vector<string>> dataset) {

  vector<tuple<string, int>> records;

  // Concatenate all elements
  for (size_t i = 0; i < dataset.size(); i++) {
    records.emplace_back(make_tuple(
        accumulate(dataset[i].begin(), dataset[i].end(), string()), i));
  }

  return records;
}

vector<string> concatRecords(const vector<vector<string>> dataset) {
  vector<string> records;

  // Concatenate all elements
  for (size_t i = 0; i < dataset.size(); i++) {
    records.emplace_back(
        accumulate(dataset[i].begin(), dataset[i].end(), string()));
  }

  return records;
}

map<string, int> calculateQidFreqs(const vector<vector<string>> dataset,
                                   const int dim) {

  vector<vector<string>> records = dataset;

  // Calculate frequency list
  map<string, int> freqs;

  for (const auto &record : records) {
    try {
      freqs[record[dim]] += 1;
    } catch (...) {
      freqs[record[dim]] = 1;
    }
  }

  return freqs;
}

vector<int> calculateFreqs(const vector<vector<string>> dataset) {

  // Concatenate all attributes of each record
  vector<string> records = concatRecords(dataset);

  // Calculate frequency list
  map<string, int> freqMap;

  for (const auto &record : records) {
    try {
      freqMap[record] += 1;
    } catch (...) {
      freqMap[record] = 1;
    }
  }

  // Get all values from freqMap
  vector<int> freqs;
  for (map<string, int>::iterator it = freqMap.begin(); it != freqMap.end();
       it++) {
    freqs.push_back(it->second);
  }

  return freqs;
}

int findMostDistinctQid(const vector<vector<string>> dataset) {

  size_t cols = dataset[0].size();
  vector<vector<string>> values(cols);

  // Get firt value of every attribute
  for (size_t i = 0; i < cols; i++)
    values[i].emplace_back(dataset[0][i]);

  // Every attribute has at least 1 distinct value
  vector<int> nvalues(cols, 1);

  // For all records
  for (size_t i = 1; i < dataset.size(); i++) {

    // Check if the auxiliar list (values) contains
    // the attribute value
    for (size_t j = 0; j < cols; j++) {
      if (find(values[j].begin(), values[j].end(), dataset[i][j]) ==
          values[j].end()) {

        values[j].emplace_back(dataset[i][j]);
        nvalues[j]++;
      }
    }
  }

  return distance(nvalues.begin(), max_element(nvalues.begin(), nvalues.end()));
}

vector<vector<vector<string>>> createClusters(vector<vector<string>> dataset,
                                              vector<int> qids) {
  // Construct clusters based on generalizations
  vector<vector<vector<string>>> clusters;
  // Map every unique combination of qids to a matrix/cluster;
  map<string, vector<vector<string>>> splitMap;

  for (const auto &entry : dataset) {
    string value;
    vector<string> record = entry;
    for (const auto &qid : qids)
      value.append(entry[qid]);

    try {
      splitMap[value].emplace_back(record);
    } catch (...) {
      splitMap[value] = vector<vector<string>>(1, record);
    }
  }

  // Create clusters from map values
  for (const auto &[k, v] : splitMap)
    clusters.emplace_back(v);

  return clusters;
}
