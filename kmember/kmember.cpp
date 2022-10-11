#include <iostream>
#include "kmember.h"

int main(int argc, char** argv) {


	if (argc != 2) {
		cout << "\nInvalid arguments.\n"
			"Use ./kmember [data directory]\n\n"
			"* Make sure your data directory meets the following structure:\n"
			"  (check dataset folder for an example)\n\n"
			" |-- [data directory]\n"
			"      |\n"
			"      |-- csv input file\n"
			"      |-- hierarchies\n"
			"           |\n"
			"           |-- hierarchy tables as csv files\n"
			"           |-- ....\n"
		<< endl;
		return -1;
	}

	string line;
	cout << "Insert K: ";
	getline(cin, line);
	const int K = stoi(line);

	// Read data file and hierarchy folders
	vector<string> headers;
	vector<int> qids;
	vector<vector<string>> dataset; //, transposedDataset;
	map<int, vector<vector<string>>> hierarchies_map;

	try {
		hierarchies_map = read_directory(fs::path(argv[1]),
					dataset, qids, headers, K);
		//transposedDataset = transpose(dataset);
	} catch (const char* e) {
		cout << e << endl; 
		return -1;
	}



	// Main algorithm
	vector<vector<string>> S = dataset;
	/*vector<string> r = getRandomRecord(S);

	// Functions not implemented
	while (S.size() >= K) {
		r = getFurthestRecord(r);
		S.erase(r);
		c = r;

		while(c.size() < K) {
			r = findBestRecord(S, c);
			S.erase(r);
			c = union(c, r);
		}
		res = union(res, c);
	}
	*/


	vector<int> result = {1, 2};
	vector<int> c = {3, 5};
	vector<int> res;

	set_union(result.begin(), result.end(),
		  c.begin(), c.end(),
		  inserter(res, res.end()));
	vector<int>::iterator it;
	for (it = res.begin(); it != res.end(); it++)
		cout << ' ' << *it;
	cout << '\n';


	// Check if all characters in a string are int digits
	string a = "577"; // 577** yields 577
	if(isdigit(a[0])) {
		cout << "It contains a number" << endl;
		cout << atoi(a.c_str()) << endl;
	}
	return 0;
}

