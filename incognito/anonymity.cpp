#include "anonymity.h"

bool isSplitKAnonymous(vector<vector<string>> split, const int K) {
  return (int)split.size() >= K;
}

bool isSplitLDiverse(vector<vector<string>> split, vector<int> confAtts,
                     const int L) {
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

bool isSplitTClose(vector<vector<string>> split, vector<vector<string>> data,
                   tuple<vector<map<string, int>>, vector<set<string>>> dataMap,
                   vector<int> confAtts, const long double T) {

  if (split.size() == 0)
    return false;

  // Generate a P frequency map for
  // every confidential attribute
  string key;
  vector<map<string, int>> splitMaps(confAtts.size());
  for (const auto &entry : split) {
    for (size_t j = 0; j < confAtts.size(); j++) {
      key = entry[confAtts[j]];
      try {
        splitMaps[j][key] += 1;
      } catch (...) {
        splitMaps[j][key] = 1;
      }
    }
  }

  vector<map<string, int>> dataFreqs = get<0>(dataMap);
  vector<set<string>> valueSets = get<1>(dataMap);

  // P and Q size
  int pSize, qSize;
  pSize = split.size();
  qSize = data.size();

  // Calculate EMD for every confidential attribute
  // using ED
  for (size_t i = 0; i < confAtts.size(); i++) {
    long double emd = 0;
    for (const auto &entry : valueSets[i]) {
      if (splitMaps[i][entry]) {
        // Entry present in P
        // ED(Pi, Qi) = abs(Pi/Pi probability in P -
        //					 Qi/Qi probability in Q)
        emd += abs((long double)splitMaps[i][entry] / pSize -
                   (long double)dataFreqs[i][entry] / qSize);
      } else {
        // Not present in P
        // ED(Pi, Qi) = abs(0 - Qi/Qi probability in Q)
        emd += (long double)dataFreqs[i][entry] / qSize;
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

bool isSplitValid(vector<vector<vector<string>>> splits,
                  tuple<vector<map<string, int>>, vector<set<string>>> dataMap,
                  vector<vector<string>> dataset, vector<int> confAtts,
                  const int K, const int L, const long double T) {
  bool kanonymity, ldiversity, tcloseness;
  kanonymity = ldiversity = tcloseness = true;

  if (splits.size() == 0)
    return false;

  if (K > 0) {
    for (const auto &split : splits) {
      if (!isSplitKAnonymous(split, K)) {
        kanonymity = false;
        break;
      }
    }
  }
  if (L > 0) {
    for (const auto &split : splits) {
      if (!isSplitLDiverse(split, confAtts, L)) {
        ldiversity = false;
        break;
      }
    }
  }
  if (T > 0) {
    for (const auto &split : splits) {
      if (!isSplitTClose(split, dataset, dataMap, confAtts, T)) {
        tcloseness = false;
        break;
      }
    }
  }

  return kanonymity && ldiversity && tcloseness;
}
