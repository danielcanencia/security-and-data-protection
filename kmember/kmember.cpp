#include <iostream>
#include "kmember.h"

vector<string> sunion(vector<string>& s1, vector<string>& s2) {
	vector<string> res;

	set_union(s1.begin(), s1.end(),
		  s2.begin(), s2.end(),
		  inserter(res, res.end()));
	return res;
}

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
	vector<int> catQids, numQids, allQids;
	vector<vector<string>> hierarchies, dataset,
			       transposedDataset;
	map<int, vector<vector<string>>> hierarchies_map;

	// Read Input !!!
	vector<string> qidNames = {"Age", "Country", "Occupation"};

	try {
		hierarchies_map = read_directory(fs::path(argv[1]),
					dataset, headers, K, qidNames,
					catQids);

		// Compare headers and qids
		allQids = getQidsHeaders(headers, qidNames);
		set_difference(allQids.begin(), allQids.end(),
			catQids.begin(), catQids.end(),
        		inserter(numQids, numQids.begin())); 
		/*
		cout << "Num Qids: " << endl;
		for (const auto& entry : numQids)
			cout << to_string(entry) + ", ";
		cout << endl;

		cout << "Cat Quids: " << endl;
		for (const auto& entry : catQids)
			cout << to_string(entry) + ", ";
		cout << endl;

		for (const auto& qid : allQids)
			cout << to_string(qid) + ", ";
		cout << endl;
		*/

		// Get dataset containint just
		// quasi-identifier attributes
		transposedDataset = transpose(dataset);
		for (size_t i=0; i <= transposedDataset.size(); i++) {
			if (find(allQids.begin(), allQids.end(), i)
			 	== allQids.end()) {
				transposedDataset.erase(
					transposedDataset.begin() + i);
			}
		}
		dataset = transpose(transposedDataset);

		/*
		for (const auto& entry : dataset) {
			for (const auto& v : entry) {
				cout << v + ", ";
			}
			cout << endl;
		}*/
		cout << endl;

		//for (const auto& entry : hierarchies_set)
		//	hierarchies.emplace_back(entry);

	} catch (const char* e) {
		cout << e << endl; 
		return -1;
	}

	// Main algorithm
	vector<vector<string>> S = dataset;
	int r = randomRecord(S);
	cout << r << endl;


	map<int, vector<vector<string>>> res;
	vector<vector<string>> c;
	int count = 0;
	while ((int)S.size() >= K) {

		r = furthestRecord(S[r], r, S,
				   hierarchies_map,
				   numQids, catQids);
		cout << to_string(r) + " < " + to_string(S.size()) << endl;
		vector<vector<string>> c;
		c.emplace_back(S[r]);

		S.erase(S.begin() + r);

		while((int)c.size() < K) {
			//r = 1;
			r = find_best_record(S, c);
			c.emplace_back(S[r]);
			S.erase(S.begin() + r);
		}

		//res = sunion(res, c);
		res[count] = c;
		count += 1;
	}

	/*
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

	*/
	// Check if all characters in a string are int digits
	/*string a = "577"; // 577** yields 577
	if(isdigit(a[0])) {
		cout << "It contains a number" << endl;
		cout << atoi(a.c_str()) << endl;
	}*/



	/*
	string str ="577.976766739.9", value;
	int maxSize = 0;
	int flag = 1, index = 0;

	maxSize = str.size();
	value = str;
	while(flag) {
		value = value.substr(0);
		//maxSize = b.size();
		if (isdigit(value[index])) {
			auto aux = atoi(value.c_str());

			//result += atoi(b.cstr());

			if (value[to_string(aux).size()] == '.') {
				index += to_string(aux).size() + 1;	
				if (index >= maxSize)
					flag = 0;
			}
		}
		else {
			cout << "It is a string" << endl;
		}
	}

	cout << value << endl;*/
	return 0;
}

