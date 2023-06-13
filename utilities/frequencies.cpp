/*! \file frequencies.cpp
    \brief Fichero que contiene todas las funciones relacionadas con el cálculo de
           frecuencias utilizado en cualquier algoritmo.
*/

#include "frequencies.h"

/*! Calcula la frecuencia de un conjunto de qids en el conjunto de datos.
  \param dataset conjunto de datos.
  \param qids índices de los atributos qids.
  \return frecuencias representadas en forma de contenedor asociativo entre una
          cadena de caracteres y una tupla, conteniendo esta última la frecuencia y
          el conjunto de qids al que hace referencia.
*/
map<string, tuple<int, vector<int>>>
evaluateFrequencyByIdx(vector<vector<string>> dataset, vector<int> qids) {
  map<string, tuple<int, vector<int>>> idxs;

  // Concatenate all elements
  for (size_t i = 0; i < dataset.size(); i++) {
    vector<string> record;
    for (const auto &qid : qids) {
      record.emplace_back(dataset[i][qid]);
    }

    string s;
    for (auto const &value : record) {
      s += value;
    }

    // Save indexes
    try {
      get<0>(idxs[s]) = get<0>(idxs[s]) + 1;
      get<1>(idxs[s]).emplace_back(i);
    } catch (...) {
      idxs[s] = make_tuple(1, vector<int>(i));
    }
  }

  return idxs;
}

/*! Concatena todos los valores presentes en cada registro presentes en
    una matriz de dimensión 2.
  \param dataset conjunto de datos.
  \return vector que contiene todas las concatenaciones de valores.
*/
vector<string> concatRecords(vector<vector<string>> dataset) {
  vector<string> records;

  // Concatenate all elements
  for (size_t i = 0; i < dataset.size(); i++) {
    string s;
    for (auto const &value : dataset[i])
      s += value;
    records.emplace_back(s);
  }

  return records;
}

/*! Calcula la frecuencia de un  qid en el conjunto de datos.
  \param dataset conjunto de datos.
  \param qids índices de los atributos qids.
  \return frecuencias representadas en forma de contenedor asociativo entre una
          cadena de caracteres (como una concatenación de todos los atributos)
          y la frecuencia de dicho conjunto de qids en el conjunto de datos.
*/
map<string, int> calculateQidFreqs(const vector<vector<string>> dataset,
                                   const int dim) {

  vector<vector<string>> records = dataset;

  // Calculate frequency list
  map<string, int> freqs;

  for (const auto &record : records) {
    try {
      freqs[record[dim]] += 1;
    } catch (...) {
      freqs[record[dim]] = 1;
    }
  }

  return freqs;
}

/*! Calcula la frecuencia de un conjunto de qids en el conjunto de datos.
  \param dataset conjunto de datos.
  \param qids índices de los atributos qids.
  \return frecuencias del conjunto de qids en el conjunto de datos.
*/
vector<int> calculateFreqs(const vector<vector<string>> dataset) {

  // Concatenate all attributes of each record
  vector<string> records = concatRecords(dataset);

  // Calculate frequency list
  map<string, int> freqMap;
  for (const auto &record : records) {
    try {
      freqMap[record] += 1;
    } catch (...) {
      cout << freqMap[record] << endl;
      freqMap[record] = 1;
    }
  }

  // Get all values from freqMap
  vector<int> freqs;
  for (map<string, int>::iterator it = freqMap.begin(); it != freqMap.end();
       it++) {
    freqs.push_back(it->second);
  }

  return freqs;
}

/*! Devuelve el qid que mayor número de valores únicos presenta en el
    conjunto de datos.
  \param dataset conjunto de datos.
  \return índice del qid resultante.
*/
int findMostDistinctQid(const vector<vector<string>> dataset) {

  size_t cols = dataset[0].size();
  vector<vector<string>> values(cols);

  // Get firt value of every attribute
  for (size_t i = 0; i < cols; i++)
    values[i].emplace_back(dataset[0][i]);

  // Every attribute has at least 1 distinct value
  vector<int> nvalues(cols, 1);

  // For all records
  for (size_t i = 1; i < dataset.size(); i++) {
    // Check if the auxiliar list (values) contains
    // the attribute value
    for (size_t j = 0; j < cols; j++) {
      if (find(values[j].begin(), values[j].end(), dataset[i][j]) ==
          values[j].end()) {

        values[j].emplace_back(dataset[i][j]);
        nvalues[j]++;
      }
    }
  }

  return distance(nvalues.begin(), max_element(nvalues.begin(), nvalues.end()));
}

/*! Genera las clases de equivalencia a partir de un conjunto de datos,
    en el que cada uno de sus registro presentan el mismo conjunto de qids.
  \param dataset conjunto de datos.
  \param qids índices de los atributos qids.
  \return clases de equivalencia en forma de una matriz de dimensión 3.
*/
vector<vector<vector<string>>> createClusters(vector<vector<string>> dataset,
                                              vector<int> qids) {
  // Construct clusters based on generalizations
  vector<vector<vector<string>>> clusters;
  // Map every unique combination of qids to a matrix/cluster;
  map<string, vector<vector<string>>> splitMap;

  for (const auto &entry : dataset) {
    string value;
    vector<string> record = entry;
    for (const auto &qid : qids)
      value.append(entry[qid]);

    try {
      splitMap[value].emplace_back(record);
    } catch (...) {
      splitMap[value] = vector<vector<string>>(1, record);
    }
  }

  // Create clusters from map values
  for (const auto &[k, v] : splitMap)
    clusters.emplace_back(v);

  return clusters;
}

/*! Crea una tupla conteninendo las frecuencias de cada atributo sensible y
    una lista de los valores únicos de cada atributo sensible.
  \param dataset conjunto de datos.
  \param confAtts índices de los atributos sensibles o SAs.
  \return tupla resultante.
*/
tuple<vector<map<string, int>>, vector<set<string>>>
createDataMap(vector<vector<string>> dataset, vector<int> confAtts) {

  vector<map<string, int>> dataMaps(confAtts.size());
  vector<set<string>> valueSets(confAtts.size());
  string key;

  for (const auto &entry : dataset) {
    for (size_t j = 0; j < confAtts.size(); j++) {
      key = entry[confAtts[j]];
      try {
        dataMaps[j][key] += 1;
      } catch (...) {
        dataMaps[j][key] = 1;
      }
      valueSets[j].insert(key);
    }
  }

  return make_tuple(dataMaps, valueSets);
};
