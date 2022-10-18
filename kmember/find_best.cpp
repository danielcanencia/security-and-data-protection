
int find_best_record(vector<vector<string>> records,
		     vector<vector<string>> cluster) {
	size_t nclusters = cluster.size();
	int min = -1;
	int best;

	for (size_t i=0; i < nclusters; i++) {
		r = records[i];

		vector<vector<string>> aux;
		aux.emplace_back(cluster);
		aux.emplace_back(record[i]);

		diff = information_loss(aux);
			- information_loss(cluster);
		if (diff < min || min == -1) {
			min = diff;
			best = r;
		}
	}

	return best;
}

int find_best_cluster(vector<vector<string>> clusters,
		      vector<string> record) {

}
