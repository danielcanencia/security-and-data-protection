/*! \file hierarchy.h
    \brief Fichero de cabecera del archivo hierarchy.cpp.
*/

#ifndef _HIERARCHY_H
#define _HIERARCHY_H

#include <algorithm>  // funciones transform y find
#include <cctype>     // funciones transform y find
#include <filesystem>
#include <fstream>    // ifstream
#include <iostream>
#include <iterator>   // iterator
#include <map>
#include <string.h>   // strcasecmp
#include <vector>

using namespace std;
namespace fs = std::filesystem;

string getNumericRoot(const vector<vector<string>> dataset, const int qidIndex);
map<int, vector<vector<string>>>
readDirectory(fs::path const &directory, vector<vector<string>> &dataset,
              vector<string> &headers, vector<string> attQids,
              vector<string> confAtts, vector<int> &qids, vector<int> &atts,
              const bool transpose);
vector<vector<string>> transpose(const vector<vector<string>> &matrix);
vector<vector<int>> getPermutations(int len, const vector<int> nodeMax);
void permute(const vector<int> data, vector<vector<int>> &permutations, int n,
             int len, int *aux, int rept = 0, int idx = 0);
vector<int> getQidsHeaders(const vector<string> headers,
                           const vector<string> qids);

#endif
