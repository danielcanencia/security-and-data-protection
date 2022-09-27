#include <iostream>
#include <vector>
#include <fstream>	// ifstream
#include <filesystem>   // filesystem utilities

using namespace std;
namespace fs = std::filesystem;

vector<vector<string>> read_directory(fs::path const &directory, 
				      vector<vector<string>>& dataset,
				      vector<vector<string>>& hierarchies_set,
			              string &headers) {

	// Locate csv input file and hierarchies directory
	string file;
	if (fs::exists(directory) && fs::is_directory(directory)) {
		auto const entry = fs::directory_iterator (directory).operator*();
		if (fs::is_regular_file(entry) && entry.path().extension()==".csv")
			file = entry.path().filename();
	}
	string filename = string(directory) + "/" + file;

	vector<string> hierarchies;
	string hierarchies_dir = string(directory) + "/" + "hierarchies" + "/";
	for (auto const& entry : fs::recursive_directory_iterator(hierarchies_dir)) {
		if (fs::is_directory(entry))
			continue;
		hierarchies.emplace_back(hierarchies_dir + entry.path().filename().c_str()); 
	}

	// Read csv dataset 
	ifstream input1{filename};
	if(!input1.is_open()) {
		throw "Error reading file";
	}

	string line;
	// Skip headers and insert them into a variable
	getline(input1, headers);
	for(line; getline(input1, line);) {
		istringstream strm(move(line));
		vector<string> row;
		for(string val; getline(strm, val, ';');) {
			row.emplace_back(val);
		}
		dataset.emplace_back(row);
	}
	input1.close();

	// Read hierarchy files
	for (const string& filename : hierarchies) {
		ifstream input2{filename};
		if(!input2.is_open()) {
			throw "Error reading file";
		}

		for(line; getline(input2, line);) {
			istringstream strm(move(line));
			vector<string> row;
			for (string val; getline(strm, val, ';');) {
				row.emplace_back(val);
			}
			hierarchies_set.emplace_back(row);
		}	
		input2.close();
	}

	return dataset;
}



int main(int argc, char** argv) {

	if (argc != 2) {
		cout << "Inentryid arguments. "
			"Use ./incognito [data directory] [k]"
		<< endl;
		return -1;
	}

	// Read csv data file
	string headers;
	vector<vector<string>> dataset;
	vector<vector<string>> hierarchies_set;
	read_directory(fs::path(argv[1]), dataset, hierarchies_set, headers);


	return 0;
}
