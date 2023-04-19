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

vector<vector<string>> Group::generalize(vector<int> qids) {
  vector<vector<double>> values;
  for (const auto &record : records)
    values.emplace_back(record.getValues());

  // Transpose matrix and generalized based on ranges
  // vector<vector<string>> matrix = transpose(values);

  // Transpose matrix
  vector<vector<double>> matrix;
  for (size_t i = 0; i < values[0].size(); i++) {
    vector<double> aux;
    for (size_t j = 0; j < values.size(); j++)
      aux.emplace_back(values[j][i]);
    matrix.emplace_back(aux);
  }

  // Generalize based on ranges
  vector<vector<string>> anonMatrix;
  for (size_t i = 0; i < values[0].size(); i++) {
    if (find(qids.begin(), qids.end(), i) != qids.end()) {
      double max = *max_element(matrix[i].begin(), matrix[i].end());
      double min = *min_element(matrix[i].begin(), matrix[i].end());

      string element = to_string(min) + '~' + to_string(max);
      anonMatrix.emplace_back(vector<string>(matrix[i].size(), element));
      continue;
    }

    vector<string> unmodifiedVector;
    transform(begin(matrix[i]), end(matrix[i]), back_inserter(unmodifiedVector),
              [](double a) { return to_string(a); });
    anonMatrix.emplace_back(unmodifiedVector);
  }

  vector<vector<string>> resMatrix(anonMatrix[0].size(),
                                   vector<string>(anonMatrix.size()));
  for (size_t i = 0; i < anonMatrix.size(); i++) {
    for (size_t j = 0; j < anonMatrix[0].size(); j++)
      resMatrix[j][i] = anonMatrix[i][j];
  }

  return resMatrix;
}

void Group::printRecords() {
  for (const Record &record : records) {
    record.printValues();
  }
}
