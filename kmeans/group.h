#ifndef _GROUP_H
#define _GROUP_H

#include <vector>
#include <fstream>
#include "record.h"

using namespace std;

class Group {
private:
	const int gindex;
	vector<double> centroid;
	vector<Record> records;
public:
	Group(int gidx, Record& _centroid);
	int getIndex();
	vector<double> getCentroid() const;
	void insertCentroid(Record& centroid);
	void addRecord(Record& record);
	void removeRecord(Record& record);
	void recalculateCentroid();
	void writeToFile(ofstream& file) const;
	void printRecords();
};

#endif