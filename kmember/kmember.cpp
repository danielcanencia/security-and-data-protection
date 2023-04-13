#include "kmember.h"

map<int, vector<vector<string>>>
evaluate(vector<vector<string>> dataset,
         map<int, vector<vector<string>>> hierarchies, vector<int> numQids,
         vector<int> catQids, const int confAtt, const int K, const int L,
         const vector<string> sensitiveValues, const int diversityPenalty,
         const int diversity, int &count) {
  // Create a hierarchy tree for every qid
  map<int, Tree> trees;
  for (const int &i : catQids) {
    trees[i] = Tree(hierarchies[i]);
  }

  // Main algorithm
  vector<vector<string>> S = dataset;
  int r = randomRecord(S);
  map<int, vector<vector<string>>> res;
  vector<vector<string>> c;
  vector<string> aux = S[r];

  // 1st loop
  while ((int)S.size() >= K) {
    r = furthestRecord(aux, r, S, hierarchies, trees, numQids, catQids);
    vector<vector<string>> c(1, S[r]);
    S.erase(S.begin() + r);

    while ((int)c.size() < K) {
      r = findBestRecord(S, c, hierarchies, trees, numQids, catQids, confAtt, L,
                         sensitiveValues, diversityPenalty, diversity);

      c.emplace_back(S[r]);
      S.erase(S.begin() + r);
    }

    aux = c.back();
    res[count] = c;
    count += 1;
  }

  // 2nd loop
  int idx;
  while (S.size() > 0) {
    r = randomRecord(S);
    idx = findBestCluster(res, S[r], hierarchies, trees, numQids, catQids);
    res[idx].emplace_back(S[r]);
    S.erase(S.begin() + r);
  }

  return res;
}

int main(int argc, char **argv) {

  if (argc != 2) {
    cout << "\nInvalid arguments.\n"
            "Use ./kmember [data directory]\n\n"
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
  // Read qid names
  const int nqids = readNumberOfQids();
  vector<string> qidNames = readQidNames(nqids);
  // Conf Att
  string attName = readConfidentialAttName();
  vector<string> confAttNames;
  if (attName.size() != 0)
    confAttNames.emplace_back(attName);

  // Read data file and hierarchy folders
  vector<string> headers;
  vector<int> catQids, numQids, allQids;
  vector<int> confAtts;
  vector<vector<string>> hierarchies, dataset;
  map<int, vector<vector<string>>> hierarchiesMap;

  try {
    hierarchiesMap =
        readDirectory(fs::path(argv[1]), dataset, headers, qidNames,
                      confAttNames, catQids, confAtts, false);
    if (confAtts.size() < confAttNames.size()) {
      cout << endl << "******************" << endl;
      cout << "An error occured.\nCheck the confidential "
              "attributte names entered exists.\nThey should be "
              "referenced in their respectives "
              "hierarchy files."
           << endl
           << endl;
      return -1;
    }

    sort(catQids.begin(), catQids.end());
    // Compare headers and qids
    allQids = getQidsHeaders(headers, qidNames);

    sort(allQids.begin(), allQids.end());
    set_difference(allQids.begin(), allQids.end(), catQids.begin(),
                   catQids.end(), inserter(numQids, numQids.begin()));

  } catch (const char *e) {
    cout << e << endl;
    return -1;
  }

  // Confidential attribute
  int confAtt;
  if (confAttNames.size() != 0)
    confAtt = confAtts[0];

  // Read Parameters
  const int K = readParameter("k-anonymity", "K", dataset.size());
  if (K == -1) {
    cout << "Kmember needs parameter K" << endl;
    return 1;
  }
  const int L = readParameter("l-diversity", "L", dataset.size());

  // Diversity Penalty, select metric used
  int diversityPenalty, diversity;
  vector<string> sensitiveValues;
  if (L != -1) {
    diversityPenalty = readDiversityPenalty();
    diversity = readDiversity();

    // Read sensitive values for sensitive diversity metric
    if (diversity == 1) {
      vector<string> values = transpose(hierarchiesMap[confAtt])[0];
      sensitiveValues = readSensitiveValues(values);
    }
  }

  // Measure Execution Time
  auto start = chrono::high_resolution_clock::now();
  // *********************************
  // Main algorithm
  int count = 0;
  map<int, vector<vector<string>>> res =
      evaluate(dataset, hierarchiesMap, numQids, catQids, confAtt, K, L,
               sensitiveValues, diversityPenalty, diversity, count);
  // *********************************
  auto stop = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::seconds>(stop - start);
  cout << endl << "===> Kmember Execution Time: ";
  cout << duration.count() << " seconds" << endl;

  cout << "===> Number of clusters: ";
  cout << count << endl;

  // Write Anonymized Clusters
  cout << "===> Writing data" << endl;
  string directory = argv[1];
  if (directory.back() != '/')
    directory += "/";
  directory += to_string(K) + "K";
  if (L != -1)
    directory += "_" + to_string(L) + "L";

  for (int i = 0; i < count; i++) {
    writeAnonymizedTable(fs::path(directory), headers, res[i], K, -1, -1,
                         "cluster" + to_string(i + 1), false);
  }

  // METRICS
  vector<vector<vector<string>>> clusters;
  for (const auto &[k, cluster] : res)
    clusters.emplace_back(cluster);

  // DM
  calculateDM(clusters, dataset.size(), K, L, -1);
  // CAvg
  calculateCAVG(clusters, dataset.size(), K, L, -1);

  return 0;
}
