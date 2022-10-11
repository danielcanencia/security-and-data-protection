#include "hierarchy.h"


// Get idx of qid in table input headers
int getHierarchyIdx(const string qidName,
		    vector<string> headers) {
	string tmp;

	// Convert qid name to lowercase
	transform(qidName.begin(), qidName.end(),
		  inserter(tmp, tmp.end()),
    	  [](unsigned char x){ return tolower(x); });

	// Iterate through the headers and find the correct index
	std::vector<string>::iterator itr = find(headers.begin(),
						 headers.end(),
						 qidName);

	if (itr != headers.cend())
		return distance(headers.begin(),
				itr);

	return -1;
}

vector<vector<vector<string>>> transposeAndFormat(
		const vector<vector<vector<string>>>& hierarchies) {
	vector<vector<vector<string>>> res;

	for (const vector<vector<string>>& entry : hierarchies) {
		res.emplace_back(transpose(entry));
	}

	return res;
}


map<int, vector<vector<string>>> read_directory(
	fs::path const &directory,
	vector<vector<string>>& dataset,
	vector<int>& qids, vector<string>& headersVector,
	const int K) {

	// Locate csv input file and hierarchies directory
	string file;
	if (fs::exists(directory) && fs::is_directory(directory)) {
		auto const entry = fs::directory_iterator (directory).operator*();
		if (fs::is_regular_file(entry) && entry.path().extension()==".csv")
			file = entry.path().filename();
		else
			throw "Error, file format error";
	}
	const string filename = string(directory) + "/" + file;

	vector<string> hierarchies;
	const string hierarchies_dir = string(directory) +
				       "/"+ "hierarchies" + "/";
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

	// Skip headers and insert them into a variable
	string headers;
	getline(input1, headers);

	// Get a lowercase version of the headers
	string tmp;
	stringstream ss(headers);
	while(getline(ss, tmp, ';')){
		transform(tmp.begin(), tmp.end(), tmp.begin(),
    		  [](unsigned char x){ return tolower(x); });
    		headersVector.push_back(tmp);
	}


	// Read input table
	string line;
	for(; getline(input1, line);) {
		// Dont read empty lines
		if (line.length() == 0)
			continue;

		istringstream strm(move(line));
		vector<string> row;
		for(string val; getline(strm, val, ';');) {
			row.emplace_back(val);
		}
		dataset.emplace_back(row);
	}
	input1.close();


	// If the value of K introduced
	// by the user is greater than the
	// number of records, throw and execption
	// and inform the user
	if (K > (int)dataset.size()) {
		throw "The value of K entered is invalid.\n" 
		      "Must be equal or less than the "
		      "number of records present in the "
		      "input table.";
	}


	// Read hierarchy files
	string qidName;
	vector<string> qidNames;

	int idx;
	vector<vector<vector<string>>> res;
	for (const string& filename : hierarchies) {
		ifstream input2{filename};
		if(!input2.is_open()) {
			throw "Error reading file";
		}

		// Read first line: qid's hierarchy name
		getline(input2, qidName);
		qidNames.emplace_back(qidName);

		// Read hierarchy values
		vector<vector<string>> hierarchy;
		for(; getline(input2, line);) {
			// Dont read empty lines
			if (line.length() == 0)
				continue;
			istringstream strm(move(line));
			vector<string> row;
			for (string val; getline(strm, val, ';');) {
				row.emplace_back(val);
			}
			hierarchy.emplace_back(row);
		}
		input2.close();

		// Get hierarchy corresponding qid
		idx = getHierarchyIdx(qidName, headersVector);
		qids.emplace_back(idx);

		res.emplace_back(hierarchy);
	}


	// Get transposed hierchies
	const auto tRes = transposeAndFormat(res);


	// Map qid values and hierarchy sets
	map<int, vector<vector<string>>> hMap;
	for (size_t i=0; i < qids.size(); i++)
		hMap[qids[i]] = tRes[i];

	return hMap;
}


vector<vector<string>> transpose(
	const vector<vector<string>>& matrix) {

	const int rows=matrix.size();
	const int cols=matrix[0].size();

	string arr[cols][rows];

	// Transpose vector
	for (int i=0; i < rows; i++) {
		for (int j=0; j < cols; j++) {
			arr[j][i] = matrix[i][j];
		}
	}

	// Convert array to vector
	vector<vector<string>> res;
	for (int i=0; i < cols; i++) {
   		vector<string> aux;
   		for (int j=0; j < rows; j++) {
      			aux.emplace_back(arr[i][j]);
   		}
		res.emplace_back(aux);
	}

	return res;
}


vector<vector<int>> getPermutations(int r, const vector<int> data) {

        vector<vector<int>> permutations;
	int auxArray[data.size()];
        permute(data, permutations, data.size(), r, auxArray);
        return permutations;
}


void permute(const vector<int> data,
	     vector<vector<int>>& permutations,
	     int n, int r, int *aux, int rept, int idx) {

        if (idx==r) {
                vector<int> entry;
                for(int i=0; i < r; i++) {
                        entry.emplace_back(data[aux[i]]);

                }
                permutations.emplace_back(entry);
                return ;
        }

        for (int i=rept; (i < n) && (n - i + 1); i++) {
                aux[idx] = i;
                permute(data, permutations, n, r, aux, i+1, idx+1);
        }
}

