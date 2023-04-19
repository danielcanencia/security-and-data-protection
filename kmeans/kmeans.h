#ifndef _KMEANS_H
#define _KMEANS_H

#include "../utilities/analysis.h"
#include "../utilities/hierarchy.h"
#include "../utilities/input.h"
#include "../utilities/metrics.h"
#include "../utilities/writeData.h"
#include "group.h"
#include "record.h"
#include <algorithm> // find
#include <chrono>
#include <ctime>      // time
#include <filesystem> // fs
#include <fstream>    // ofstream
#include <iostream>   // cout
#include <string>
#include <tuple>
#include <vector>

using namespace std;
namespace fs = std::filesystem;

#define MAX_ITERATIONS 30

class Kmeans {
private:
  int K;
  vector<Group> inicializeCentroids(vector<Record> &records);
  vector<double> centroidsDistances(const vector<Group> &groups,
                                    const vector<Record> &records);
  bool updateGroups(vector<Group> &groups, const vector<double> &newGroups,
                    vector<Record> &curRecords);

public:
  Kmeans(int K);
  vector<Group> computeAll(vector<Record> &records);
};

vector<vector<vector<string>>> generalize(vector<Group> groups,
                                          vector<int> qids);
vector<Record> preprocessing(string file, vector<string> &headers,
                             vector<string> qidNames, vector<int> &qids);
#endif
