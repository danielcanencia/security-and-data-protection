#include "evaluate.h"

vector<Graph> graphGeneration(const vector<int> &qids, map<int, int> nodeMax,
                              int numAttr, set<vector<int>> toPrune) {
  vector<Graph> graphs;
  vector<vector<int>> C;

  C = getPermutations(numAttr, qids);

  // Convert from set to vector
  vector<vector<int>> pruneVector(toPrune.begin(), toPrune.end());

  // Generate trees for permutations of $perm qids
  for (int perm = 0; perm < (int)C.size(); perm++) {
    vector<int> CMaxValue;
    for (const auto &entry : C[perm]) {
      CMaxValue.emplace_back(nodeMax[entry]);
    }

    // Generate Graph
    GraphData graphData;
    // C[perm] => curr qid indexes
    // CMaxValue => max qid indexes
    Graph graph(CMaxValue, C[perm], pruneVector, graphData);
    graphs.emplace_back(graph);
  }

  return graphs;
}

map<string, vector<string>>
generateGeneralizationMap(vector<vector<string>> hierarchy, vector<int> qids) {

  map<string, vector<string>> genMap;

  for (const auto &entry : hierarchy) {
    genMap[entry[0]] = entry;
  }

  return genMap;
}

map<int, map<string, vector<string>>>
generateGeneralizationsMap(map<int, vector<vector<string>>> hierarchies,
                           vector<int> qids) {

  map<int, map<string, vector<string>>> gensMap;
  for (const auto &qid : qids)
    gensMap[qid] = generateGeneralizationMap(hierarchies[qid], qids);

  return gensMap;
}

vector<vector<string>>
generateAnonymizedDataset(vector<vector<string>> dataset,
                          map<int, vector<vector<string>>> hierarchiesMap,
                          map<int, map<string, vector<string>>> gens,
                          vector<Graph> graphs, vector<int> qids) {

  // Select one solution among the ones that satisfy K
  // Criteria: Generalization/Node that produces the maximum amount
  // 			 of equivalence classes
  const GraphNode node = graphs.back().getFinalKAnon(gens, dataset, qids);
  vector<int> data = node.getData();

  map<int, map<string, string>> generalizations;
  for (size_t i = 0; i < qids.size(); i++) {
    const int qid = qids[i];
    map<string, string> qidMap;
    for (size_t j = 0; j < hierarchiesMap[qid][0].size(); j++) {
      qidMap[hierarchiesMap[qid][0][j]] = hierarchiesMap[qid][data[i]][j];
    }
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

tuple<vector<vector<string>>, vector<vector<vector<string>>>>
incognito(vector<vector<string>> dataset,
          map<int, vector<vector<string>>> hierarchies, vector<int> qids,
          vector<int> confAtts, const int K, const int L, const long double P) {
  // Levels per hierchary. Useful to construct node
  // and edges tables
  map<int, int> nodeMax;
  for (const int &qid : qids) {
    nodeMax[qid] = hierarchies[qid].size() - 1;
  }
  // Transpose hierarchy maps
  map<int, vector<vector<string>>> transposedHierarchies;
  for (const auto &qid : qids) {
    transposedHierarchies[qid] = transpose(hierarchies[qid]);
  }
  // Transpose dataset
  vector<vector<string>> transposedDataset;
  transposedDataset = transpose(dataset);

  // Generate a generalization map
  map<int, map<string, vector<string>>> gensMap =
      generateGeneralizationsMap(transposedHierarchies, qids);

  // Generate all posible graphs containing qids
  // defined by qid variable
  vector<Graph> graphs = graphGeneration(qids, nodeMax, 1, {});

  // Main Algorithm
  vector<Graph> rGraphs;
  set<vector<int>> pruningVector;
  for (size_t i = 1; i < qids.size() + 1; i++) {
    for (size_t gsize = 0; gsize < graphs.size(); gsize++) {
      Graph g = graphs[gsize];
      set<GraphNode> nodesQueue = g.getRoots();
      vector<GraphNode> kanon;

      // Main Loop
      while (!nodesQueue.empty()) {
        GraphNode node = *(nodesQueue.begin());
        nodesQueue.erase(nodesQueue.begin());

        if (!g.isNodeMarked(node)) {
          // Not marked
          if (node.isAnonymityValid(hierarchies, dataset, gensMap, g.getQids(),
                                    confAtts, K, L, P)) {
            g.markGeneralizations(node);
          } else {
            // Add node to pruning vector
            pruningVector.insert(node.getData());
            g.addGeneralizations(node, nodesQueue);
          }
        }
      }

      rGraphs.emplace_back(g);
    }

    // Generate graphs
    if (i <= qids.size())
      graphs = graphGeneration(qids, nodeMax, i + 1, pruningVector);
  }

  // Construct anonymized dataset
  vector<vector<string>> result =
      generateAnonymizedDataset(dataset, hierarchies, gensMap, rGraphs, qids);

  // Create equivalence classes or clusters
  return make_tuple(result, createClusters(result, qids));
}
