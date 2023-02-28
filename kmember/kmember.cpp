#include "kmember.h"

map<int, vector<vector<string>>> evaluate(
		vector<vector<string>> dataset,
		map<int, vector<vector<string>>> hierarchies,
		vector<int> numQids, vector<int> catQids,
		const int confAtt, const int K, const int L,
		const vector<string> sensitiveValues,
		const int diversityPenalty,	const int diversity,
		int& count) {
	// Main algorithm
	vector<vector<string>> S = dataset;
	int r = randomRecord(S);
	map<int, vector<vector<string>>> res;
	vector<vector<string>> c;
	vector<string> aux = S[r];

	// 1st loop
	while ((int)S.size() >= K) {

		r = furthestRecord(aux, r, S,
			   hierarchies,
			   numQids, catQids);

		vector<vector<string>> c;
		c.emplace_back(S[r]);
		S.erase(S.begin() + r);

		while((int)c.size() < K) {
			r = find_best_record(S, c, hierarchies,
				numQids, catQids, confAtt, L,
				sensitiveValues, diversityPenalty,
				diversity);
			c.emplace_back(S[r]);
			S.erase(S.begin() + r);
		}

		aux = c.back();
		res[count] = c;
		count += 1;
	}

	// 2nd loop
	int idx;
	while (S.size() > 0) {
		r = randomRecord(S);
		idx = find_best_cluster(res, S[r],
			hierarchies,
			numQids, catQids);
		res[idx].emplace_back(S[r]);
		S.erase(S.begin() + r);
	}

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

	// Read input
	// Read qid names
	const int nqids = readNumberOfQids();
	vector<string> qidNames = readQidNames(nqids);
	// Conf Att
	vector<string> confAttNames(1, readConfidentialAttName());

	// Read data file and hierarchy folders
	vector<string> headers;
	vector<int> catQids, numQids, allQids;
	vector<int> confAtts;
	vector<vector<string>> hierarchies, dataset;
	map<int, vector<vector<string>>> hierarchiesMap;


	try {
		hierarchiesMap = read_directory(fs::path(argv[1]),
					dataset, headers, qidNames, confAttNames,
					catQids, confAtts, false);

		if (catQids.size() == 0) {
			cout << endl << "******************" << endl; 
			cout << "An error occured.\nCheck the qid "
				"names entered exists. They should be "
				"referenced\nin their respectives "
				"hierarchy files." << endl << endl;
			return -1;
		}
		// Compare headers and qids
		allQids = getQidsHeaders(headers, qidNames);

		set_difference(allQids.begin(), allQids.end(),
			catQids.begin(), catQids.end(),
        		inserter(numQids, numQids.begin())); 

	} catch (const char* e) {
		cout << e << endl; 
		return -1;
	}

	// Read Parameters
	const int K = readParameter("k-anonymity", "K", dataset.size());
	if (K == -1) {
		cout << "Kmember needs parameter K" << endl;
		return 1;
	}
	const int L = readParameter("l-diversity", "L", dataset.size());
	// Diversity Penalty, select metric used
	const int confAtt = confAtts[0];
	const int diversityPenalty = readDiversityPenalty();
	const int diversity = readDiversity();
	vector<vector<string>> transposedDataset = transpose(dataset);
	// Read sensitive values for sensitive diversity metric
	vector<string> sensitiveValues;
	if (diversity == 1)
		sensitiveValues = readSensitiveValues(transposedDataset[confAtt]);


	// Measure Execution Time
	auto start = chrono::high_resolution_clock::now();
	// *********************************
	// Main algorithm
	int count = 0;
	map<int, vector<vector<string>>> res = evaluate(dataset, hierarchiesMap,
													numQids, catQids, confAtt,
													K, L, sensitiveValues,
													diversityPenalty, diversity, count);
	// *********************************
	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
	cout << endl << "===> Kmember Execution Time: ";
	cout << duration.count() << " microseconds" << endl;

	cout << "===> Number of clusters: ";
	cout << count << endl;


	// Write Anonymized Clusters
	string directory = argv[1];
	if (directory.back() != '/')
		directory += "/";
	directory += to_string(K) + "K";
	if (L != -1)
		directory += "_" + to_string(L) + "L";
	for (int i=0; i < count; i++) {
		writeAnonymizedTable(
			fs::path(directory),
			headers, res[i], K, -1, -1,
			"cluster" + to_string(i+1)
		);
	}

	return 0;
}

