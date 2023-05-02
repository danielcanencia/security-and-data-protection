#include "metrics.h"

#define GET_NAME(Var) (#Var)

// Discernibility Metric
void calculateDM(vector<vector<vector<string>>> clusters, const int tableSize,
                 const long double param, const string paramName) {
  long double dm = 0;
  for (const vector<vector<string>> &cluster : clusters) {
    if ((long double)cluster.size() >= param)
      dm += pow(cluster.size(), 2.0);
    else
      dm += tableSize * cluster.size();
  }

  cout << "\t* DM using " + paramName + ": ";
  cout << fixed << setprecision(3) << dm << endl;
}

void calculateDM(vector<vector<vector<string>>> clusters, const int tableSize,
                 const int K, const int L, const long double T) {
  if (K != -1)
    calculateDM(clusters, tableSize, (long double)K, GET_NAME(K));
  if (L != -1)
    calculateDM(clusters, tableSize, (long double)L, GET_NAME(L));
  if (T != -1.0)
    calculateDM(clusters, tableSize, T, GET_NAME(T));
}

// Equivalent Class Size Metric
void calculateCAVG(vector<vector<vector<string>>> clusters, const int tableSize,
                   const long double param, const string paramName) {
  const long double denom = (long double)(clusters.size() * param);
  const long double cavg = (denom == 0) ? 0 : tableSize / denom;

  cout << "\t* CAvg using " + paramName + ": ";
  cout << fixed << setprecision(3);
  cout << cavg << endl;
}

void calculateCAVG(vector<vector<vector<string>>> clusters, const int tableSize,
                   const int K, const int L, const long double T) {
  if (K != -1)
    calculateCAVG(clusters, tableSize, K, GET_NAME(K));
  if (L != -1)
    calculateCAVG(clusters, tableSize, L, GET_NAME(L));
  if (T != -1.0)
    calculateCAVG(clusters, tableSize, T, GET_NAME(T));
}

// Generalized Information Loss
long double calculateMaxNumValue(vector<string> entries) {
  try {
    stold(entries[0]);
  } catch (const exception &e) {
    throw "\nError, some categorical attribute "
          " can't be treated as numerical\n";
  }

  // Max Value
  string globalMax =
      (*max_element(entries.begin(), entries.end(),
                    [&](const string &a, const string &b) {
                      string str1, str2;
                      size_t posA = a.find("~");
                      size_t posB = b.find("~");
                      if (posA == string::npos)
                        str1 = a;
                      else
                        str1 = a.substr(a.find("~") + 1, a.size());

                      if (posB == string::npos)
                        str2 = b;
                      else
                        str2 = b.substr(b.find("~") + 1, b.size());

                      return strtold(str1.c_str(), NULL) <
                             strtold(str2.c_str(), NULL);
                    }))
          .c_str();

  if (globalMax.find("~") == string::npos)
    return strtold(globalMax.c_str(), NULL);

  return strtold(
      globalMax.substr(globalMax.find("~") + 1, globalMax.size()).c_str(),
      NULL);
}

long double calculateMinNumValue(vector<string> entries) {

  // Max Value
  string globalMin = (*min_element(entries.begin(), entries.end(),
                                   [&](const string &a, const string &b) {
                                     string str1, str2;
                                     size_t posA = a.find("~");
                                     size_t posB = b.find("~");
                                     if (posA == string::npos)
                                       str1 = a;
                                     else
                                       str1 = a.substr(0, a.find("~"));

                                     if (posB == string::npos)
                                       str2 = b;
                                     else
                                       str2 = b.substr(0, b.find("~"));

                                     return strtold(str1.c_str(), NULL) <
                                            strtold(str2.c_str(), NULL);
                                   }))
                         .c_str();

  if (globalMin.find("~") == string::npos)
    return strtold(globalMin.c_str(), NULL);

  return strtold(globalMin.substr(0, globalMin.find("~")).c_str(), NULL);
}

long double calculateNumGenILoss(const string entry, vector<string> entries,
                                 const long double globalMax,
                                 const long double globalMin) {
  const int delimeterPos = entry.find("~");

  const long double min = strtold(entry.substr(0, delimeterPos).c_str(), NULL);
  const long double max =
      strtold(entry.substr(delimeterPos + 1, entry.size()).c_str(), NULL);

  if ((max - min) == 0.0 || (globalMax - globalMin) == 0.0) {
    return 0;
  }

  return (max - min) / (globalMax - globalMin);
}

long double calculateCatGenILoss(const string entry, Tree tree) {

  const int directChildren = tree.getDirectChildren(entry).size();
  const int childrenInLevel = tree.getChildrenInLevel(entry).size();
  const int numerator =
      directChildren >= 1 ? directChildren - 1 : directChildren;
  const int denominator =
      childrenInLevel >= 1 ? childrenInLevel - 1 : childrenInLevel;

  return denominator == 0 ? 0 : numerator / (long double)denominator;
}

void calculateGenILoss(vector<vector<string>> transposedDataset,
                       map<int, Tree> trees, const vector<int> allQids,
                       const vector<int> catQids, const vector<int> numQids,
                       const int tableSize) {
  const long double initialLoss =
      1.0 / (transposedDataset[0].size() * (catQids.size() + numQids.size()));
  long double loss = 0;

  vector<map<string, int>> catFreqs, numFreqs;
  vector<tuple<long double, long double>> numGlobal;
  vector<string> numEntries;

  // Construct a frequency map of values for each qid
  for (const auto &idx : allQids) {
    map<string, int> freqs;
    for (const auto &entry : transposedDataset[idx]) {
      try {
        freqs[entry] += 1;
      } catch (...) {
        freqs[entry] = 1;
        if (find(numQids.begin(), numQids.end(), idx) != numQids.end())
          numEntries.emplace_back(entry);
      }
    }

    auto it = find(catQids.begin(), catQids.end(), idx);
    if (find(numQids.begin(), numQids.end(), idx) != numQids.end()) {
      numFreqs.emplace_back(freqs);
      tuple<long double, long double> tuple(
          make_tuple(calculateMaxNumValue(transposedDataset[idx]),
                     calculateMinNumValue(transposedDataset[idx])));
      numGlobal.emplace_back(tuple);
    } else if (it != catQids.end())
      catFreqs.emplace_back(freqs);
  }

  // Numerical Attributes
  for (size_t i = 0; i < numFreqs.size(); i++) {
    long double globalMax = get<0>(numGlobal[i]);
    long double globalMin = get<1>(numGlobal[i]);
    // Calculate Attributes GenILoss
    for (const auto &[k, v] : numFreqs[i])
      loss += calculateNumGenILoss(k, numEntries, globalMax, globalMin) * v;
  }

  // Categorical Attributes
  for (size_t i = 0; i < catFreqs.size(); i++) {
    // Calculate Attributes GenILoss
    for (const auto &[k, v] : catFreqs[i])
      loss += calculateCatGenILoss(k, trees[catQids[i]]) * v;
  }

  cout << "\t* GenILoss: ";
  cout << fixed << setprecision(3);
  cout << initialLoss * loss << endl;
}
