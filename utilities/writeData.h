/*! \file writeData.h
    \brief Fichero de cabecera del archivo writeData.cpp.
*/

#ifndef _WRITEDATA_H
#define _WRITEDATA_H

#include <filesystem>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

namespace fs = std::filesystem;
using namespace std;

void writeStrings(ofstream &fp, vector<vector<string>> dataset,
                  string delimiter);
void writeAnonymizedTable(const string inputFname, const vector<string> headers,
                          const vector<vector<string>> dataset, const int K,
                          const int L, const long double T,
                          const string exFileName = "", const bool verbose = true);

#endif
