/*! \file evaluate.cpp
    \brief Fichero que contiene todas las funciones principales sobre las que
           se construye el algoritmo de anonimización Mondrian.
*/

#include "evaluate.h"

/*! Función inicial que da lugar a las sucecsivas particiones realizadas
    sobre el conjunto de datos inicial.
  \param partition partición inicial (contiene el conjunto de datos inicial).
  \return conjunto de datos anonimizados en forma de matriz de dimensión 3.
*/
vector<vector<vector<string>>> evaluate(Partition partition) {
  vector<Partition> result;
  evaluate(partition, result);

  vector<vector<vector<string>>> clusters;
  for (const auto &p : result) {
    vector<vector<string>> vec = p.getResult();
    clusters.emplace_back(vec);
  }

  return clusters;
}

/*! Contiene el bucle principal del algoritmo y realiza el método de particionado.
  \param partition partición inicial (contiene el conjunto de datos inicial).
  \param result contiene la lista de particiones auxiliar que contendrá el resultado
                final cuando finalize el algoritmo.
*/
void evaluate(Partition partition, vector<Partition> &result) {
  if (partition.getNumAllowedCuts() == 0) {
    result.emplace_back(partition);
    return;
  }

  int dim = partition.chooseDimension();
  vector<Partition> pts = partition.splitPartition(dim);

  if (pts.size() == 0) {
    partition.setAllowedCuts(0, dim);
    evaluate(partition, result);
    return;
  }

  for (const auto &p : pts)
    evaluate(p, result);

  return;
}

/*! Inicializa las estructuras necesarias para llevar a cabo el algoritmo
    Mondrian, haciendo uso de las funciones evaluate que realizan el particionado
    de los datos de entrada.
  \param dataset conjunto de datos.
  \param hierarchies conjunto de jerarquías (una por cada atributo).
  \param allQids índices de los atributos qids.
  \param isQidCat vector que contiene los tipos de cado uno de los atributos qids.
  \param confAtts conjunto de atributos sensibles o SAs.
  \param K parámetro del modelo de privacidad k-anonymity.
  \param L parámetro del modelo de privacidad l-diversity.
  \param T parámetro del modelo de privacidad t-closeness.
  \return conjunto de datos anonimizados en forma de matriz de dimensión 3.
*/
vector<vector<vector<string>>>
mondrian(vector<vector<string>> dataset,
         map<int, vector<vector<string>>> hierarchies, vector<int> allQids,
         vector<int> isQidCat, vector<int> confAtts, const int K, const int L,
         const long double T) {
  // 1. Create a hierarchy tree for every qid
  map<int, Tree> trees;
  // 2. Initialize default generalizations
  vector<string> gens;
  for (size_t i = 0; i < allQids.size(); i++) {
    if (isQidCat[i]) {
      // Generate a hierarchy tree for every categorical attribute
      trees[allQids[i]] = Tree(hierarchies[allQids[i]]);
      gens.emplace_back(trees[allQids[i]].root);
      continue;
    }

    // Numeric Value
    string numRoot = getNumericRoot(dataset, allQids[i]);
    gens.emplace_back(numRoot);
  }

  // 3. Anonymize whole initial partition
  Partition partition(dataset, gens, allQids, isQidCat, trees, confAtts, K, L,
                      T);

  return evaluate(partition);
}
