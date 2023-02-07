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


