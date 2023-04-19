#ifndef _RECORD_H
#define _RECORD_H

#include <algorithm> // transform
#include <cmath>     // pow
#include <fstream>   // ofstream
#include <iostream>  // cout
#include <vector>

using namespace std;

class Record {
private:
  int rindex;      // Determines records. Record's id
  int gindex = -1; // Determines equivalence class/cluster
  vector<double> values;

public:
  Record(int ridx);
  void pushBackValue(double value);
  void setGroup(int gidx);
  int getGindex() const;
  int size();
  double at(int idx) const;
  double euclideanDistance(const vector<double> tuple) const;
  vector<double> getValues() const;
  void writeToFile(ofstream &file) const;
  void printValues() const;
};

#endif
