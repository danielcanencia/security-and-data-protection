#include "metrics.h"

#define GET_NAME(Var) (#Var)

// Discernibility Metric
void calculateDM(vector<vector<vector<string>>> clusters,
                 const int tableSize, const int param,
                 const string paramName) {
    long double dm = 0;
    for (const vector<vector<string>>& cluster : clusters) {
        if ((int)cluster.size() >= param)
            dm += pow(cluster.size(), 2.0);
        else
            dm += tableSize * cluster.size();
    }

    cout << "\t* DM using " + paramName + ": ";
    cout << fixed << setprecision(3) << dm << endl;
}

void calculateDM(vector<vector<vector<string>>> clusters,
                 const int tableSize, const int K,
                 const int L, const int P) {
    if (K != -1)
        calculateDM(clusters, tableSize, K, GET_NAME(K));
    if (L != -1)
        calculateDM(clusters, tableSize, L, GET_NAME(L));
    if (P != -1)
        calculateDM(clusters, tableSize, P, GET_NAME(P));
}

// Equivalent Class Size Metric
void calculateCAVG(vector<vector<vector<string>>> clusters,
                   const int tableSize, const int param,
                   const string paramName) {
    cout << "\t* CAvg using " + paramName + ": ";
    cout << fixed << setprecision(3);
    cout << tableSize / (long double) (clusters.size() * param) << endl;
}

void calculateCAVG(vector<vector<vector<string>>> clusters,
                   const int tableSize, const int K,
                   const int L, const int P) {
    if (K != -1)
        calculateCAVG(clusters, tableSize, K, GET_NAME(K));
    if (L != -1)
        calculateCAVG(clusters, tableSize, L, GET_NAME(L));
    if (P != -1)
        calculateCAVG(clusters, tableSize, P, GET_NAME(P));
}

// Generalized Information Loss
long double calculateMaxNumValue(vector<string> entries) {

    // Max Value
    string globalMax = (*max_element(entries.begin(), entries.end(),
                            [&] (const string& a, const string& b) {
                                string str1 = a.substr(
                                    a.find("~"),
                                    a.size());
                                string str2 = b.substr(
                                    b.find("~"),
                                    b.size());
                                return strtold(str1.c_str(), NULL) <
                                       strtold(str2.c_str(), NULL);
                            })).c_str();

    return strtold(
        globalMax.substr(
            globalMax.find("~") + 1,
            globalMax.size()
        ).c_str(), NULL);
}

long double calculateMinNumValue(vector<string> entries) {
    // Max Value
    string globalMin = (*min_element(entries.begin(), entries.end(),
                            [&] (const string& a, const string& b) {
                                string str1 = a.substr(
                                    a.find("~"),
                                    a.size());
                                string str2 = b.substr(
                                    b.find("~"),
                                    b.size());
                                return strtold(str1.c_str(), NULL) <
                                       strtold(str2.c_str(), NULL);
                            })).c_str();

    return strtold(
        globalMin.substr(
            0,
            globalMin.find("~")
        ).c_str(), NULL);
}

long double calculateNumGenILoss(const string entry, vector<string> entries,
        const long double globalMax, const long double globalMin) {

    const int delimeterPos = entry.find("~");
    const long double min = strtold(
                        entry.substr(
                            0, delimeterPos
                        ).c_str(), NULL);
    const long double max = strtold(
                        entry.substr(
                            delimeterPos + 1,
                            entry.size()
                        ).c_str(), NULL);

    return (max - min) / (globalMax - globalMin);
}

long double calculateCatGenILoss(const string entry, Tree tree) {

    const int directChildren = tree.getDirectChildren("America").size();
    const int childrenInLevel = tree.getChildrenInLevel("America").size();
    const int numerator = directChildren >= 1 ? directChildren - 1 : directChildren;
    const int denominator = childrenInLevel >= 1 ? childrenInLevel - 1 : childrenInLevel;

    return denominator == 0 ?
        0 : numerator / (long double) denominator;
}


void calculateGenILoss(vector<vector<string>> transposedDataset,
                       map<int, Tree> trees, const vector<int> catQids,
                       const vector<int> numQids, const int tableSize) {
    const long double initialLoss = 1.0 / (catQids.size() + numQids.size());
    long double loss = 0;

    // Iterate through every attribute in transposed dataset
    for (size_t i = 0; i < transposedDataset.size(); i++) {
        vector<string> entries = transposedDataset[i];
        long double globalMax, globalMin;
        globalMax = globalMin = 0;
        int categorical = -1;
        
        auto it = find(catQids.begin(), catQids.end(), i);
        if (find(numQids.begin(), numQids.end(), i) != numQids.end()) {
            globalMax = calculateMaxNumValue(entries);
            globalMin = calculateMinNumValue(entries);
            categorical = 0;
        }
        else if (it != catQids.end())
            categorical = 1;

        for (const string& entry : entries) {
            if (categorical == 0) {
                loss += calculateNumGenILoss(entry, entries,
                            globalMax, globalMin);
            }
            else if (categorical) 
                loss += calculateCatGenILoss(entry, trees[i]);
        }
    }

    cout << "\t* GenILoss: ";
    cout << fixed << setprecision(3);
    cout << initialLoss * loss << endl;
}
