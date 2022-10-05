#ifndef _TREE_H
#define _TREE_H

#include <iostream>
#include <vector>
#include <string>


using namespace std;

class Tree {
private:
	//vector<vector<string>> nodes;
	vector<int> nodeMax;
	void permute(vector<int> data,
	       	     vector<vector<int>>& permutations,
		     int n, int r, int rept=0, int idx=0,
		     int aux[]=NULL);

public:
	Tree(vector<int> nodeMax);
	vector<vector<int>> getPermutations(int r);
};


#endif
