#include "tree.h"

Tree::Tree(vector<int> nodeMax) {
	this->nodeMax = nodeMax;
}

vector<vector<int>> Tree::getPermutations(int r) {

	int nsize = this->nodeMax.size();

	//Permutamos, obteniendo los valores de los nodos del grafo
	vector<vector<int>> permutations;
        permute(this->nodeMax, permutations, nsize, r);
	return permutations;
}


void Tree::permute(const vector<int> data,
		   vector<vector<int>>& permutations,
	           int n, int r, int rept, int idx, int aux[]) {
	if (aux==NULL) {
		aux = new int[r];
	}


	if (idx==r) {
		vector<int> entry;
		for(int i=0; i < r; i++) {
			//cout << data[aux[i]];
			entry.emplace_back(data[aux[i]]);
			
		}
		permutations.emplace_back(entry);
		//cout << endl;
		return ;
	}

	for (int i=rept; i < n; i++) {
		aux[idx] = i;
		permute(data, permutations, n, r, i, idx+1, aux);
	}
}

