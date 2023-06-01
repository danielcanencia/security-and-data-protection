/*! \file group.h
    \brief Fichero de cabecera del archivo group.cpp.
*/

#ifndef _GROUP_H
#define _GROUP_H

#include "record.h"
#include <fstream>
#include <vector>

using namespace std;

/*! Clase Group. Su función es simular un cluster o grupo, es decir,
    un conjunto de registros.
*/
class Group {
private:
  const int gindex;        //! Índice del grupo.
  vector<double> centroid; //! Centroide (entendido como registro).
  vector<Record> records;  //! Conjunto de registros.

public:
  Group(int gidx, Record &_centroid);
  int getIndex();
  vector<double> getCentroid() const;
  void insertCentroid(Record &centroid);
  void addRecord(Record &record);
  void removeRecord(Record &record);
  void recalculateCentroid();
  vector<vector<string>> generalize(vector<int> qids);
  void printRecords();
};

#endif
