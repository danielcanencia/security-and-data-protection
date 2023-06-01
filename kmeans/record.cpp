/*! \file record.cpp
    \brief Fichero que contiene la clase Record, destinada a aportar
           la funcionalidad necesaria para manejar un registro.
*/

#include "record.h"

//! Constructor de la clase Record
/*!
  \param ridx índice del registro.
*/
Record::Record(int ridx) { rindex = ridx; }

//! Inserta un elemento en el registro.
/*!
  \param value valor numérico.
*/
void Record::pushBackValue(double value) { values.emplace_back(value); }
//! Actualiza el grupo al que pertenece el registro.
/*!
  \param gidx índice del grupo.
*/
void Record::setGroup(int gidx) { gindex = gidx; }
//! Método getter del índice del grupo al que pertenece el registro.
/*!
  \return índice del grupo.
*/
int Record::getGindex() const { return gindex; }
//! Devuelve la cardinalidad del registro.
/*!
  \return cardinalidad del registro.
*/
int Record::size() { return values.size(); }
//! Devuelve un elemento del registro a partir de su índice.
/*!
  \param idx posición del elemento a obtener.
  \return elemento del registro en la posición idx.
*/
double Record::at(int idx) const { return values[idx]; }
//! Calcula la distancia euclideana de una lista de valores numéricos,
//! típicamente de longitud dos.
/*!
  \param tuple lista de valores numéricos.
  \return distancia euclideana calculada.
*/
double Record::euclideanDistance(const vector<double> tuple) const {
  int sum = 0;
  for (int i = 0; i < (int)min(tuple.size(), values.size()); i++)
    sum += pow((double)(values[i]) - (double)(tuple[i]), 2);
  return sqrt(sum);
}
//! Devuelve todos los elementos de los que se compone el registro.
/*!
  \return vector compuesto por todos los elementos.
*/
vector<double> Record::getValues() const { return values; }
//! Escritura del registro en un fichero específico.
/*!
  \param file output stream del fichero.
*/
void Record::writeToFile(ofstream &file) const {
  for (auto it = begin(values); it != end(values) - 1; it++) {
    file << to_string(*it) + ",";
  }
  file << to_string(values.back()) << endl;
}
//! Imprime todos los elementos del registro por pantalla.
void Record::printValues() const {
  for (const double &val : values) {
    cout << to_string(val) + ", ";
  }
  cout << '\n';
}
