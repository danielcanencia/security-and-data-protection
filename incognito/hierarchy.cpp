#include "hierarchy.h"


vector<vector<vector<string>>> read_directory(fs::path const &directory, 
	 				      vector<vector<string>>& dataset,
					      vector<int>& qids,
			   		      string& headers) {

	// Locate csv input file and hierarchies directory
	string file;
	if (fs::exists(directory) && fs::is_directory(directory)) {
		auto const entry = fs::directory_iterator (directory).operator*();
		if (fs::is_regular_file(entry) && entry.path().extension()==".csv")
			file = entry.path().filename();
		else
			throw "Error, file format error";
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

	// Skip headers and insert them into a variable
	string line;
	getline(input1, headers);
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

	// Read hierarchy files
	vector<vector<vector<string>>> res;
	string qidName;
	vector<string> qidNames;
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

		res.emplace_back(hierarchy);
	}

	// Get a lowercase version of the headers
	string tmp;
	stringstream ss(headers);
	vector<string> headersVector;
	while(getline(ss, tmp, ';')){
		transform(tmp.begin(), tmp.end(), tmp.begin(),
    		  [](unsigned char x){ return tolower(x); });
    		headersVector.push_back(tmp);
	}

	// Convert qids vector to an int vector 
	// (denotes qid index/column in dataset)
	for (const string& entry : qidNames) {

		// Convert qid name to lowercase
		transform(entry.begin(), entry.end(), inserter(tmp, tmp.end()), 
    		  [](unsigned char x){ return tolower(x); });


		// Iterate through the headers and find the correct index
		std::vector<string>::iterator itr = find(headersVector.begin(),
							 headersVector.end(),
							 entry); 
		if (itr != headersVector.cend())
			qids.emplace_back(distance(headersVector.begin(),
					  itr));
	}
	


	// Return tansposed hierchies
	return transposeAndFormat(res);
}


vector<vector<string>> transpose(const vector<vector<string>>& matrix) {
	int rows=matrix.size();
	int cols=matrix[0].size();

	// Convert array to vector
	vector<vector<string>> res;
	for (int i=0; i < rows; i++) {
   		vector<string> aux;
   		for (int j=0; j < cols; j++) {
      			aux.emplace_back(matrix[i][j]);
   		}
		res.emplace_back(aux);
	}

	return res;
}

vector<vector<vector<string>>> transposeAndFormat(
		const vector<vector<vector<string>>>& hierarchies) {
	vector<vector<vector<string>>> res;

	for (const vector<vector<string>>& entry : hierarchies) {
		res.emplace_back(transpose(entry));
	}

	return res;
}


vector<vector<int>> getPermutations(int r, vector<int> nodeMax) {

        //Permutamos, obteniendo los valores de los nodos del grafo
        vector<vector<int>> permutations;
        permute(nodeMax, permutations, nodeMax.size(), r);
        return permutations;
}


void permute(const vector<int> data,
                   vector<vector<int>>& permutations,
                   int n, int r, int rept, int idx, int aux[]) {
        if (aux==NULL) {
                aux = new int[r];
        }

        if (idx==r) {
                vector<int> entry;
                for(int i=0; i < r; i++) {
                        entry.emplace_back(data[aux[i]]);

                }
                permutations.emplace_back(entry);
                return ;
        }

        for (int i=rept; i < n; i++) {
                aux[idx] = i;
                permute(data, permutations, n, r, i, idx+1, aux);
        }
}


