#include "hierarchy.h"


using namespace std;
//namespace fs = std::filesystem;


vector<string**> read_directory(fs::path const &directory, 
	 			vector<vector<string>>& dataset,
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
	for(; getline(input1, line);) {
		istringstream strm(move(line));
		vector<string> row;
		for(string val; getline(strm, val, ';');) {
			row.emplace_back(val);
		}
		dataset.emplace_back(row);
	}
	input1.close();

	// Read hierarchy files
	vector<vector<vector<string>>> res;
	vector<vector<string>> hierarchy;
	for (const string& filename : hierarchies) {
		ifstream input2{filename};
		if(!input2.is_open()) {
			throw "Error reading file";
		}

		for(; getline(input2, line);) {
			istringstream strm(move(line));
			vector<string> row;
			for (string val; getline(strm, val, ';');) {
				row.emplace_back(val);
			}
			//hierarchies_set.emplace_back(row);
			hierarchy.emplace_back(row);
		}
		input2.close();
		/*for (const vector<string>& entry : hierarchy) {
			for (const string& val : entry) {
				cout << val + ",";
			}
			cout << endl;
		}*/

		//hierarchies_set.emplace_back(transpose(hierarchy));
		res.emplace_back(hierarchy);
	}

	return transposeAndFormat(res);
}


string** transpose(const vector<vector<string>>& matrix) {
	//vector<vector<string>> res;
	int rows=matrix.size() + 1;
	int cols=matrix[0].size() + 1;
	string** array = new string*[rows];
	for(int i=0;i < rows;i++) {
    		array[i] = new string[cols];
	}

	int i=0;
	int j;
	for (const vector<string>& entry : matrix) {
		j = 0;
		for (const string& val : entry) {
		 	array[i][j] = val;
			j++;
		}
		i++;
	}
	return array;
}

vector<string**> transposeAndFormat(const vector<vector<vector<string>>>& hierarchies) {
	vector<string**> res;

	for (const vector<vector<string>>& entry : hierarchies) {
		//res[i] = transpose(entry);
		res.emplace_back(transpose(entry));
	}

	for (const auto x : res) {
		cout << x[0][0] << endl;
	}
	return res;
}

