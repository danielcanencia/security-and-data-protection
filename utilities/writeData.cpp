#include "writeData.h"

#define GET_NAME(Var) (#Var)

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

void writeAnonymizedTable(const string inputFname,
                          const vector<string> headers,
                          const vector<vector<string>> dataset,
                          const int K, const int L, const long double P,
                          const string prefix) {
        string kName = K == -1 ? "" : to_string(K) + GET_NAME(K);
        if (L != -1 && K != -1) kName += "_";
        string lName = L == -1 ? "" : to_string(L) + GET_NAME(L);
        if (P != -1 && L != -1) lName += "_";
        stringstream stream;
        stream << fixed << setprecision(2) << P;
        const string pName = P == -1 ? "" : stream.str() + GET_NAME(P);


        string input = inputFname;
        if (input.back() == '/')
               input.resize(input.size() - 1); 
        const string parentPath = filesystem::path{input}.string();
        string dname = "generalized_tables"
                + parentPath.substr(parentPath.find_last_of('/'), parentPath.size()) + "/";

        if (!fs::is_directory(dname) || !fs::exists(dname)) {
                if (!fs::create_directories(dname)) {
                        throw "Error creating output directory";
                        return ;
                }
        }

        string fname = dname;
        if (prefix == "\0")
                fname += kName + lName + pName + ".csv";
        else
                fname += prefix + ".csv";
        
	cout << "===> Writing data to file: " << endl;
	cout << "\t* " + fname << endl;
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


