/*! \file kmeans.h
    \brief archivo de cabecera principal del algoritmo K-Means.
*/

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

#define Epsilon 60

/*! Clase principal que contiene las funciones necesarias
    para ejecutar el algoritmo K-Means. */
class Kmeans {
private:
  //! Número de clusters a generar.
  int K;
  vector<Group> inicializeCentroids(vector<Record> &records);
  vector<double> centroidsDistances(const vector<Group> &groups,
                                    const vector<Record> &records);
  bool updateGroups(vector<Group> &groups, const vector<double> &newGroups,
                    vector<Record> &curRecords);
  bool convergency(vector<vector<double>> centroids,
                  vector<Group> updatedGroups, int epsilon);
public:
  Kmeans(int K);
  vector<Group> computeAll(vector<Record> &records, int epsilon);
};

vector<vector<vector<string>>> generalize(vector<Group> groups,
                                          vector<int> qids);
vector<Record> preprocessing(string file, vector<string> &headers,
                             vector<string> qidNames, vector<int> &qids);
#endif
