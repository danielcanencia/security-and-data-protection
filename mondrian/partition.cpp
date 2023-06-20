/*! \file partition.cpp
    \brief Fichero que proporciona las estructuras y métodos necesarios para realizar
           el proceso de particionado de una tabla de datos.
*/

#include "partition.h"

/*! Constructor de la clase Partition.
  \param data conjunto de datos.
  \param qids índices de los atributos qids.
  \param generalization generalización a aplicar a cada qid.
  \param isQidCat vector que contiene los tipos de cado uno de los atributos qids.
  \param trees árboles de jerarquía de todos los atributos.
  \param confAtts conjunto de atributos sensibles o SAs.
  \param K parámetro del modelo de privacidad k-anonymity.
  \param L parámetro del modelo de privacidad l-diversity.
  \param T parámetro del modelo de privacidad t-closeness.
  \return instancia de la partición creada.
*/
Partition::Partition(vector<vector<string>> data,
                     vector<string> generalizations, vector<int> qids,
                     vector<int> isQidCat, map<int, Tree> trees,
                     vector<int> confAtts, int K, int L, long double T) {
  this->data = data;
  this->generalizations = generalizations;
  this->qids = qids;
  this->isQidCat = isQidCat;
  this->trees = trees;
  this->confAtts = confAtts;
  this->K = K;
  this->L = L;
  this->T = T;
  this->allowedCuts = vector<int>(qids.size(), 1);
}

/*! Generaliza el conjunto de datos perteneciente a la partición.
  \return conjunto de datos anonimizado en forma de matriz de dimensión 2.
*/
vector<vector<string>> Partition::getResult() const {
  vector<vector<string>> result = data;

  for (size_t i = 0; i < data.size(); i++) {
    for (size_t j = 0; j < qids.size(); j++) {
      result[i][qids[j]] = generalizations[j];
    }
  }

  return result;
}

/*! Devuelve el número de cortes permitido por la partición.
  \return número de cortes.
*/
int Partition::getNumAllowedCuts() {
  return accumulate(allowedCuts.begin(), allowedCuts.end(), 0);
}

/*! Establece el número de cortes permitido por la partición.
  \param value número de cortes.
  \param dim dimensión (índice de qid) sobre la que actuan los cortes.
*/
void Partition::setAllowedCuts(int value, int dim) { allowedCuts[dim] = value; }

/*! Calcula una dimensión o qid según la anchura normalizada.
  \return dimensión.
*/
int Partition::chooseDimension() {
  int dimension, width;
  dimension = width = -1;

  for (size_t i = 0; i < qids.size(); i++) {
    if (allowedCuts[i] == 0)
      continue;

    int aux = normWidth(i);
    if (aux > width) {
      width = aux;
      dimension = i;
    }
  }

  return dimension;
}

/*! Devuelve el número normalizado de elementos incluidos en la partición.
  \param dimension dimensión.
  \return valor normalizado.
*/
int Partition::normWidth(int dimension) {
  // Rango normalizado de valores para la dimensión
  vector<string> elements;
  for (const vector<string> &entry : data) {
    elements.emplace_back(entry[qids[dimension]]);
  }

  auto it = unique(elements.begin(), elements.end());
  elements.resize(distance(elements.begin(), it));

  return elements.size();
}

/*! Calcula el valor medio de la partición. Utilizado para valores numéricos.
  \param dimension dimensión.
  \return valor normalizado.
*/
string Partition::findMedian(int dimension) {
  // Calculate qid frecuencies (qid values are sorted)
  map<string, int> map = calculateQidFreqs(this->data, qids[dimension]);
  vector<pair<string, int>> freqs;
  freqs.resize(map.size());
  copy(map.begin(), map.end(), freqs.begin());

  // Sort values
  sort(freqs.begin(), freqs.end(),
       [](const pair<string, int> &x, const pair<string, int> &y) {
         return stoi(x.first) < stoi(y.first);
       });

  // Sumar todos los valores
  vector<int> values;
  transform(freqs.begin(), freqs.end(), std::back_inserter(values),
            [](const auto &tuple) { return tuple.second; });
  int nValues = accumulate(values.begin(), values.end(), 0);

  // Encontrar valor medio
  double middle = nValues / 2.0;
  // Comprobar si se puede realizar el corte
  if (freqs.size() < 2 || middle < K || middle < L || middle < T) {
    return "";
  }

  // Calcular el valor final a partir de la lista de frecuencias
  string splitValue;
  int aux = 0;
  for (size_t i = 0; i < freqs.size(); i++) {
    aux += freqs[i].second;
    if (aux >= middle) {
      splitValue = freqs[i].first;
      break;
    }
  }

  return splitValue;
}

/*! Devuelve el rango sobre el que se situan los valores de una dimensión dada.
  \param dimension dimensión.
  \return vector de longitud 2.
*/
vector<int> Partition::getAttributeRanges(int dimension) {
  vector<int> values;
  for (const auto &record : this->data) {
    values.emplace_back(stoi(record[qids[dimension]]));
  }

  int min = *min_element(values.begin(), values.end());
  int max = *max_element(values.begin(), values.end());

  return {min, max};
}

/*! Comprueba si la partición es k-anonima.
  \param split partición.
  \return 1 si la partición es k-anonima, o 0 si no lo es.
*/
bool Partition::isSplitKAnonymous(vector<vector<string>> split) {
	// Obtener registros conteniendo unicamente qids
  vector<vector<string>> qidsDataset;
	for (size_t i=0; i < split.size(); i++) {
		vector<string> aux;
		for (const int& idx : qids) {
			aux.emplace_back(split[i][idx]);
		}
		qidsDataset.emplace_back(aux);
	}

  for (const int &freq : calculateFreqs(qidsDataset)) {
    if (freq < K) {
      return false;
    }
  }

  return true;
}

/*! Comprueba si la partición es l-diversa.
  \param split partición.
  \return 1 si la partición es l-diversa, o 0 si no lo es.
*/
bool Partition::isSplitLDiverse(vector<vector<string>> split) {
  vector<map<string, int>> freqs(confAtts.size());
  string key;

  for (const auto &entry : split) {
    for (size_t j = 0; j < confAtts.size(); j++) {
      key = entry[confAtts[j]];
      try {
        freqs[j][key] += 1;
      } catch (...) {
        freqs[j][key] = 1;
      }
    }
  }

  // Cada atributo confidencial debe tener, al menos,
  // l valores "bien representados"
  for (const map<string, int> &attFreq : freqs) {
    if ((int)attFreq.size() < L)
      return false;
  }

  return true;
}

/*! Comprueba si la partición cumple el modelo de privacidad t-closeness.
  \param split partición.
  \return 1 si la partición sí lo cumple, o 0 si no es así.
*/
bool Partition::isSplitTClose(vector<vector<string>> split) {
  if (split.size() == 0)
    return false;

  // Obtener tablas transpuestas
  vector<vector<string>> tSplit = transpose(split);
  vector<vector<string>> tData = transpose(data);

  // Generar un mapa de frecuencias de P
  vector<map<string, int>> splitMaps;
  for (const auto &att : confAtts) {
    map<string, int> freqs;
    for (const auto &entry : tSplit[att]) {
      try {
        freqs[entry] += 1;
      } catch (...) {
        freqs[entry] = 1;
      }
    }
    splitMaps.emplace_back(freqs);
  }

  // Generar un mapa de frecuencias de Q
  // y una lista de entradas
  vector<map<string, int>> dataMaps;
  vector<set<string>> valueSets;
  for (const auto &att : confAtts) {
    set<string> entries;
    map<string, int> freqs;
    for (const auto &entry : tData[att]) {
      entries.insert(entry);
      try {
        freqs[entry] += 1;
      } catch (...) {
        freqs[entry] = 1;
      }
    }
    dataMaps.emplace_back(freqs);
    valueSets.emplace_back(entries);
  }

  int pSize, qSize;
  pSize = split.size();
  qSize = data.size();

  // Calcular EMD para cada atributo confidencial
  // utilizando ED
  long double emd;
  for (size_t i = 0; i < confAtts.size(); i++) {
    emd = 0;
    for (const auto &entry : valueSets[i]) {
      if (splitMaps[i][entry]) {
        // Entrada presente en P
        // ED(Pi, Qi) = abs(Pi/Pi probability in P -
        // 					 Qi/Qi probability in Q)
        emd += abs((long double)splitMaps[i][entry] / pSize -
                   (long double)dataMaps[i][entry] / qSize);
      } else {
        // Entrada no presente P
        // ED(Pi, Qi) = abs(0 - Qi/Qi probability in Q)
        emd += (long double)dataMaps[i][entry] / qSize;
      }
    }

    // ED(P, Q) =  sum(sum(pj - qj)) / 2
    emd /= 2;

    // Comprobar t-closeness
    if (emd > T)
      return false;
  }

  return true;
}

/*! Comprueba si la partición cumple los modelos de privacidad seleccionados.
  \param split partición.
  \return 1 si la partición los cumple, o 0 si no es así.
*/
bool Partition::isSplitValid(vector<vector<string>> split) {
  bool kanonymity, ldiversity, tcloseness;
  kanonymity = ldiversity = tcloseness = true;

  if (split.size() == 0)
    return false;

  if (K > 0) {
    kanonymity = isSplitKAnonymous(split);
  }
  if (L > 0) {
    ldiversity = isSplitLDiverse(split);
  }
  if (T > 0) {
    tcloseness = isSplitTClose(split);
  }

  return kanonymity && ldiversity && tcloseness;
}

/*! Realiza un corte sobre una partición.
  \param dimension dimensión.
  \return vector de particiones.
*/
vector<Partition> Partition::splitPartition(int dimension) {
  // Corte no permitido para esta dimension
  if (allowedCuts[dimension] == 0) {
    return {};
  }

  if (isQidCat[dimension])
    return splitPartitionCategorical(dimension);
  return splitPartitionNumeric(dimension);
}

/*! Realiza un corte sobre una partición utilizando un atributo numérico.
  \param dimension dimensión.
  \return vector de particiones.
*/
vector<Partition> Partition::splitPartitionNumeric(int dimension) {
  // Obtener el valor medio
  string splitValue = findMedian(dimension);
  // Corte no permitido
  if (splitValue == "") {
    return {};
  }

  string split1, split2;
  split1 = splitValue;
  split2 = to_string(stoi(splitValue) + 1);

  // Obtener valores extremos presentes en la partición,
  string lowest, highest;
  vector<int> range = getAttributeRanges(dimension);
  lowest = to_string(range[0]);
  highest = to_string(range[1]);

  // Límite de cortes
  if (highest == split1) {
    this->setAllowedCuts(0, dimension);
    return {};
  }

  // Construir nuevas generalizaciones
  string gen1 = lowest == splitValue
                    ? splitValue
                    : (lowest == split1 ? lowest : lowest + "~" + split1);
  string gen2 = highest == splitValue
                    ? splitValue
                    : (highest == split2 ? highest : split2 + "~" + highest);

  // Nuevas particiones
  vector<vector<string>> d1, d2;
  for (const auto &record : this->data) {
    int number = stoi(record[qids[dimension]]);
    if (number <= stoi(split1))
      d1.emplace_back(record);
    else
      d2.emplace_back(record);
  }

  // generalizar d1
  vector<vector<string>> s1 = d1;
  for (size_t i = 0; i < s1.size(); i++) {
    for (size_t j = 0; j < generalizations.size(); j++)
      s1[i][qids[j]] = generalizations[j];
    s1[i][dimension] = gen1;
  }
  // generalizar d2
  vector<vector<string>> s2 = d1;
  for (size_t i = 0; i < s2.size(); i++) {
    for (size_t j = 0; j < generalizations.size(); j++)
      s2[i][qids[j]] = generalizations[j];
    s2[i][dimension] = gen2;
  }

  // Comprobación modelos de privacidad
  if (!isSplitValid(s1) || !isSplitValid(s2)) {
    this->setAllowedCuts(0, dimension);
    return {};
  }

  // Actualizar array de generalizaciones
  vector<string> gens1, gens2;
  gens1 = gens2 = this->generalizations;
  gens1[dimension] = gen1;
  gens2[dimension] = gen2;
  Partition p1(d1, gens1, qids, isQidCat, trees, confAtts, K, L, T);
  Partition p2(d2, gens2, qids, isQidCat, trees, confAtts, K, L, T);

  return {p1, p2};
}

/*! Realiza un corte sobre una partición utilizando un atributo categórico.
  \param dimension dimensión.
  \return vector de particiones.
*/
vector<Partition> Partition::splitPartitionCategorical(int dimension) {

  vector<Partition> pts;
  Tree tree = trees[qids[dimension]];
  string middle = this->generalizations[dimension];
  vector<string> children = tree.getDirectChildren(middle);

  // If it doesn't have children, return empty vector
  if (children.size() == 0)
    return pts;

  // Split records based on dimension (qid) generalization values
  vector<vector<vector<string>>> splits(children.size(),
                                        vector<vector<string>>());
  for (const auto &record : this->data) {
    string qid = record[qids[dimension]];

    int i = 0;
    for (const auto &child : children) {
      if (!tree.isChild(child, qid)) {
        i++;
        continue;
      }
      splits[i].emplace_back(record);
      break;
    }
  }

  // Generalizar split y comprobar modelos de privacidad
  for (size_t idx = 0; idx < splits.size(); idx++) {
    vector<vector<string>> aux = splits[idx];
    for (size_t i = 0; i < aux.size(); i++) {
      for (size_t j = 0; j < generalizations.size(); j++)
        aux[i][qids[j]] = generalizations[j];
      aux[i][dimension] = children[idx];
    }
  
    if (!isSplitValid(aux)) {
      this->setAllowedCuts(0, dimension);
      return pts;
    }
  }

  // Actualizar array de generalizaciones y actualizar
  // particiones
  for (size_t i = 0; i < splits.size(); i++) {
    if (splits[i].size() != 0) {
      vector<string> gens = generalizations;
      gens[dimension] = children[i];
      Partition p =
          Partition(splits[i], gens, qids, isQidCat, trees, confAtts, K, L, T);
      pts.emplace_back(p);
    }
  }

  return pts;
}
