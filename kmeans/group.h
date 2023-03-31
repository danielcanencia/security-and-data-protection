#ifndef _GROUP_H
#define _GROUP_H

#include "record.h"
#include <fstream>
#include <vector>

using namespace std;

class Group {
private:
  const int gindex;        // Determines equivalence class/cluster
  vector<double> centroid; // Centroids are assign to a specific record (id)
  vector<Record> records;

public:
  Group(int gidx, Record &_centroid);
  int getIndex();
  vector<double> getCentroid() const;
  void insertCentroid(Record &centroid);
  void addRecord(Record &record);
  void removeRecord(Record &record);
  void recalculateCentroid();
  void writeToFile(string filename, string headers) const;
  void printRecords();
};

#endif
