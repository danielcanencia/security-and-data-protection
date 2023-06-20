/*! \file anonymity.cpp
    \brief Fichero que proporciona las funciones que comprueban si una
           partición cumple con los modelos de privacidad.
*/

#include "anonymity.h"

/*! Comprueba si una partición es k-anonima.
  \param split partición.
  \param qids lista de cuasi-identificadores.
  \param K parámetro de la k-anonimidad.
  \return 1 si la partición es k-anonima, o 0 si no es así.
*/
bool isSplitKAnonymous(vector<vector<string>> split, vector<int> qids,
                       const int K) {
  // Obtener registros conteniendo únicamente qids
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

/*! Comprueba si una partición es l-diversa.
  \param split partición.
  \param confAtts lista de índices de atributos sensibles.
  \param L parámetro de la l-diversidad.
  \return 1 si la partición es k-anonima, o 0 si no es así.
*/
bool isSplitLDiverse(vector<vector<string>> split, vector<int> confAtts,
                     const int L) {
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

  // Cada atributo debe tener, al menos, l valores
  // "bien representados"
  for (const map<string, int> &attFreq : freqs) {
    if ((int)attFreq.size() < L)
      return false;
  }

  return true;
}

/*! Comprueba si una partición cumple el modelo de privacidad t-closeness.
  \param split partición.
  \param data matriz de datos anonimizados.
  \param dataMap matriz de datos sin anonimizar.
  \param confAtts lista de índices de atributos sensibles.
  \param T parámetro de t-closeness.
  \return 1 si la partición es k-anonima, o 0 si no es así.
*/
bool isSplitTClose(vector<vector<string>> split, vector<vector<string>> data,
                   tuple<vector<map<string, int>>, vector<set<string>>> dataMap,
                   vector<int> confAtts, const long double T) {

  if (split.size() == 0)
    return false;

  // Generar un mapa de frecuencias P, para cada atributo
  // confidencials
  string key;
  vector<map<string, int>> splitMaps(confAtts.size());
  for (const auto &entry : split) {
    for (size_t j = 0; j < confAtts.size(); j++) {
      key = entry[confAtts[j]];
      try {
        splitMaps[j][key] += 1;
      } catch (...) {
        splitMaps[j][key] = 1;
      }
    }
  }

  vector<map<string, int>> dataFreqs = get<0>(dataMap);
  vector<set<string>> valueSets = get<1>(dataMap);

  int pSize, qSize;
  pSize = split.size();
  qSize = data.size();

  // Calculat EMD para cada atributo confidencial
  // utilizando ED
  for (size_t i = 0; i < confAtts.size(); i++) {
    long double emd = 0;
    for (const auto &entry : valueSets[i]) {
      if (splitMaps[i][entry]) {
        // Entrada presente en P
        // ED(Pi, Qi) = abs(Pi/Pi probability in P -
        //					 Qi/Qi probability in Q)
        emd += abs((long double)splitMaps[i][entry] / pSize -
                   (long double)dataFreqs[i][entry] / qSize);
      } else {
        // Entrada no presente en P
        // ED(Pi, Qi) = abs(0 - Qi/Qi probability in Q)
        emd += (long double)dataFreqs[i][entry] / qSize;
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

/*! Comprueba si una partición cumple el modelo de privacidad t-closeness.
  \param splits particiones.
  \param dataMap matriz de datos sin anonimizar.
  \param dataset conjunto de datos.
  \param qids lista de atributos cuasi-identificadores.
  \param confAtts lista de índices de atributos sensibles.
  \param K parámetro de la k-anonimidad.
  \param L parámetro de la l-diversidad.
  \param T parámetro de t-closeness.
  \return 1 si la partición es k-anonima, o 0 si no es así.
*/
bool isSplitValid(vector<vector<vector<string>>> splits,
                  tuple<vector<map<string, int>>, vector<set<string>>> dataMap,
                  vector<vector<string>> dataset, vector<int> qids,
                  vector<int> confAtts,
                  const int K, const int L, const long double T) {
  bool kanonymity, ldiversity, tcloseness;
  kanonymity = ldiversity = tcloseness = true;

  if (splits.size() == 0)
    return false;

  if (K > 0) {
    for (const auto &split : splits) {
      if (!isSplitKAnonymous(split, qids, K)) {
        kanonymity = false;
        break;
      }
    }
  }
  if (L > 0) {
    for (const auto &split : splits) {
      if (!isSplitLDiverse(split, confAtts, L)) {
        ldiversity = false;
        break;
      }
    }
  }
  if (T > 0) {
    for (const auto &split : splits) {
      if (!isSplitTClose(split, dataset, dataMap, confAtts, T)) {
        tcloseness = false;
        break;
      }
    }
  }

  return kanonymity && ldiversity && tcloseness;
}
