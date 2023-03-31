#include "mondrian.h"

int main(int argc, char **argv) {

  if (argc != 2) {
    cout << "\nInvalid arguments.\n"
            "Use ./datafly.out [data directory]\n\n"
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
  // Conf Atts Names
  vector<string> confAttNames = readConfidentialAttNames();

  // Read data file and hierarchy folders
  vector<string> headers;
  vector<int> catQids, confAtts, numQids, allQids;
  vector<int> isQidCat;
  map<int, vector<vector<string>>> hierarchiesMap;
  vector<vector<string>> dataset;

  try {
    hierarchiesMap =
        readDirectory(fs::path(argv[1]), dataset, headers, qidNames,
                      confAttNames, catQids, confAtts, false);
    sort(catQids.begin(), catQids.end());

    // Compare headers and qids
    allQids = getQidsHeaders(headers, qidNames);
    if (allQids.size() < qidNames.size()) {
      cout << endl << "******************" << endl;
      cout << "An error occured.\nCheck the qid "
              "names entered exists. They should be "
              "referenced\nin their respectives "
              "hierarchy files."
           << endl
           << endl;
      return -1;
    }
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

    sort(allQids.begin(), allQids.end());
    set_difference(allQids.begin(), allQids.end(), catQids.begin(),
                   catQids.end(), inserter(numQids, numQids.begin()));

    // Build a vector containing qid types
    for (const auto &qid : allQids) {
      if (find(catQids.begin(), catQids.end(), qid) != catQids.end()) {
        isQidCat.emplace_back(1);
        continue;
      }
      isQidCat.emplace_back(0);
    }

  } catch (const char *e) {
    cout << e << endl;
    return -1;
  }

  // Read Parameters
  int K, L;
  long double T;
  readParameters(dataset.size(), confAttNames.size(), K, L, T);
  // Read Weights
  vector<double> weights = readWeights(nqids, qidNames);

  // Ask for desired qid types to be used on metrics
  vector<int> numMetricsQids, catMetricsQids;
  tuple<vector<int>, vector<int>> metricsQids =
      readMetricsQids(numQids, catQids, headers);
  numMetricsQids = get<0>(metricsQids);
  catMetricsQids = get<1>(metricsQids);

  // Measure Execution Time
  auto start = chrono::high_resolution_clock::now();
  // *********************************
  // Main algorithm
  vector<vector<vector<string>>> clusters =
      mondrian(dataset, hierarchiesMap, allQids, isQidCat, confAtts, K, L, T);
  // *********************************
  auto stop = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::seconds>(stop - start);
  cout << endl << "===> Mondrian Execution Time: ";
  cout << duration.count() << " seconds" << endl;

  cout << "===> Number of clusters: ";
  cout << clusters.size() << endl;

  // Create matrix from clusters
  vector<vector<string>> result;
  for (const auto &partition : clusters) {
    result.insert(result.begin(), partition.begin(), partition.end());
  }
  // Write anonymized table
  writeAnonymizedTable(fs::path(argv[1]), headers, result, K, L, T);

  // METRICS
  cout << "===> Analysis: " << endl;
  // Create a hierarchy tree for every qid
  map<int, Tree> trees;
  for (const int &i : catQids) {
    trees[i] = Tree(hierarchiesMap[i]);
  }

  // GCP
  try {
    // 	1. Precalculate NCP for every qid value included in every cluster
    vector<long double> cncps =
        calculateNCPS(clusters, weights, allQids, numMetricsQids, trees);
    // 	2. Calculate GCP
    calculateGCP(clusters, dataset.size(), allQids, cncps);
  } catch (const char *e) {
    cout << e << endl;
    return -1;
  }

  // DM
  calculateDM(clusters, dataset.size(), K, L, T);

  // CAvg
  calculateCAVG(clusters, dataset.size(), K, L, T);

  // GenILoss
  try {
    calculateGenILoss(transpose(result), trees, allQids, catMetricsQids,
                      numMetricsQids, dataset.size());
  } catch (const char *e) {
    cout << e << endl;
    return -1;
  }

  return 0;
}
