#include "input.h"

const int readNumberOfQids() {
  string question = "Number of qids: ";
  cout << question;
  int nqids;

  while (1) {
    cin >> nqids;
    if (cin.fail()) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Error, enter a valid number." << endl;
      cout << question;
      continue;
    }
    break;
  }

  return nqids;
}

vector<string> readQidNames(const int nqids) {
  set<string> qid_set;
  for (int i = 0; i < nqids; i++) {
    cout << "Enter qid name (" << i << "): ";
    string qidName;
    cin >> qidName;
    qid_set.insert(qidName);
  }
  if ((int)qid_set.size() != nqids) {
    throw "Input Error: Qids should be unique. "
          "Check if you repeated some of them.";
  }

  return vector<string>(qid_set.begin(), qid_set.end());
}

vector<string> readConfidentialAttNames() {
  cout << "Confidential attributes: " << endl;
  // Read number of confidential attributes
  int nAtts;
  while (1) {
    cout << "\tNumber of confidential attributes: ";
    cin >> nAtts;
    if (cin.fail()) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "\tError, enter a valid number." << endl;
      continue;
    }
    break;
  }

  // Read confidential attributes names
  set<string> att_set;
  for (int i = 0; i < nAtts; i++) {
    cout << "\tEnter att name (" << i << "): ";
    string attName;
    cin >> attName;
    att_set.insert(attName);
  }
  if ((int)att_set.size() != nAtts) {
    const string error = "Input Error: Confidential Attributes should be "
                         "unique. Check if you repeated some of them.";
    cout << error << endl;
    return vector<string>(1, error);
  }

  return vector<string>(att_set.begin(), att_set.end());
}

vector<double> readWeights(const int nqids, vector<string> qidNames) {
  vector<double> weights;
  cout << "Do you want to use custom weights? "
          "(will only be used on gcp metric) [Y(y)/N(n)]: ";
  char answer;
  cin >> answer;
  bool keep = false;
  while (!keep) {
    switch (answer) {
    case 'Y':
    case 'y':
      for (int i = 0; i < nqids;) {
        cout << "Weight for qid " << i << ": ";
        float weight;
        cin >> weight;
        if (cin.fail()) {
          cin.clear();
          cin.ignore();
          cout << "Error, enter a valid number." << endl;
        } else {
          weights.emplace_back(weight);
          i++;
        }
      }
      if (accumulate(weights.begin(), weights.end(), (float)0) != (float)1) {
        cout << "Input Error: Weights must sum 1." << endl;
        weights.clear();
        continue;
      }
      keep = true;
      break;
    case 'N':
    case 'n':
      weights = vector<double>(qidNames.size(), 1.0 / qidNames.size());
      keep = true;
      break;
    default:
      cout << "Do you want to use weights ";
      cout << "(will only be used on metrics) ";
      cout << "[Y(y)/N(n)]: ";
      cin >> answer;
    }
  }

  return weights;
}

tuple<vector<int>, vector<int>> readMetricsQids(vector<int> numQids,
                                                vector<int> catQids,
                                                vector<string> headers) {
  vector<int> numMetricsQids = numQids, catMetricsQids;
  string question = "Do you want to treat some hierarchical attributes as "
                    "numerical? (will only be used on metrics) [Y(y)/N(n)]: ";
  cout << question;
  char answer;
  cin >> answer;
  bool keep;
  keep = true;
  string aux;
  int number;
  while (keep) {
    switch (answer) {
    case 'Y':
    case 'y':
      cout << "Enter number printed between brackets: " << endl;
      while (catQids.size() > 0) {
        cout << "\t";
        for (size_t i = 0; i < catQids.size(); i++)
          cout << headers[catQids[i]] + "(" + to_string(i) + ") ";
        cout << endl;
        cout << "\t\t[enter q to quit] >> ";
        cin >> aux;

        if (aux == "q")
          break;

        try {
          number = stoi(aux);
          if (number < 0 || number >= (int)catQids.size()) {
            cin.clear();
            cout << "\tError: Number not present. Try again." << endl;
          } else {
            numMetricsQids.emplace_back(catQids[number]);
            catQids.erase(catQids.begin() + number);
          }
        } catch (...) {
          cin.clear();
          cout << "\tError, enter a number or q." << endl;
        }
      }
      keep = false;
      break;
    case 'N':
    case 'n':
      keep = false;
      break;
    default:
      cout << question;
      cin >> answer;
    }
  }
  for (const auto &qid : catQids) {
    if (find(numMetricsQids.begin(), numMetricsQids.end(), qid) ==
        numMetricsQids.end())
      catMetricsQids.emplace_back(qid);
  }

  tuple<vector<int>, vector<int>> tuple(numMetricsQids, catMetricsQids);
  return tuple;
}

long double readParameter(const string privacyDef, const string parameter,
                          const int datasetSize) {
  // Default value. Means privacy definition won't be checked
  // (k-anonimity, l-diversity, t-closeness, etc)
  long double param = -1;

  string question =
      "Do yo want to anonymize preserving " + privacyDef + "? [Y(y)/N(n)]: ";
  cout << question;
  char answer;
  cin >> answer;
  bool keep = true;
  while (keep) {
    switch (answer) {
    case 'Y':
    case 'y':
      while (1) {
        cout << "Insert " + parameter + ": ";
        cin >> param;
        if (cin.fail()) {
          cin.clear();
          cin.ignore();
          cout << "Error reading number, try again." << endl;
          continue;
        }
        if (param > datasetSize) {
          cin.clear();
          cin.ignore();
          cout << parameter +
                      " value should be equal or less tham dataset size.";
          cout << endl;
          continue;
        }
        break;
      }
      keep = false;
      break;
    case 'N':
    case 'n':
      keep = false;
      break;
    default:
      cout << "Error: enter a valid value" << endl;
      cout << question;
      cin >> answer;
    }
  }

  return param;
}

bool readParameters(const int datasetSize, const int confAtts, int &K, int &L,
                    long double &T) {
  // K (K-anonimity)
  K = readParameter("k-anonymity", "K", datasetSize);

  // L (l-diversity)
  L = readParameter("l-diversity", "L", datasetSize);

  // T (t-closeness)
  T = readParameter("t-closeness", "T", datasetSize);

  // Check l-diversity and t-closeness errors
  if ((L == -1 || T == -1.0) && confAtts == 0) {
    cout << endl;
    cout << "An error occured.\nIf l-diversity or t-closeness "
            "are used, there should exists, at least, one "
            "confidential attribute."
         << endl
         << endl;
    return 1;
  }

  if (K == -1 && L == -1 && T == -1.0) {
    cout << "Error, some privacy technique should be used." << endl;
    readParameters(datasetSize, confAtts, K, L, T);
  }

  return 0;
}
