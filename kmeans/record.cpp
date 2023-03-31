#include "record.h"

Record::Record(int ridx) { rindex = ridx; }

void Record::pushBackValue(double value) { values.emplace_back(value); }
void Record::setGroup(int gidx) { gindex = gidx; }
int Record::getGindex() const { return gindex; }
int Record::size() { return values.size(); }
double Record::at(int idx) const { return values[idx]; }
double Record::euclideanDistance(const vector<double> tuple) const {
  int sum = 0;
  for (int i = 0; i < (int)min(tuple.size(), values.size()); i++)
    sum += pow((double)(values[i]) - (double)(tuple[i]), 2);
  return sqrt(sum);
}
vector<double> Record::getValues() { return values; }
void Record::writeToFile(ofstream &file) const {
  for (auto it = begin(values); it != end(values) - 1; it++) {
    file << to_string(*it) + ",";
  }
  file << to_string(values.back()) << endl;
}
void Record::printValues() const {
  for (const double &val : values) {
    cout << to_string(val) + ", ";
  }
  cout << '\n';
}
