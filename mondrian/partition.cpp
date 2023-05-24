#include "partition.h"

Partition::Partition(vector<vector<string>> data,
                     vector<string> generalizations, vector<int> qids,
                     vector<int> isQidCat, map<int, Tree> trees,
                     vector<int> confAtts, int K, int L, long double T) {
  this->data = data;
  this->generalizations = generalizations;
  this->qids = qids;
  this->isQidCat = isQidCat;
  this->trees = trees;
  this->confAtts = confAtts;
  this->K = K;
  this->L = L;
  this->T = T;
  this->allowedCuts = vector<int>(qids.size(), 1);
}

vector<vector<string>> Partition::getResult() const {
  vector<vector<string>> result = data;

  for (size_t i = 0; i < data.size(); i++) {
    for (size_t j = 0; j < qids.size(); j++) {
      result[i][qids[j]] = generalizations[j];
    }
  }

  return result;
}

int Partition::getNumAllowedCuts() {
  return accumulate(allowedCuts.begin(), allowedCuts.end(), 0);
}

void Partition::setAllowedCuts(int value, int dim) { allowedCuts[dim] = value; }

int Partition::chooseDimension() {
  int dimension, width;
  dimension = width = -1;

  for (size_t i = 0; i < qids.size(); i++) {
    if (allowedCuts[i] == 0)
      continue;

    int aux = normWidth(i);
    if (aux > width) {
      width = aux;
      dimension = i;
    }
  }

  return dimension;
}

int Partition::normWidth(int dimension) {
  // Normalized range of values for dimension
  vector<string> elements;
  for (const vector<string> &entry : data) {
    elements.emplace_back(entry[qids[dimension]]);
  }

  auto it = unique(elements.begin(), elements.end());
  elements.resize(distance(elements.begin(), it));

  return elements.size();
}

string Partition::findMedian(int dimension) {
  // Calculate qid frecuencies (qid values are sorted)
  map<string, int> map = calculateQidFreqs(this->data, qids[dimension]);
  vector<pair<string, int>> freqs;
  freqs.resize(map.size());
  copy(map.begin(), map.end(), freqs.begin());

  // Sort values
  sort(freqs.begin(), freqs.end(),
       [](const pair<string, int> &x, const pair<string, int> &y) {
         return stoi(x.first) < stoi(y.first);
       });

  // Sum frequency values
  vector<int> values;
  transform(freqs.begin(), freqs.end(), std::back_inserter(values),
            [](const auto &tuple) { return tuple.second; });
  int nValues = accumulate(values.begin(), values.end(), 0);

  // Find middle index 
  double middle = nValues / 2.0;
  // Cut is not allowed
  if (freqs.size() < 2 || middle < K || middle < L || middle < T) {
    return "";
  }

  // Find first split set of keys
  string splitValue;
  int aux = 0;
  for (size_t i = 0; i < freqs.size(); i++) {
    aux += freqs[i].second;
    if (aux >= middle) {
      splitValue = freqs[i].first;
      break;
    }
  }

  return splitValue;
}

vector<int> Partition::getAttributeRanges(int dimension) {
  vector<int> values;
  for (const auto &record : this->data) {
    values.emplace_back(stoi(record[qids[dimension]]));
  }

  int min = *min_element(values.begin(), values.end());
  int max = *max_element(values.begin(), values.end());

  return {min, max};
}

bool Partition::isSplitKAnonymous(vector<vector<string>> split) {
  return (int)split.size() >= K;
}

bool Partition::isSplitLDiverse(vector<vector<string>> split) {
  vector<map<string, int>> freqs(confAtts.size());
  string key;

  for (const auto &entry : split) {
    for (size_t j = 0; j < confAtts.size(); j++) {
      key = entry[confAtts[j]];
      try {
        freqs[j][key] += 1;
      } catch (...) {
        freqs[j][key] = 1;
      }
    }
  }

  // Every confidential attribute should have, at least,
  // l well represented values
  for (const map<string, int> &attFreq : freqs) {
    if ((int)attFreq.size() < L)
      return false;
  }

  return true;
}

bool Partition::isSplitTClose(vector<vector<string>> split) {
  if (split.size() == 0)
    return false;

  // Get transposed split and original table
  vector<vector<string>> tSplit = transpose(split);
  vector<vector<string>> tData = transpose(data);

  // Generate a P frequency map for
  // every confidential attribute
  vector<map<string, int>> splitMaps;
  for (const auto &att : confAtts) {
    map<string, int> freqs;
    for (const auto &entry : tSplit[att]) {
      try {
        freqs[entry] += 1;
      } catch (...) {
        freqs[entry] = 1;
      }
    }
    splitMaps.emplace_back(freqs);
  }

  // Generate a Q frequency map and
  // a set of entries
  vector<map<string, int>> dataMaps;
  vector<set<string>> valueSets;
  for (const auto &att : confAtts) {
    set<string> entries;
    map<string, int> freqs;
    for (const auto &entry : tData[att]) {
      entries.insert(entry);
      try {
        freqs[entry] += 1;
      } catch (...) {
        freqs[entry] = 1;
      }
    }
    dataMaps.emplace_back(freqs);
    valueSets.emplace_back(entries);
  }

  // P and Q size
  int pSize, qSize;
  pSize = split.size();
  qSize = data.size();

  // Calculate EMD for every confidential attribute
  // using ED
  long double emd;
  for (size_t i = 0; i < confAtts.size(); i++) {
    emd = 0;
    for (const auto &entry : valueSets[i]) {
      if (splitMaps[i][entry]) {
        // Entry present in P
        // ED(Pi, Qi) = abs(Pi/Pi probability in P -
        // 					 Qi/Qi probability in Q)
        emd += abs((long double)splitMaps[i][entry] / pSize -
                   (long double)dataMaps[i][entry] / qSize);
      } else {
        // Not present in P
        // ED(Pi, Qi) = abs(0 - Qi/Qi probability in Q)
        emd += (long double)dataMaps[i][entry] / qSize;
      }
    }

    // ED(P, Q) =  sum(sum(pj - qj)) / 2
    emd /= 2;

    // Check if partition is tclose
    if (emd > T)
      return false;
  }

  return true;
}

bool Partition::isSplitValid(vector<vector<string>> split) {
  bool kanonymity, ldiversity, tcloseness;
  kanonymity = ldiversity = tcloseness = true;

  if (split.size() == 0)
    return false;

  if (K > 0) {
    kanonymity = isSplitKAnonymous(split);
  }
  if (L > 0) {
    ldiversity = isSplitLDiverse(split);
  }
  if (T > 0) {
    tcloseness = isSplitTClose(split);
  }

  return kanonymity && ldiversity && tcloseness;
}

vector<Partition> Partition::splitPartition(int dimension) {
  // Cut is not allowed for this specific dimension
  if (allowedCuts[dimension] == 0) {
    return {};
  }

  if (isQidCat[dimension])
    return splitPartitionCategorical(dimension);
  return splitPartitionNumeric(dimension);
}

vector<Partition> Partition::splitPartitionNumeric(int dimension) {
  string splitValue = findMedian(dimension);
  // Cut is not allowed
  if (splitValue == "") {
    return {};
  }

  // Check if splitValue is a range
  string split1, split2;
  split1 = splitValue;
  split2 = to_string(stoi(splitValue) + 1);

  // Get lowest and highest value present in partition,
  // Note that ranges are not a problem if not present
  // initial data, as we don't modify these values, just
  // this->generalizations value.
  string lowest, highest;
  vector<int> range = getAttributeRanges(dimension);
  lowest = to_string(range[0]);
  highest = to_string(range[1]);

  // Cut limit reached
  if (highest == split1) {
    this->setAllowedCuts(0, dimension);
    return {};
  }

  string gen1 = lowest == splitValue
                    ? splitValue
                    : (lowest == split1 ? lowest : lowest + "~" + split1);
  string gen2 = highest == splitValue
                    ? splitValue
                    : (highest == split2 ? highest : split2 + "~" + highest);

  // New partitions data
  vector<vector<string>> d1, d2;
  for (const auto &record : this->data) {
    int number = stoi(record[qids[dimension]]);
    if (number <= stoi(split1))
      d1.emplace_back(record);
    else
      d2.emplace_back(record);
  }

  // If splits aren't k-anonymous, return original partition
  if (!isSplitValid(d1) || !isSplitValid(d2)) {
    this->setAllowedCuts(0, dimension);
    return {};
  }

  // Updata generalizations array with new dimension values
  vector<string> gens1, gens2;
  gens1 = gens2 = this->generalizations;
  gens1[dimension] = gen1;
  gens2[dimension] = gen2;
  Partition p1(d1, gens1, qids, isQidCat, trees, confAtts, K, L, T);
  Partition p2(d2, gens2, qids, isQidCat, trees, confAtts, K, L, T);

  return {p1, p2};
}

vector<Partition> Partition::splitPartitionCategorical(int dimension) {

  vector<Partition> pts;
  Tree tree = trees[qids[dimension]];
  string middle = this->generalizations[dimension];
  vector<string> children = tree.getDirectChildren(middle);

  // If it doesn't have children, return empty vector
  if (children.size() == 0)
    return pts;

  // Split records based on dimension (qid) generalization values
  vector<vector<vector<string>>> splits(children.size(),
                                        vector<vector<string>>());
  for (const auto &record : this->data) {
    string qid = record[qids[dimension]];

    int i = 0;
    for (const auto &child : children) {
      if (!tree.isChild(child, qid)) {
        i++;
        continue;
      }
      splits[i].emplace_back(record);
      break;
    }
  }

  // If splits aren't k-anonymous, return original partition
  for (const auto &split : splits) {
    if (!isSplitValid(split)) {
      return pts;
    }
  }

  // Update generalizations and create new partitions
  for (size_t i = 0; i < splits.size(); i++) {
    if (splits[i].size() != 0) {
      vector<string> gens = generalizations;
      gens[dimension] = children[i];
      Partition p =
          Partition(splits[i], gens, qids, isQidCat, trees, confAtts, K, L, T);
      pts.emplace_back(p);
    }
  }

  return pts;
}
