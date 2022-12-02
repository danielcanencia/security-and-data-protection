#ifndef _EVALUATE_H
#define _EVALUATE_H

using namespace std;

#include <iostream>
#include <vector>
#include "partition.h"

vector<Partition> evaluate(Partition partition);
void evaluate(Partition partition,
	      vector<Partition>& result);

#endif

