#include "analysis.h"

long double calculateCatNCP(const vector<string> atts, Tree tree) {
  // Calculate NCP fot qid values
  long double card = tree.getNCP(atts);
  if (card == 1)
    return 0;

  int leaves = tree.getNumLeaves();
  if (leaves != 0)
    card /= leaves;

  return (long double)card;
}

long double calculateNumNCP(const vector<string> atts, long double globalMax,
                            long double globalMin) {

  long double min, max, splitMin, splitMax;

  string value = atts[0];
  int index = atts[0].find('~');
  try {
    if (index != (int)string::npos) {
      min = stod(value.substr(0, index));
      max = stod(value.substr(index + 1, value.size()));
    } else
      min = max = stod(value);
  } catch (const exception &e) {
    cout << e.what() << endl;
    return -1;
  }

  for (size_t i = 0; i < atts.size(); i++) {
    value = atts[i];
    index = atts[i].find('~');
    if (index != (int)string::npos) {
      splitMin = stod(value.substr(0, index));
      splitMax = stod(value.substr(index + 1, value.size()));
    } else
      splitMin = splitMax = stod(value);

    if (splitMin < min)
      min = splitMin;
    if (splitMax > max)
      max = splitMax;
  }

  if (globalMax - globalMin == 0)
    return (max - min);

  return (max - min) / (globalMax - globalMin);
}

bool sortMaxSplit(string a, string b) {
  long double max1, max2;
  int i1 = a.find('~');
  int i2 = b.find('~');
  if (i1 != (int)string::npos)
    max1 = stod(a.substr(i1 + 1, a.size()));
  else
    max1 = stod(a);

  if (i2 != (int)string::npos)
    max2 = stod(b.substr(i2 + 1, b.size()));
  else
    max2 = stod(b);

  return max1 < max2;
}

bool sortMinSplit(string a, string b) {
  long double min1, min2;
  int i1 = a.find('~');
  int i2 = b.find('~');
  if (i1 != (int)string::npos)
    min1 = stod(a.substr(0, i1));
  else
    min1 = stod(a);

  if (i2 != (int)string::npos)
    min2 = stod(b.substr(0, i2));
  else
    min2 = stod(b);

  return min1 > min2;
}

vector<long double> calculateNCPS(vector<vector<vector<string>>> clusters,
                                  vector<double> weights, vector<int> allQids,
                                  vector<int> numQids, map<int, Tree> trees) {
  vector<long double> cncps;

  // Calculate numerical attibute global maximum and minimum values
  // in all clusters
  map<int, tuple<long double, long double>> numNumerator;
  for (const auto &qid : numQids) {
    long double max, min, auxMax, auxMin;
    string aux;
    max = min = -1;

    for (const auto &cluster : clusters) {
      vector<vector<string>> tcluster = transpose(cluster);

      try {
        aux =
            *max_element(tcluster[qid].begin(), tcluster[qid].end(),
                         [](string a, string b) { return sortMaxSplit(a, b); });
      } catch (const exception &e) {
        throw "\nError, some categorical attribute "
              " can't be treated as numerical\n";
      }

      int index = aux.find('~');
      if (index != (int)string::npos)
        auxMax = stod(aux.substr(index + 1, aux.size()));
      else
        auxMax = stod(aux);

      if (auxMax > max || max == -1)
        max = auxMax;

      aux = *min_element(tcluster[qid].begin(), tcluster[qid].end(),
                         [](string a, string b) { return sortMinSplit(a, b); });
      index = aux.find('~');
      if (index != (int)string::npos) {
        auxMin = stod(aux.substr(0, index));
      } else
        auxMin = stod(aux);
      if (auxMin < min || min == -1)
        min = auxMin;
    }

    numNumerator[qid] = make_tuple(max, min);
  }

  // Calculate NCP
  for (const auto &cluster : clusters) {
    vector<vector<string>> tcluster = transpose(cluster);

    // Qids Att Values
    long double ncp = 0.0;

    for (size_t i = 0; i < allQids.size(); i++) {
      if (find(numQids.begin(), numQids.end(), allQids[i]) != numQids.end()) {
        // Numeric attributes
        ncp += calculateNumNCP(tcluster[allQids[i]],
                               get<0>(numNumerator[allQids[i]]),
                               get<1>(numNumerator[allQids[i]])) *
               weights[i];
      } else {
        // Categorical attributes
        long double aux =
            calculateCatNCP(tcluster[allQids[i]], trees[allQids[i]]);
        if (aux == -1)
          continue;
        ncp += aux * weights[i];
      }
    }
    ncp *= allQids.size();
    cncps.emplace_back(ncp);
  }

  return cncps;
}

void calculateGCP(vector<vector<vector<string>>> clusters, int numRecords,
                  vector<int> qids, vector<long double> cncps) {

  long double gcp = 0;
  for (size_t i = 0; i < clusters.size(); i++) {
    gcp += (long double)(clusters[i].size() * cncps[i]);
  }
  gcp /= (qids.size() * numRecords);

  // Show only 3 decimals
  cout << "\t* GCP: ";
  cout << fixed << setprecision(3) << gcp << endl;
}
