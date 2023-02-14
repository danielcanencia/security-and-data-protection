#include "metrics.h"

void calculateDM(vector<vector<vector<string>>> clusters,
                 const int tableSize, const int K) {
    long double dm = 0;
    for (const vector<vector<string>>& cluster : clusters) {
        if ((int)cluster.size() >= K) {
            dm += pow(cluster.size(), 2.0);
            //cout << ">=K: "; cout << cluster.size() << endl;
        }
        else {
            dm += tableSize * cluster.size();
            //cout << "<K: ";
            //cout << cluster.size() << endl;
        }
    }

    cout << "\t* DM: ";
    cout << fixed << setprecision(3) << dm << endl;
}

void calculateCAVG(vector<vector<vector<string>>> clusters,
                   const int tableSize, const int K) {
    cout << "\t* CAvg: ";
    cout << fixed << setprecision(3);
    cout << tableSize / (long double) (clusters.size() * K) << endl;
}

// Generalized Information Loss

long double calculateMaxNumValue(vector<string> entries) {
    /*for (const auto& a : entries)
        cout << a + ", ";
    cout << endl;*/

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

    //string str2 = globalMax.substr(globalMax.find("~") + 1, globalMax.size());
    //cout << "GMax: "; cout << str2 << endl;

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

    //cout << "GMin: ";
    //cout << globalMin.substr(0, globalMin.find("~")).c_str() << endl;

    return strtold(
        globalMin.substr(
            0,
            globalMin.find("~")
        ).c_str(), NULL);
}

long double calculateNumGenILoss(const string entry, vector<string> entries,
        const long double globalMax, const long double globalMin) {

    /*for (const auto& a : entries) {
        cout << a + ", ";
    }
    cout << endl;
    cout << "Entry: "; cout << entry << endl;*/

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

    /*cout << "Min: "; cout << min << endl;
    cout << "Max: "; cout << max << endl;

    const long double a = (max - min) / (globalMax - globalMin);
    cout << "NumGLoss: "; cout << a << endl;*/
    return (max - min) / (globalMax - globalMin);
}

long double calculateCatGenILoss(const string entry, Tree tree) {
    /*cout << "Cat GenILoss: " << endl;
    cout << "Node: " + entry + " Children: " << endl;
    for (const auto& a : tree.getDirectChildren(entry))
        cout << a + ", ";
    cout << endl;*/

    const int directChildren = tree.getDirectChildren("America").size();
    const int childrenInLevel = tree.getChildrenInLevel("America").size();
    const int numerator = directChildren >= 1 ? directChildren - 1 : directChildren;
    const int denominator = childrenInLevel >= 1 ? childrenInLevel - 1 : childrenInLevel;

    /*cout << "Size: ", cout << directChildren << endl;
    cout << "Children In Level: " << childrenInLevel;
    cout << endl;
    cout << "Num: "; cout << numerator << endl;
    cout << "Den: "; cout << denominator << endl;
    const long double q =  denominator == 0 ? 0 : numerator / (long double) denominator;
    cout << q << endl;*/


    return denominator == 0 ?
        0 : numerator / (long double) denominator;
}

void calculateGenILoss(vector<vector<string>> transposedDataset,
                       map<int, Tree> trees, const vector<int> catQids,
                       const vector<int> numQids,
                       const int tableSize, const int K) {
    const long double initialLoss = 1.0 / (catQids.size() + numQids.size());
    long double loss = 0;

    // Iterate through every attribute in transposed dataset
    for (size_t i = 0; i < transposedDataset.size(); i++) { //const vector<string>& entries : transposedDataset) {
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

    cout << "QidsS: ";
    cout << (catQids.size() + numQids.size()) << endl;
    cout << "Loss: "; cout << loss << endl;
    cout << "ILoss: "; cout << initialLoss << endl;

    cout << "\t* GenILoss: ";
    cout << fixed << setprecision(3);
    cout << initialLoss * loss << endl;
}