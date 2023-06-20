#include "mondrian.h"

int main(int argc, char **argv) {

  if (argc != 2) {
    cout << "\nInvalid arguments.\n"
            "Use ./datafly.out [data directory]\n\n"
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
  vector<int> catQids, confAtts, numQids, allQids;
  vector<int> isQidCat;
  map<int, vector<vector<string>>> hierarchiesMap;
  vector<vector<string>> dataset;

  try {
    hierarchiesMap =
        readDirectory(fs::path(argv[1]), dataset, headers, qidNames,
                      confAttNames, catQids, confAtts, false);
    sort(catQids.begin(), catQids.end());

    allQids = getQidsHeaders(headers, qidNames);
    if (allQids.size() < qidNames.size()) {
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

    sort(allQids.begin(), allQids.end());
    set_difference(allQids.begin(), allQids.end(), catQids.begin(),
                   catQids.end(), inserter(numQids, numQids.begin()));

    // Generar un vector conteniendo los tipos de cada qid
    for (const auto &qid : allQids) {
      if (find(catQids.begin(), catQids.end(), qid) != catQids.end()) {
        isQidCat.emplace_back(1);
        continue;
      }
      isQidCat.emplace_back(0);
    }

  } catch (const char *e) {
    cout << e << endl;
    return -1;
  }

  // Leer parámetros vinculados a los modelos de privacidad
  int K, L;
  long double T;
  if (!readParameters(dataset.size(), confAttNames.size(), K, L, T))
    return -1;

  // Leer pesos asignados a cada qid
  vector<double> weights = readWeights(nqids, qidNames);
  // Leer tipos de qids (importante considerar los atributos numéricos como tales)
  vector<int> numMetricsQids, catMetricsQids;
  tuple<vector<int>, vector<int>> metricsQids =
      readMetricsQids(numQids, catQids, headers);
  numMetricsQids = get<0>(metricsQids);
  catMetricsQids = get<1>(metricsQids);

  // Calcular el tiempo de ejecución
  auto start = chrono::high_resolution_clock::now();
  // *********************************
  // Algoritmo principal
  vector<vector<vector<string>>> clusters =
      mondrian(dataset, hierarchiesMap, allQids, isQidCat, confAtts, K, L, T);
  // *********************************
  auto stop = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::seconds>(stop - start);
  cout << endl << "===> Mondrian Execution Time: ";
  cout << duration.count() << " seconds" << endl;

  cout << "===> Number of clusters: ";
  cout << clusters.size() << endl;

  // Crear matriz de las clases de equivalencia
  vector<vector<string>> result;
  for (const auto &partition : clusters) {
    result.insert(result.begin(), partition.begin(), partition.end());
  }
  // Escribir conjunto de datos anonimizado
  writeAnonymizedTable(fs::path(argv[1]), headers, result, K, L, T);

  // Métricas
  cout << "===> Analysis: " << endl;
  // Convertir árboles jerárquicos en un mapa de datos
  map<int, Tree> trees;
  for (const int &i : catQids) {
    trees[i] = Tree(hierarchiesMap[i]);
  }

  // GCP
  try {
    // 1. Precalcular NCP para cada atributo qid
    vector<long double> cncps =
        calculateNCPS(clusters, weights, allQids, numMetricsQids, trees);
    // 2. Calcular GCP
    calculateGCP(clusters, dataset.size(), allQids, cncps);
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
    calculateGenILoss(transpose(result), trees, allQids, catMetricsQids,
                      numMetricsQids, dataset.size());
  } catch (const char *e) {
    cout << e << endl;
    return -1;
  }

  return 0;
}
