/*! \file evaluate.cpp
    \brief Fichero que contiene todas las funciones principales sobre las que
           se construye el algoritmo de anonimización Datafly.
*/

#include "evaluate.h"

/*! Determina si el conjunto de datos se encuentra lista para la fase de
    supresión de registros.
  \param dataset conjunto de datos.
  \param qids índices de los atributos qids.
  \param tableSize cardinalidad de un conjunto de datos.
  \param K parámetro del modelo de privacidad k-anonymity.
  \param suppThreshold parámetro relacionado con el porcentaje
                       máximo de datos a suprimir.
  \return 1 si la tabla se encuentra lista para la supresión, 0 si no es así.
*/
bool readyForSuppression(vector<vector<string>> dataset, vector<int> qids,
                         const int tableSize, const int K,
                         const long double suppThreshold) {

  int count = 0;
  for (const auto &freq : calculateFreqs(dataset)) {
    if (freq < K)
      count += freq;
  }

  // Porcentaje de tuplas que pueden ser suprimidas: (loss * dataset.size() / 100)
  return (count <= (suppThreshold * tableSize) / 100);
}

/*! Bucle principal del algoritmo Datafly.
  \param dataset conjunto de datos.
  \param hierarchies conjunto de jerarquías (una por cada atributo).
  \param qids índices de los atributos qids.
  \param confAtts conjunto de atributos sensibles o SAs.
  \param suppThreshold parámetro relacionado con el porcentaje
                       máximo de datos a suprimir.
  \param K parámetro del modelo de privacidad k-anonymity.
  \return tupla conteniendo la tabla anonimizada y las distintas clases
          de equivalencia correspondientes.
*/
tuple<vector<vector<string>>, vector<vector<vector<string>>>>
datafly(vector<vector<string>> dataset,
        map<int, vector<vector<string>>> hierarchies, vector<int> qids,
        vector<int> confAtts, const long double suppThreshold, const int K) {
  vector<vector<string>> qidsDataset, result;
  vector<vector<vector<string>>> clusters;

  // Obtener subconjunto de datos conteniendo únicamente qids
	for (size_t i=0; i < dataset.size(); i++) {
		vector<string> aux;
		for (const int& idx : qids) {
			aux.emplace_back(dataset[i][idx]);
		}
		qidsDataset.emplace_back(aux);
	}

  // 1. Crear un árbol jerárquico para cada qid
  vector<Tree> trees;
  for (const int &val : qids) {
    trees.emplace_back(Tree(hierarchies[val]));
  }

  int idx;
  // 2&3. Calcular frecuencias y comprobar si la k-anonimidad se cumple
	while (!isKAnonSatisfied(qidsDataset, K)) {
    // Comprobar si la tabla esta lista para la fase de supresión
    if (readyForSuppression(qidsDataset, qids, dataset.size(), K,
                            suppThreshold)) {
      // 4. Suprimir registros que no sean k-anonimos (< K veces)
      supressRecords(qidsDataset, qids, K);
      break;
    }

    // 5. Encontrar el qid con el mayor número de valores únicos
    idx = findMostDistinctQid(qidsDataset);

    // 6. Generalizar el atributo qid
    try {
      generalizeQid(qidsDataset, idx, trees[idx]);
    } catch (const char *e) {
      cout << e << endl;
      return make_tuple(qidsDataset, clusters);
    }
  }

  // Actualizar conjunto de datos original con los valores generalizados
  result = dataset;
	for (size_t i=0; i < dataset.size(); i++) {
		for (size_t j=0; j < qids.size(); j++) {
			result[i][qids[j]] = qidsDataset[i][j];
		}
  }
  
  // Crear clases de equivalencia
  clusters = createClusters(result, qids);
  return make_tuple(result, clusters);
}
