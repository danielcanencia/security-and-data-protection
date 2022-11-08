#include "generalize.h"

bool isKAnonSatisfied(const vector<vector<string>> dataset,
                      const int K) {
        cout << "Frequencies: ";
        for (const int& freq : calculateFreqs(dataset)) {
                cout << to_string(freq) + ", ";
                if (freq < K) {
                        cout << endl;
                        return false;
                }
        }
        cout << endl;

        return true;
}


/*
  Full-Domain Generalization
*/
void generalizeQid(vector<vector<string>> &dataset,
                   const int qid, Tree tree) {

        /*vector<string> aux;
        for (size_t i=0; i < dataset.size(); i++) {
                aux.emplace_back(dataset[i][qid]);
        }

        vector<string> gen = tree.getNextGens(aux);*/

        cout << "Gens: " << endl;
        for (size_t i=0; i < dataset.size(); i++) {
                //dataset[i][qid] = gen;
                dataset[i][qid] = tree.getNextGen(dataset[i][qid]);
                cout << dataset[i][qid]  + ", ";
                cout << endl;
        }
        cout << endl;

        //transform(dataset[qid].cbegin(), dataset[qid].cend(),
        //          dataset[qid].begin(),
        //          [](unsigned string s) { return gen; });

}
void supressRecords(vector<vector<string>> &dataset, const int K) {
        // Concatenate all attributes of each record
        vector<tuple<string, int>> records = concatRecordsByIdx(dataset);

        // Calculate frequency list
        vector<int> deletions;
        while (records.size() > 0) {
                auto tuple = records.front();
                records.erase(records.begin());

                string val = get<0>(tuple);
                vector<int> idx = { get<1>(tuple) };
                int freq = 1;

                vector<int> toDelete, idxs;
                for (size_t j=0; j < records.size(); j++) {
                        if (val == get<0>(records[j])) {
                                freq += 1;
                                toDelete.emplace_back(j);
                                idxs.emplace_back(get<1>(records[j]));
                        }
                }

                int n=0;
                for (const auto& i : toDelete) {
                        records.erase(records.begin() + i - n);
                        n++;
                }

                if (freq < K) {
                        deletions.insert(deletions.begin(),
                                idxs.begin(), idxs.end());
                }

        }

        sort(deletions.begin(), deletions.end());
        int n = 0;
        for (const auto& i : deletions) {
                dataset.erase(dataset.begin() + i - n);
                n++;
        }
        cout << endl;

}


