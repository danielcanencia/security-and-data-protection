#ifndef _KMEANS_H
#define _KMEANS_H

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
  void writeOutput(vector<Group> groups, string filename, string headers);
};

vector<Record> preprocessing(string file, string &headers);

#endif
