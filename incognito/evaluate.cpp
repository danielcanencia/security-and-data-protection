/*! \file evaluate.cpp
    \brief Fichero que proporciona las funciones principales del algoritmo
           incognito.
*/

#include "evaluate.h"

/*! Genera una lista de grafos basados en calcular las posibles permutaciones
    de una lista de cuasi-identificadores.
  \param qids lista de cuasi-identificadores.
  \param nodeMax define los valores máximos de generalización de cada qid.
  \param numAttr número de atributos de los que se componen las permutaciones.
  \param toPrune vector de nodos a tener en cuenta en la fase de poda del grafo.
  \return lista de grafos.
*/
vector<Graph> graphGeneration(const vector<int> &qids, map<int, int> nodeMax,
                              int numAttr, set<vector<int>> toPrune) {
  vector<Graph> graphs;
  vector<vector<int>> C;

  C = getPermutations(numAttr, qids);

  // Convert set a vector
  vector<vector<int>> pruneVector(toPrune.begin(), toPrune.end());

  // Generate trees for permutations of $perm qids
  for (int perm = 0; perm < (int)C.size(); perm++) {
    vector<int> CMaxValue;
    for (const auto &entry : C[perm]) {
      CMaxValue.emplace_back(nodeMax[entry]);
    }

    // Generar grafo
    GraphData graphData;
    // C[perm] => índice de qid actual
    // CMaxValue => máximos índices de qids
    Graph graph(CMaxValue, C[perm], pruneVector, graphData);
    graphs.emplace_back(graph);
  }

  return graphs;
}

/*! Genera un mapa de generalizaciones de cada atributo cuasi-identificador.
  \param hierarchy matriz de jerarquía.
  \param qids lista de cuasi-identificadores.
  \return mapa de generalizaciones.
*/
map<string, vector<string>>
generateGeneralizationMap(vector<vector<string>> hierarchy, vector<int> qids) {

  map<string, vector<string>> genMap;

  for (const auto &entry : hierarchy) {
    genMap[entry[0]] = entry;
  }

  return genMap;
}

/*! Genera mapas de generalizaciones para cada atributo cuasi-identificador.
  \param hierarchy mapa de matrices de jerarquías.
  \param qids lista de cuasi-identificadores.
  \return mapa de generalizaciones.
*/
map<int, map<string, vector<string>>>
generateGeneralizationsMap(map<int, vector<vector<string>>> hierarchies,
                           vector<int> qids) {

  map<int, map<string, vector<string>>> gensMap;
  for (const auto &qid : qids)
    gensMap[qid] = generateGeneralizationMap(hierarchies[qid], qids);

  return gensMap;
}

/*! Anonimiza un conjunto de datos.  
  \param dataset conjunto de datos.
  \param hierarchiesMap tablas de jerarquías.
  \param gens mapa de generalizaciones.
  \param graphs lista de grafos.
  \param qids lista de cuasi-identificadores.
  \return matriz de datos anonimizados.
*/
vector<vector<string>>
generateAnonymizedDataset(vector<vector<string>> dataset,
                          map<int, vector<vector<string>>> hierarchiesMap,
                          map<int, map<string, vector<string>>> gens,
                          vector<Graph> graphs, vector<int> qids) {
  // Seleccionar un nodo de entre todos los que satisfacen K.
  // Criterio: nodo que produzca el máximo número de clases de equivalencia.
  const GraphNode node = graphs.back().getFinalKAnon(gens, dataset, qids);
  vector<int> data = node.getData();

  map<int, map<string, string>> generalizations;
  for (size_t i = 0; i < qids.size(); i++) {
    const int qid = qids[i];
    map<string, string> qidMap;

    for (size_t j = 0; j < hierarchiesMap[qid][0].size(); j++)
      qidMap[hierarchiesMap[qid][0][j]] = hierarchiesMap[qid][data[i]][j];

    generalizations[qid] = qidMap;
  }

  vector<vector<string>> result;
  for (size_t i = 0; i < dataset.size(); i++) {
    vector<string> row;
    for (size_t j = 0; j < dataset[0].size(); j++) {
      auto it = find(qids.begin(), qids.end(), j);
      if (it != qids.end()) {
        row.emplace_back(generalizations[j][dataset[i][j]]);
        continue;
      }
      row.emplace_back(dataset[i][j]);
    }
    result.emplace_back(row);
  }

  return result;
}

/*! Función principal sobre la que actua incognito.  
  \param dataset conjunto de datos.
  \param hierarchies tablas de jerarquías.
  \param qids lista de cuasi-identificadores.
  \param confAtts lista de índices de atributos sensibles.
  \param K parámetro de la k-anonimidad.
  \param L parámetro de la l-diversidad.
  \param T parámetro de t-closeness.
  \return tupla compuesta por la matriz de datos anonimizados y el conjunto de clases
          de equivalencia que lo forman.
*/
tuple<vector<vector<string>>, vector<vector<vector<string>>>>
incognito(vector<vector<string>> dataset,
          map<int, vector<vector<string>>> hierarchies, vector<int> qids,
          vector<int> confAtts, const int K, const int L, const long double T) {

  // Generar datos auxiliares a utilizar para determinar t-closeness
  tuple<vector<map<string, int>>, vector<set<string>>> dataMap;
  if (T != -1)
    dataMap = createDataMap(dataset, confAtts);

  // Niveles de generalización máximos (jerarquias) de cada qid
  map<int, int> nodeMax;
  for (const int &qid : qids) {
    nodeMax[qid] = hierarchies[qid].size() - 1;
  }
  // Mapas de jerarquias transpuestos
  map<int, vector<vector<string>>> transposedHierarchies;
  for (const auto &qid : qids) {
    transposedHierarchies[qid] = transpose(hierarchies[qid]);
  }
  // Conjunto de datos transpuesto
  vector<vector<string>> transposedDataset;
  transposedDataset = transpose(dataset);

  // Generate a generalization map
  map<int, map<string, vector<string>>> gensMap =
      generateGeneralizationsMap(transposedHierarchies, qids);

  // generar todos los posibles grafos en función de los qids y
  // los niveles de generalización máximos (nodeMax)
  vector<Graph> graphs = graphGeneration(qids, nodeMax, 1, {});

  // Procedimiento principal del algoritmo
  vector<Graph> rGraphs;
  set<vector<int>> pruningVector;
  for (size_t i = 1; i < qids.size() + 1; i++) {
    for (size_t gsize = 0; gsize < graphs.size(); gsize++) {
      Graph g = graphs[gsize];
      set<GraphNode> nodesQueue = g.getLeaves();
      vector<GraphNode> kanon;

      // Bucle principal
      while (!nodesQueue.empty()) {
        GraphNode node = *(nodesQueue.begin());
        nodesQueue.erase(nodesQueue.begin());

        if (!g.isNodeMarked(node)) {
          // No marcado
          if (node.isAnonymityValid(hierarchies, dataset, gensMap, dataMap,
                                    g.getQids(), confAtts, K, L, T)) {
            g.markGeneralizations(node);
          } else {
            // Añadir nodo al vector de poda
            pruningVector.insert(node.getData());
            g.addGeneralizations(node, nodesQueue);
          }
        }
      }

      rGraphs.emplace_back(g);
    }

    // Generar grafos
    if (i <= qids.size())
      graphs = graphGeneration(qids, nodeMax, i + 1, pruningVector);
  }

  // Construir dataset anonimizado
  vector<vector<string>> result =
      generateAnonymizedDataset(dataset, hierarchies, gensMap, rGraphs, qids);

  // Crear clases de equivalencia
  return make_tuple(result, createClusters(result, qids));
}
