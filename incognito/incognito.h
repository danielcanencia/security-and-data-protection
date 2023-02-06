#ifndef _INCOGNITO_H
#define _INCOGNITO_H

#include "hierarchy.h"
#include "tree.h"
using namespace std;

vector<Tree> graphGeneration(const vector<int>& qids,
			     const vector<int>& nodeMax,
			     int numAttrs);

#endif

