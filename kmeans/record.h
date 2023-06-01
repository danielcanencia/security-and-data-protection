/*! \file record.h
    \brief Fichero de cabecera del archivo record.cpp.
*/

#ifndef _RECORD_H
#define _RECORD_H

#include <algorithm> // transform
#include <cmath>     // pow
#include <fstream>   // ofstream
#include <iostream>  // cout
#include <vector>

using namespace std;

/*! Clase Record. Su función es simular un registro, es decir,
    un conjunto de cadenas de caracteres vinculadas a un individuo o
    entidad.
*/
class Record {
private:
  int rindex;      //! Índice del registro.
  int gindex = -1; //! Índice del cluster al que pertenece el registro.
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
