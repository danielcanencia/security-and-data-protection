#include "datafly.h"

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
  // Leer nombres de qids
  vector<string> qidNames = readQidNames(nqids);
  vector<string> confAttNames;

  // Leer el directorio que contiene el conjunto de datos y las jerarquias
  vector<string> headers;
  vector<int> qids, confAtts;
  vector<vector<string>> qidsDataset, dataset;
  map<int, vector<vector<string>>> hierarchiesMap;

  try {
    hierarchiesMap =
        readDirectory(fs::path(argv[1]), dataset, headers, qidNames,
                      confAttNames, qids, confAtts, false);
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

    sort(qids.begin(), qids.end());
  } catch (const char *e) {
    cout << e << endl;
    return -1;
  }

  // Leer parámetros vinculados a los modelos de privacidad
  const int K = readParameter("k-anonymity", "K", dataset.size());
  if (K == -1) {
    cout << "Error, Datafly needs parameter K." << endl;
    return -1;
  }
  long double suppThreshold = readSuppThreshold();

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
  auto resTuple =
      datafly(dataset, hierarchiesMap, qids, confAtts, suppThreshold, K);
  vector<vector<string>> result = get<0>(resTuple);
  vector<vector<vector<string>>> clusters = get<1>(resTuple);
  if (result.size() == 0 || clusters.size() == 0)
    return 1;
  // *********************************
  auto stop = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::seconds>(stop - start);
  cout << endl << "===> Datafly Execution Time: ";
  cout << duration.count() << " seconds" << endl;

  cout << "===> Number of clusters: ";
  cout << clusters.size() << endl;

  // Escribir conjunto de datos anonimizado
  writeAnonymizedTable(fs::path(argv[1]), headers, result, K, -1, -1);

  // Métricas
  cout << "===> Analysis: " << endl;
  // Convertir árboles jerárquicos en un mapa de datos
  map<int, Tree> trees;
  for (const int &i : qids) {
    trees[i] = Tree(hierarchiesMap[i]);
  }

  // GCP
  try {
    // 1. Precalcular NCP para cada atributo qid
    vector<long double> cncps =
        calculateNCPS(clusters, weights, qids, numMetricsQids, trees);

    // 2. Calcular GCP
    calculateGCP(clusters, dataset.size(), qids, cncps);
  } catch (const char *e) {
    cout << e << endl;
    return -1;
  }

  // DM
  calculateDM(clusters, dataset.size(), K);

  // CAvg
  calculateCAVG(clusters, dataset.size(), K);

  // GenILoss
  try {
    calculateGenILoss(transpose(result), trees, qids, catMetricsQids,
                      numMetricsQids, dataset.size());
  } catch (const char *e) {
    cout << e << endl;
    return -1;
  }

  return 0;
}
