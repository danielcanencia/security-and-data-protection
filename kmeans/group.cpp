#include "group.h"

Group::Group(int gidx, Record &_centroid) : gindex(gidx) {
  this->insertCentroid(_centroid);
}

int Group::getIndex() { return gindex; }
vector<double> Group::getCentroid() const { return centroid; }
void Group::insertCentroid(Record &centroid) {
  this->centroid = centroid.getValues();
  centroid.setGroup(gindex);
  records.emplace_back(centroid);
}

void Group::addRecord(Record &record) { records.emplace_back(record); }
void Group::removeRecord(Record &record) { records.erase(records.begin() + 1); }

void Group::recalculateCentroid() {
  int atts = this->records[0].size();
  vector<double> centroid(atts, 0.0);
  for (Record &record : this->records) {
    for (int j = 0; j < atts; j++) {
      centroid[j] += record.at(j);
    }
  }
  for (int j = 0; j < atts; j++)
    centroid[j] /= records.size();

  this->centroid = centroid;
}

void Group::writeToFile(string filename, string headers) const {
  ofstream file;

  file.open(filename, ios::trunc);
  if (!file.is_open()) {
    cout << "Unable to open file" << endl;
    return;
  }

  file << headers << endl;
  for (const Record &record : records)
    record.writeToFile(file);

  file.close();
}

void Group::printRecords() {
  for (const Record &record : records) {
    record.printValues();
  }
}
