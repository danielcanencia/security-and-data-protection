#include "evaluate.h"

vector<vector<string>> evaluate(Partition partition) {
    vector<Partition> result;
    evaluate(partition, result);

	vector<vector<string>> data;
	for (const auto& p : result) {
		vector<vector<string>> vec = p.getResult();
		data.insert(data.begin(), vec.begin(),
	    vec.end());
	}

    return data;
}

void evaluate(Partition partition, vector<Partition>& result) {
	if (partition.getNumAllowedCuts() == 0) {
    	result.emplace_back(partition);
		return ;
    }

	int dim = partition.choose_dimension();
    vector<Partition> pts = partition.splitPartition(dim);

    if (pts.size() == 0) {
        partition.setAllowedCuts(0, dim);
		evaluate(partition, result);
        return ;
    }

	for (const auto& p : pts)
		evaluate(p, result);

	return ;
}

vector<vector<string>> mondrian (vector<vector<string>> dataset,
								 map<int, vector<vector<string>>> hierarchies,
								 vector<int> allQids, vector<int> isQidCat,
								 const int K) {	
	// 1. Create a hierarchy tree for every qid
	map<int, Tree> trees;
	// 2. Initialize default generalizations
	vector<string> gens;
	for (size_t i=0; i < allQids.size(); i++) {
		if (isQidCat[i]) {
			// Generate a hierarchy tree for every categorical attribute
			trees[allQids[i]] = Tree(hierarchies[i]);
			gens.emplace_back(trees[allQids[i]].root);
			continue;
		}

		// Numeric Value
		string numRoot = getNumericRoot(dataset, allQids[i]);
		gens.emplace_back(numRoot);
	}

	// 3. Anonymize whole initial partition
	Partition partition(dataset, gens, allQids,
			    isQidCat, trees, K);

	return evaluate(partition);
}
