#ifndef _RECORD_H
#define _RECORD_H

#include <vector>
#include <cmath>    // pow
#include <fstream>  // ofstream
#include <iostream> // cout

using namespace std;

class Record {
private:
	int rindex;
	int gindex = -1;
	vector<double> values;

public:
	Record(int ridx);
	void pushBackValue(double value);
	void setGroup(int gidx);
	int getGindex() const;
	int size();
	double at(int idx) const;
	double euclideanDistance(const vector<double> tuple) const;
	vector<double> getValues();
	void writeToFile(ofstream& file) const;
	void printValues() const;
};

#endif
