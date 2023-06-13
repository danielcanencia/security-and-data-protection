/*! \file generalize.cpp
    \brief Fichero que contiene todas las funciones relacionadas con las
           técnicas de generalización y supresión de datos utilizadas por
           Datafly, así como la comprobación del modelo de privacidad
           k-anonymity.
*/

#include "generalize.h"

/*! Comprueba si el conjunto de datos satisface el modelo de privacidad
    k-anonymity.
  \param dataset conjunto de datos.
  \param K parámetro del modelo de privacidad k-anonymity.
  \return 1 si el modelo de privacidad se satisface, 0 en otro caso.
*/
bool isKAnonSatisfied(const vector<vector<string>> dataset, const int K) {

  for (const int &freq : calculateFreqs(dataset)) {
    if (freq < K) {
      return false;
    }
  }

  return true;
}

/*! Generaliza un conjunto de datos utilizando generalización global sobre
    un atributo cuasi-identificador dado.
  \param dataset conjunto de datos.
  \param qid índice del atributo cuasi-identificador o qid.
  \param tree árbol de jerarquía dell atributo qid.
*/
void generalizeQid(vector<vector<string>> &dataset, const int qid, Tree tree) {

  for (size_t i = 0; i < dataset.size(); i++)
    dataset[i][qid] = tree.getNextGen(dataset[i][qid]);
}

/*! Suprime los registros cuya frecuencia de su conjunto de qids sea
    menor a K.
  \param dataset conjunto de datos.
  \param qids índices de los atributos qids.
  \param K parámetro de la k-anonimidad.
*/
void supressRecords(vector<vector<string>> &dataset, vector<int> qids,
                    const int K) {
  // Calculate frequencies and indexes
  map<string, tuple<int, vector<int>>> records =
      evaluateFrequencyByIdx(dataset, qids);

  // Suppress records with freqs < K
  vector<int> deletions;
  for (const auto &[key, tuple] : records) {
    int freq = get<0>(tuple);
    vector<int> idxs = get<1>(tuple);

    if (freq < K) {
      deletions.insert(deletions.begin(), idxs.begin(), idxs.end());
    }
  }

  sort(deletions.begin(), deletions.end());
  int n = 0;
  for (const auto &i : deletions) {
    dataset.erase(dataset.begin() + i - n);
    n++;
  }
}
