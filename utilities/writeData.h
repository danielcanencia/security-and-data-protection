#ifndef _WRITEDATA_H
#define _WRITEDATA_H

#include <iostream>
#include <vector>
#include <numeric>
#include <fstream>
#include <filesystem>


namespace fs = std::filesystem;
using namespace std;


void writeStrings(ofstream &fp, vector<vector<string>> dataset,
               	  string delimiter);
void writeAnonymizedTable(const string inputFname,
			  	  const vector<string> headers,
                  const vector<vector<string>> dataset,
			  	  const int K, const string prefix = "");
void writeAnonymizedTable(const string inputFname,
                          const vector<string> headers,
                          const vector<vector<string>> dataset,
                          const int K, const int L, const int P,
                          const string prefix = "");


#endif

