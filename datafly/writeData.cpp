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
                          const vector<vector<string>> dataset) {
        string dname = "generalized_tables";
        if (!fs::is_directory(dname) || !fs::exists(dname)) {
                if (!fs::create_directory(dname)) {
                        throw "Error creating output directory";
                        return ;
                }
        }

        string fname = "generalized_tables/" + inputFname;
        ofstream fp(fname);

        vector<vector<string>> aux;
        aux.emplace_back(headers);

        writeStrings(fp, aux, ";");
        writeStrings(fp, dataset, ";");

        fp.close();
}


