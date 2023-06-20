#include "incognito.h"

int main(int argc, char **argv) {

  if (argc != 2) {
    cout << "\nInvalid arguments.\n"
            "Use ./incognito [data directory] [k]\n\n"
            "* Make sure your data directory meets the following structure:\n"
            "  (check dataset folder for an example)\n\n"
            " |-- [data directory]\n"
            "      |\n"
            "      |-- csv input file\n"
            "      |-- hierarchies\n"
            "           |\n"
            "           |-- hierarchy tables as csv files\n"
            "           |-- ....\n"
         << endl;
    return -1;
  }

  // Leer parámetros
  const int nqids = readNumberOfQids();
  vector<string> qidNames;
  vector<string> confAttNames;
  try {
    // Leer nombres de qids
    qidNames = readQidNames(nqids);
    // Leer atributos sensibles
    confAttNames = readConfidentialAttNames();
  } catch (const char *e) {
    cout << e << endl;
    return -1;
  }

  // Leer el directorio que contiene el conjunto de datos y las jerarquias
  vector<string> headers;
  vector<int> qids, confAtts;
  vector<vector<string>> dataset, transposedDataset;
  map<int, vector<vector<string>>> hierarchiesMap;
  map<int, vector<vector<string>>> transposedHierarchyMap;

  try {
    hierarchiesMap =
        readDirectory(fs::path(argv[1]), dataset, headers, qidNames,
                      confAttNames, qids, confAtts, true);
    sort(qids.begin(), qids.end());
    if (qids.size() < qidNames.size()) {
      cout << endl << "******************" << endl;
      cout << "An error occured.\nCheck the qid "
              "names entered exists. They should be "
              "referenced\nin their respectives "
              "hierarchy files."
           << endl
           << endl;
      return -1;
    }

    if (confAtts.size() < confAttNames.size()) {
      cout << endl << "******************" << endl;
      cout << "An error occured.\nCheck the confidential "
              "attributte names entered exists.\nThey should be "
              "referenced in their respectives "
              "hierarchy files."
           << endl
           << endl;
      return -1;
    }

    for (const auto &qid : qids) {
      transposedHierarchyMap[qid] = transpose(hierarchiesMap[qid]);
    }

    transposedDataset = transpose(dataset);
  } catch (const char *e) {
    cout << e << endl;
    return -1;
  }

  // Leer parámetros vinculados a los modelos de privacidad
  int K, L;
  long double T;
  if(!readParameters(dataset.size(), confAttNames.size(), K, L, T))
    return -1;

  // Leer pesos asignados a cada qid
  vector<double> weights = readWeights(nqids, qidNames);
  // Leer tipos de qids (importante considerar los atributos numéricos como tales)
  vector<int> numMetricsQids, catMetricsQids;
  tuple<vector<int>, vector<int>> metricsQids =
      readMetricsQids({}, qids, headers);
  numMetricsQids = get<0>(metricsQids);
  catMetricsQids = get<1>(metricsQids);

  // Calcular el tiempo de ejecución
  auto start = chrono::high_resolution_clock::now();
  // *********************************
  // Algoritmo principal
  auto resTuple = incognito(dataset, hierarchiesMap, qids, confAtts, K, L, T);
  vector<vector<string>> result = get<0>(resTuple);
  vector<vector<vector<string>>> clusters = get<1>(resTuple);
  // *********************************
  auto stop = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::seconds>(stop - start);
  cout << endl << "===> Incognito Execution Time: ";
  cout << duration.count() << " seconds" << endl;

  cout << "===> Number of clusters: ";
  cout << clusters.size() << endl;

  // Escribir conjunto de datos anonimizado
  writeAnonymizedTable(fs::path(argv[1]), headers, result, K, L, T);

  // Crear un árbol jerárquico por cada qid categórico
  vector<Tree> trees;
  for (const int &val : qids) {
    trees.emplace_back(Tree(transposedHierarchyMap[val]));
  }

  // Métricas
  cout << "===> Analysis: " << endl;
  // Convertir árboles jerárquicos en un mapa de datos
  map<int, Tree> treeMap;
  for (size_t i = 0; i < qids.size(); i++) {
    treeMap[qids[i]] = trees[i];
  }

  // GCP
  try {
    // 1. Precalcular NCP para cada atributo qid
    vector<long double> cncps =
        calculateNCPS(clusters, weights, qids, numMetricsQids, treeMap);

    // 2. Calcular GCP
    calculateGCP(clusters, dataset.size(), qids, cncps);
  } catch (const char *e) {
    cout << e << endl;
    return -1;
  }

  if (K != -1) {
    // DM
    calculateDM(clusters, dataset.size(), K);

    // CAvg
    calculateCAVG(clusters, dataset.size(), K);
  }

  // GenILoss
  try {
    calculateGenILoss(transpose(result), treeMap, qids, catMetricsQids,
                      numMetricsQids, dataset.size());
  } catch (const char *e) {
    cout << e << endl;
    return -1;
  }

  return 0;
}
