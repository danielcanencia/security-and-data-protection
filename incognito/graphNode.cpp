/*! \file graphNode.cpp
    \brief Fichero que contiene la clase graphNode.
*/

#include "graphNode.h"

// Constructor vacio de la clase GraphNode.
GraphNode::GraphNode() { this->id = -1; };

/*! Constructor de la clase graphNode.
  \param id identificador del nodo.
  \param data datos del nodo a añadir.
*/
GraphNode::GraphNode(int id, vector<int> data) {
  this->id = id;
  this->data = data;
}

/*! Devuelve el identificador del nodo.
  \param data nodo a añadir.
*/
int GraphNode::getId() const { return this->id; }

/*! Devuelve los datos del nodo en una posición específica i.
  \param i posición del dato.
*/
int GraphNode::getData(int i) const { return this->data[i]; }

/*! Devuelve los datos del nodo.
  \return datos del nodo.
*/
vector<int> GraphNode::getData() const { return this->data; }

/*! Comprueba si el nodo dado es igual al de la clase.
  \param node nodo.
*/
bool GraphNode::isEqual(const vector<int> &node) const {
  for (int i = 0; i < (int)node.size(); i++) {
    if (this->data[i] != node[i])
      return false;
  }
  return true;
}

/*! Comprueba si un nodo es hijo del actual.
  \return 1 si es hijo, 0 si no lo es.
*/
bool GraphNode::isChild(GraphNode node) {

  int sum = 0, flag = 0;
  for (int i = 0; i < int(this->data.size()); i++) {
    if (this->data[i] == node.getData(i) - 1) {
      sum = sum + 1;
    }
    if (abs(node.getData(i) - this->data[i]) == 1) {
      flag += 1;
    }

    if (abs(node.getData(i) - this->data[i]) >= 2) {
      return false;
    }
  }

  if (sum == 1 && flag == 1)
    return true;
  return false;
}

/*! Define el comportamiento del operador < sobre dos clases GraphNode.
  \param node nodo secundario.
  \return 1 si cumple los requisitos del operador <, 0 si no es así.
*/
bool GraphNode::operator<(const GraphNode &node) const {
  return (getSum() < node.getSum()) || (id < node.getId());
}

/*! Suma todos los datos del nodo.
  \return suma total.
*/
int GraphNode::getSum() const {
  int sum = 0;
  for (const auto &entry : this->data)
    sum += entry;
  return sum;
}

/*! Comprueba si el nodo esta marcado.
  \return 1 si esta marcado, 0 si no es así.
*/
bool GraphNode::marked() const { return this->nodeMark; }

/*! Marca el nodo actual.
*/
void GraphNode::mark() { this->nodeMark = true; }

/*! Establece que el nodo cumple con los modelos de privacidad dados.
*/
void GraphNode::setKAnon() { this->kAnon = true; }

/*! Comprueba si el nodo cumple con los modelos de privacidad.
  \return 1 si cumple con los modelos de privacidad, 0 si no es así.
*/
bool GraphNode::isKAnon() const { return this->kAnon; }

/*! Generaliza el valor de un atributo.
  \param entry valor del atributo.
  \param hierarchy jerarquía del atributo.
  \param generalizations lista de generalizaciones del atributo.
  \return valor generalizado.
*/
string GraphNode::generalizeEntry(string entry,
                                  const vector<vector<string>> hierarchy,
                                  vector<string> generalizations) {
  int index = -1;

  for (int i = 0; i < int(hierarchy.size()); i++) {
    for (int j = 0; j < int(generalizations.size()); j++) {
      if (hierarchy[i][j].compare(entry) == 0) {
        index = j;
        break;
      }
    }
  }

  if (index == -1)
    return "No entry found";

  return generalizations[index];
}

/*! Comprueba si el nodo cumple con la k-anonimidad.
  \param dataset conjunto de datos.
  \param gensMap mapa de generalizaciones.
  \param dataMap matriz de datos sin anonimizar.
  \param qids lista de atributos cuasi-identificadores.
  \param confAtts lista de índices de atributos sensibles.
  \param K parámetro de la k-anonimidad.
  \param L parámetro de la l-diversidad.
  \param T parámetro de t-closeness.
  \return 1 si se cumplen los modelos de privacidad, 0 si no es así.
*/
bool GraphNode::isAnonymityValid(
    map<int, vector<vector<string>>> hierarchies,
    vector<vector<string>> dataset,
    map<int, map<string, vector<string>>> gensMap,
    tuple<vector<map<string, int>>, vector<set<string>>> dataMap,
    vector<int> qids, vector<int> confAtts, const int K, const int L,
    const long double P) {

  // Mapear o vincular cada combinación única de qids a una clase de equivalencia
  map<string, vector<vector<string>>> splits;
  vector<vector<string>> anonData;
  string value, choosenGen;
  for (const auto &entry : dataset) {
    string value;
    vector<string> record = entry;

    for (size_t i = 0; i < qids.size(); i++) {
      choosenGen = gensMap[qids[i]][entry[qids[i]]][this->data[i]];
      value.append(choosenGen);
      record[qids[i]] = choosenGen;
    }
    
    try {
      splits[value].emplace_back(record);
    } catch (...) {
      splits[value] = vector<vector<string>>(1, record);
    }

    anonData.emplace_back(record);
  }

  // Crear clases de equivalencia del mapa creado anteriormente
  vector<vector<vector<string>>> clusters;
  for (const auto &[k, v] : splits)
    clusters.emplace_back(v);

  return isSplitValid(clusters, dataMap, anonData, qids, confAtts, K, L, P);
}

/*! Calcula las frecuencias de cada valor generalizado.
  \param dataset conjunto de datos.
  \param generalizations mapa de generalizaciones.
  \param qids lista de atributos cuasi-identificadores.
  \return lista de frecuencias.
*/
vector<int> GraphNode::evaluateFrequency(
    map<int, map<string, vector<string>>> generalizations,
    vector<vector<string>> dataset, vector<int> qids) const {

  // Anonimizar cada columna en la que se encuentre un qid
  vector<string> genData;
  int qid = qids[0];
  for (size_t i = 0; i < dataset.size(); i++) {
    genData.emplace_back(generalizations[qid][dataset[i][qid]][data[0]]);
  }

  // Cálcular las frecuencias para cada qid
  map<string, int> freqMap;
  for (const auto &gen : genData) {
    try {
      freqMap[gen] += 1;
    } catch (...) {
      freqMap[gen] = 1;
    }
  }

  // Obtener la lista de frecuencias
  vector<int> freqs;
  for (const auto &[k, v] : freqMap)
    freqs.emplace_back(v);

  return freqs;
}

/*! Imprime los datos del nodo.
*/
void GraphNode::print() const {
  cout << "Id: " + to_string(id) + ", ";
  cout << "Data: ";
  for (const int &val : this->data)
    cout << to_string(val) + " ";
  cout << endl;
}
