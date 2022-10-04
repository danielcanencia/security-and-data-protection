#ifndef _HIERARCHY_H
#define _HIERARCHY_H


#include <iostream>
#include <vector>
#include <fstream>	// ifstream
#include <filesystem>   // filesystem utilities

using namespace std;
namespace fs = std::filesystem;

vector<string**> read_directory(fs::path const &directory,
				vector<vector<string>>& dataset,
		    	 	string &headers);
vector<string**> transposeAndFormat(const vector<vector<vector<string>>>& hierarchies);

#endif


