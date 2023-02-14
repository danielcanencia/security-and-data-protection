#include "input.h"

const int readK() {
	string line;
	cout << "Insert K: ";
	getline(cin, line);
	return stoi(line);
}

const int readNumberOfQids() {
	cout << "Number of qids: ";
    string line;
	getline(cin, line);

    return stoi(line);
}

vector<string> readQidNames(const int nqids) {
    set<string> qid_set;
	for (int i=0; i < nqids; i++) {
		cout << "Enter qid " << i << ": ";
		string qid;
		getline(cin, qid);
		qid_set.insert(qid);
	}
	if ((int)qid_set.size() != nqids) {
        const string error = "Input Error: Qids should be unique."
		                     "Check if you repeated some of them";
		cout << "Input Error: Qids should be unique.";
		cout << "Check if you repeated some of them" << endl; 
		return vector<string>(1, error);
	}
	vector<string> qidNames(qid_set.begin(), qid_set.end());

    return qidNames;
}

vector<double> readWeights(const int nqids, vector<string> qidNames) {
	vector<double> weights;
	cout << "Do you want to use custom weights "
			"(will only be used on gcp metric) [Y(y)/N(n)]: ";
	char answer;
	cin >> answer;
	bool keep = false;
	while(!keep) {
		switch(answer) {
			case 'Y':
			case 'y':
				for (int i=0; i < nqids; i++) {
					cout << "Weight for qid " << i << ": ";
					float weight;
					cin >> weight;
					weights.emplace_back(weight);
				}
				if (accumulate(weights.begin(), weights.end(),
					(float)0) != (float)1) {
					cout << "Input Error: Weights must sum 1" << endl;
					weights.clear();
					continue;
				}
				keep = true;
				break;
			case 'N':
			case 'n':
				weights = vector<double>(qidNames.size(), 1.0/qidNames.size());
				keep = true;
				break;
			default:
				cout << "Do you want to use weights ";
				cout << "(will only be used on metrics) ";
				cout << "[Y(y)/N(n)]: ";
				cin >> answer;
		}
	}

    return weights;
}

tuple<vector<int>, vector<int>> readMetricsQids(vector<int> numQids, vector<int> catQids,
                                                vector<string> qidNames) {
	vector<int> numMetricsQids = numQids, catMetricsQids;
	string question = "Do you want to treat some hierarchical attributes as "
			 		  "numerical (will only be used on metrics) [Y(y)/N(n)]: ";
	cout << question;
    char answer;
	cin >> answer;
    bool keep;
	keep = false;
	while(!keep) {
		switch(answer) {
			case 'Y':
			case 'y':
				cout << "Enter number printed between brackets: ";
				for (size_t i=0; i < catQids.size(); i++)
					cout << qidNames[catQids[i]] + "(" + to_string(i) + ") ";
				cout << endl;
				char aux;
				while (1) {
					cout << " [enter q to quit] >> ";
					cin >> aux;
					if (aux == 'q')
						break;
					numMetricsQids.emplace_back(catQids[(int)aux - 48]);
				}
				keep = true;
				break;
			case 'N':
			case 'n':
				keep = true;
				break;
			default:
				cout << question;
				cin >> answer;
		}
	}
	for (const auto& qid : catQids) {
		if (find(numMetricsQids.begin(), numMetricsQids.end(), qid)
			  == numMetricsQids.end())
			catMetricsQids.emplace_back(qid);
	}


    tuple<vector<int>, vector<int>> tuple(numMetricsQids, catMetricsQids);
    return tuple;
}
