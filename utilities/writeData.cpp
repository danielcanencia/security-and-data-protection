#include "writeData.h"


void writeStrings(ofstream &fp, vector<vector<string>> dataset,
                  string delimiter) {
        // Write all records
        for (size_t i=0; i < dataset.size(); i++) {
                fp << accumulate(
                        dataset[i].begin(), dataset[i].end(),
                        string(),
                        [](string &a, string &b)
                        {
                                return a.empty() ? b : a + ";" + b;
                        }) << endl;
        }
}

void writeAnonymizedTable(const string inputFname, const vector<string> headers,
                          const vector<vector<string>> dataset, const int K) {
        string dname = "generalized_tables/"
		+ inputFname.substr(0, inputFname.find_last_of('/'));
        if (!fs::is_directory(dname) || !fs::exists(dname)) {
                if (!fs::create_directories(dname)) {
                        throw "Error creating output directory";
                        return ;
                }
        }

        string fname = "generalized_tables/" + inputFname
		+ "_" + to_string(K) + ".csv";
	cout << "===> Writing data to file: " << endl;
	cout << "\t * " + fname << endl;
        ofstream fp(fname);

        vector<vector<string>> aux;
        aux.emplace_back(headers);

	try {
        	writeStrings(fp, aux, ";");
        	writeStrings(fp, dataset, ";");
	} catch (...) {
		cout << "Error writing anonymized data to file" << endl;
	}

        fp.close();
}


