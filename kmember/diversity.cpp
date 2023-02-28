#include "diversity.h"

bool isDiverse(vector<vector<string>> cluster,
               const int confAtt, const int L) {
    map<string, int> freqs;

    // Confidential / sensitive attribute frequencies
	for (const auto& entry : cluster) {
		try {
			freqs[entry[confAtt]] += 1;
		} catch (...) {
			freqs[entry[confAtt]] = 1;
		}
	}

	// Confidential attribute should have, at least,
	// l well represented values
	// Get all values from map
	for (const auto& [k, v] : freqs) {
		if (v < L)
			return false;
	}

	return true;
}

string majorityClass(vector<vector<string>> cluster,
					 const int confAtt) {
    map<string, int> freqs;

    // Confidential / sensitive attribute frequencies
	for (const auto& entry : cluster) {
		try {
			freqs[entry[confAtt]] += 1;
		} catch (...) {
			freqs[entry[confAtt]] = 1;
		}
	}

	// Get value with maximum frequency
	return (*max_element(freqs.begin(), freqs.end(),
		   		[](const auto &a, const auto &b) {
            		return a.second < b.second;
				}
		   )).first;
}

bool isSensitive(string majorityClass, const int confAtt,
				 vector<vector<string>> cluster) {
	vector<string> values = transpose(cluster)[confAtt];
	return find(values.begin(), values.end(), majorityClass)
		!= values.begin();
}

string readConfidentialAttName() {
	cout << "Confidential attribute: " << endl;

	// Read confidential attributes name
	string attName;
	cout << "\tEnter att name: ";
	cin >> attName;

    return attName;
}

int readDiversityPenalty() {
	string question = "Diversity Penalty: ";
	cout << question;

	int penalty;
	while(1) {
		cin >> penalty;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Error, enter a valid number." << endl;
			cout << question;
			continue;
		}
		break;
	}

    return penalty;
}

int readDiversity() {
	string question = "Diversity Metric:\n"
		"\tEqual Diversity Metric (0), "
		"Sensitive Diversity Metric (1): ";
	cout << question;

	int diversity;
	while(1) {
		cin >> diversity;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Error, enter a valid number." << endl;
			cout << question;
			continue;
		}
		break;
	}

	return diversity;
}

vector<string> readSensitiveValues(vector<string> values) {
	vector<string> sensitiveValues;
	string aux;

	while (!values.empty()) {
		cout << "Enter number printed between brackets:\n\t";
		for (size_t i=0; i < values.size(); i++)
			cout << values[i] + "(" + to_string(i) + ") ";
		cout << endl << "\t\t[enter q to quit] >> ";
		cin >> aux;

		if (aux == "q")
			break;

		int idx = stoi(aux);
		if (idx >= (int)values.size() || idx < 0) {
			cout << "Error, index out of range." << endl;
			continue;
		}
		sensitiveValues.emplace_back(values[idx]);
		values.erase(values.begin() + idx);
	}

	return sensitiveValues;
}
