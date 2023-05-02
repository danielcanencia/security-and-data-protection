#include "kmeans.h"

Kmeans::Kmeans(int K) { this->K = K; }

vector<Group> Kmeans::inicializeCentroids(vector<Record> &records) {
  vector<Group> groups;
  vector<int> aux;

  srand(time(NULL));

  for (int i = 0; i < K; i++) {
    while (1) {
      int random = rand() % records.size();
      // Check if random value is repeated
      if (find(aux.begin(), aux.end(), random) == aux.end()) {
        // Inicialize centroids and assign them a cluster
        aux.emplace_back(random);
        records.at(random).setGroup(i);
        Group group(i, records.at(random));
        groups.emplace_back(group);
        break;
      }
    }
  }
  return groups;
}

vector<double> Kmeans::centroidsDistances(const vector<Group> &groups,
                                          const vector<Record> &records) {
  vector<double> res;
  int gindex;
  double min, ed;
  vector<vector<double>> centroids;
  for (const Group &group : groups) {
    centroids.emplace_back(group.getCentroid());
  }

  // Calculate euclidian distance between records and all centroids
  for (const Record &record : records) {
    min = record.euclideanDistance(centroids[0]);
    gindex = 0;

    for (int i = 1; i < (int)centroids.size(); i++) {
      ed = record.euclideanDistance(centroids[i]);
      if (ed < min) {
        min = ed;
        gindex = i;
      }
    }
    // Array Representing Records & its Clusters
    res.emplace_back(gindex);
  }

  return res;
}

bool Kmeans::updateGroups(vector<Group> &groups,
                          const vector<double> &newGroups,
                          vector<Record> &curRecords) {
  bool end = 1;
  int newGroup, curGroup;

  for (int i = 0; i < (int)newGroups.size(); i++) {
    newGroup = newGroups[i];
    curGroup = curRecords[i].getGindex();
    if (newGroup != curGroup) {
      if (curGroup != -1) {
        groups[curGroup].removeRecord(curRecords[i]);
      }
      curRecords[i].setGroup(newGroup);
      groups[newGroup].addRecord(curRecords[i]);
      end = 0;
    }
  }
  return end;
}

vector<Group> Kmeans::computeAll(vector<Record> &records) {

  // 1. Inicialize centroids
  vector<Group> groups = inicializeCentroids(records);

  // Loop
  int iterations = 0;
  while (iterations != MAX_ITERATIONS) {
    iterations++;

    // 2. Euclidean Distance And Group Classification
    vector<double> newGroups = centroidsDistances(groups, records);
    // 3. Update all record's group
    bool end = updateGroups(groups, newGroups, records);
    if (end)
      break;

    // 4. Recalculate the centroid for each group/cluster
    for (Group &group : groups) {
      group.recalculateCentroid();
    }
  }

  return groups;
}

vector<vector<vector<string>>> generalize(vector<Group> groups,
                                          vector<int> qids) {
  vector<vector<vector<string>>> clusters;

  // Global recoding
  for (auto &group : groups) {
    clusters.emplace_back(group.generalize(qids));
  }

  return clusters;
}

vector<Record> preprocessing(string file, vector<string> &headers,
                             vector<string> qidNames, vector<int> &qids) {
  // Read CSV Input files
  ifstream input{file};
  if (!input.is_open()) {
    throw "Error reading file";
  }

  // Parse CSV Input file
  vector<Record> values;
  int index = 0;
  string line, headersAux;
  try {
    // Headers && Qids
    getline(input, headersAux);
    string tmp;
    stringstream ss(headersAux);
    while (getline(ss, tmp, ';')) {
      headers.push_back(tmp);
    }
    headers.back().pop_back();

    // Get qids indexes
    for (size_t i = 0; i < qidNames.size(); i++) {
      auto it = find(headers.begin(), headers.end(), qidNames[i]);
      if (it != headers.end())
        qids.emplace_back(it - headers.begin());
      else if (headers.back() == qidNames[i])
        qids.emplace_back(headers.size() - 1);
    }

    // Records
    for (; getline(input, line);) {
      Record record = Record(index);
      istringstream strm(move(line));

      for (string val; getline(strm, val, ';');) {
        record.pushBackValue(stod(val));
      }
      values.push_back(move(record));
      index++;
    }
  } catch (const exception &e) {
    input.close();
    // It throws an expetion because of stod(val) conversion to double
    return values;
  }

  input.close();
  return values;
}

int main(int argc, char **argv) {

  // Arguments
  if (argc != 3) {
    cout << "Invalid arguments. Use ./kmeans [filename] [k]" << endl;
    return -1;
  }
  // Input Filename
  string filename{argv[1]};
  // Check File format
  if (filename.substr(filename.size() - 4, filename.size()) != ".csv") {
    cout << "Please, use a csv input file" << endl;
    return -1;
  }

  // K
  int K = atoi(argv[2]);
  // Read input
  // Read qid names
  const int nqids = readNumberOfQids();
  vector<string> qidNames = readQidNames(nqids);

  // Data Preprocessing
  vector<Record> records;
  vector<int> qids;
  vector<string> headers;

  try {
    records = preprocessing(filename, headers, qidNames, qids);
    if (qids.size() < qidNames.size()) {
      cout << endl << "******************" << endl;
      cout << "An error occured.\nCheck the qid "
              "names entered exists. They should be "
              "referenced\nin their respectives "
              "hierarchy files."
           << endl
           << endl;
      return -1;
    }
    sort(qids.begin(), qids.end());
  } catch (exception &e) {
    cout << "Error reading file" << endl;
    return -1;
  }

  // Read Weights
  vector<double> weights = readWeights(nqids, qidNames);

  cout << "* K-Means algorithm is being run..." << endl;
  // Measure Execution Time
  auto start = chrono::high_resolution_clock::now();
  // Especify the number of clusters/groups to use
  Kmeans kmeans(K);
  // Run the algorithm
  vector<Group> groups = kmeans.computeAll(records);
  // Generalize clusters
  vector<vector<vector<string>>> clusters = generalize(groups, qids);

  // Execution Time
  auto stop = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
  cout << endl << "===> K-Means Execution Time: ";
  cout << duration.count() << " microseconds" << endl;
  cout << "===> Number of clusters: ";
  cout << groups.size() << endl;
  cout << "* K-Means algorithm finished. A csv file will be generated...."
       << endl;

  // Write resulting clusters to file
  string directory = filename;
  directory = directory.substr(directory.find('/') + 1, directory.size());
  directory = directory.substr(0, directory.find_last_of("."));
  directory.insert(0, "outputs/");
  directory += "_K" + to_string(K) + "/clusters/";
  fs::create_directories(directory);
  cout << "* Writing output to directory: " + directory << endl;

  // Create matrix from clusters
  vector<vector<string>> result;
  for (size_t i = 0; i < clusters.size(); i++) {
    result.insert(result.begin(), clusters[i].begin(), clusters[i].end());
    writeAnonymizedTable(fs::path(directory), headers, clusters[i], K, -1, -1,
                         "cluster" + to_string(i + 1), false);
  }

  // METRICS
  // GCP
  try {
    // 	1. Precalculate NCP for every qid value included in every cluster
    vector<long double> cncps =
        calculateNCPS(clusters, weights, qids, qids, {});
    // 	2. Calculate GCP
    calculateGCP(clusters, records.size(), qids, cncps);
  } catch (const char *e) {
    cout << e << endl;
    return -1;
  }

  // For DM and CAvg metrics we should calculate K
  // based on clusters minimum size
  cout << "\t===> Evaluating K value to be used on DM and CAvg metrics"
          " based on clusters sizes"
       << endl;
  int KValue =
      (*min_element(clusters.begin(), clusters.end(),
                    [](vector<vector<string>> a, vector<vector<string>> b) {
                      return a.size() < b.size();
                    }))
          .size();
  cout << "\t===> K Value: ";
  cout << KValue << endl;

  // DM
  calculateDM(clusters, records.size(), KValue, -1, -1);
  // CAvg
  calculateCAVG(clusters, records.size(), KValue, -1, -1);

  // Mirar valor GenILoss
  // GenILoss
  try {
    calculateGenILoss(transpose(result), {}, qids, {}, qids, records.size());
  } catch (const char *e) {
    cout << e << endl;
    return -1;
  }

  return 0;
}
