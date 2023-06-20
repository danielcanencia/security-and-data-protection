/*! \file info.cpp
    \brief Fichero que implementa la clase Info.
*/

#include "info.h"

/*! Constructor de la clase Info.
  \param records lista de registros.
  \param hierarchies mapa de jerarquias.
  \param numQids lista de índices de atributos numéricos.
  \param catQids lista de índices de atributos categóricos.
*/
Info::Info(vector<vector<string>> records,
           map<int, vector<vector<string>>> hierarchies, map<int, Tree> trees,
           vector<int> numQids, vector<int> catQids) {
  this->records = records;
  this->attsValues = transpose(records);
  this->hierarchies = hierarchies;
  this->trees = trees;
  this->numQids = numQids;
  this->catQids = catQids;

  calculateHeights();
  calculateMaxDomSizes();
}

/*! Calcular el tipo de un atributo qid.
  \param qid índice del atributo qid.
  \return tipo del atributo.
*/
int Info::valueType(int qid) {

  if (find(this->numQids.begin(), this->numQids.end(), qid) !=
      this->numQids.end()) {
    return 1;
  }
  if (find(this->catQids.begin(), this->catQids.end(), qid) !=
      this->catQids.end()) {
    return 0;
  }

  return -1;
}

/*! Devuelve los atributos qid numéricos.
  \return lista ded atributos qids numéricos.
*/
vector<int> Info::getNumQids() { return this->numQids; }

/*! Devuelve los atributos qid categóricos.
  \return lista de atributos qids categóricos.
*/
vector<int> Info::getCatQids() { return this->catQids; }

/*! Cálcula las alturas de los árboles jerárquicos.
*/
void Info::calculateHeights() {
  int aux = 0;

  for (const auto &idx : this->catQids) {
    int height = 0;
    for (const auto &gens : this->hierarchies[idx]) {
      aux = gens.size();
      if (aux > height)
        height = aux;
    }
    this->treeHeights[idx] = height;
  }
}

/*! Cálcula los rangos de cada atributo qid numérico.
  \return lista ded atributos qids numéricos.
*/
void Info::calculateMaxDomSizes() {
  int value, vtype;

  for (size_t i = 0; i < this->attsValues.size(); i++) {
    vtype = valueType(i);
    if (!vtype || vtype == -1)
      continue;

    long double min = stold(this->attsValues[i][0]);
    long double max = stold(this->attsValues[i][0]);

    for (size_t j = 0; j < this->attsValues[i].size(); j++) {
      value = stold(this->attsValues[i][j]);
      if (value > max)
        max = value;
      if (value < min)
        min = value;
    }
    this->maxDomSizes[i] = max - min;
  }
}

/*! Devuelve la altura de un árbol jerárquico.
  \param index índice del qid categórico.
  \return altura del árbol.
*/
int Info::getTreeHeight(int index) { return trees[index].getHeight(); }

/*! Devuelve el rango de un atributo numérico.
  \param index índice del qid numérico.
  \return rango máximo de valores.
*/
long double Info::getMaxDomSize(int index) { return this->maxDomSizes[index]; }

/*! Devuelve una lista de todos los registros.
  \return lista de registros.
*/
vector<vector<string>> Info::getRecords() { return this->records; }

/*! Devuelve los mapas de jerarquías de los atributos qids categóricos.
  \param index índice del qid categórico.
  \return altura del árbol.
*/
map<int, vector<vector<string>>> Info::getHierarchies() {
  return this->hierarchies;
}

/*! Devuelve la altura del subárbol definido por v1 y v2.
  \param v1 primer valor categórico.
  \param v2 segundo valor categórico.
  \param index índice del qid categórico.
  \return altura del subárbol.
*/
int Info::getSubTreeHeight(string v1, string v2, int index) {
  string ancestor =
      trees[index].getLowestCommonAncestor(vector<string>({v1, v2})).value;
  return trees[index].getHeight(ancestor);
}

/*! Devuelve el ancestro común mínimo (a menor profundidad) de todos los registros.
  \param index índice del qid categórico.
  \return valor del ancestro común.
*/
int Info::lowestCommonAncestor(int index) {

  set<string> valueSet;
  vector<int> indexes;
  for (size_t i = 0; i < records.size(); i++) {
    valueSet.insert(records[i][index]);
    indexes.emplace_back(i);
  }

  vector<string> values(valueSet.begin(), valueSet.end());
  string ancestor = trees[index].getLowestCommonAncestor(values).value;

  return trees[index].getHeight(ancestor);
}
