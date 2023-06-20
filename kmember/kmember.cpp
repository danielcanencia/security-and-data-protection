/*! \file kmember.cpp
    \brief Fichero que contiene las funciones principales sobre las
           que actua el algoritmo k-member.
*/

#include "kmember.h"

/*! Generaliza un conjunto de datos.
  \param data conjunto de datos.
  \param trees árbol jerárquicos de los atributos qids cateǵoricos
  \param numQids lista de índices de atributos numéricos.
  \param catQids lista de índices de atributos categóricos.
  \param clusters clases de equivalencia.
  \return valor del ancestro común.
*/
map<int, vector<vector<string>>>
generalize(map<int, vector<vector<string>>> data, map<int, Tree> trees,
           vector<int> numQids, vector<int> catQids, int clusters) {
  // Generalizar valores en función de las clases de equivalencia (global recoding)
  for (int i = 0; i < clusters; i++) {
    vector<vector<string>> cluster = data[i];
    vector<vector<string>> matrix = transpose(cluster);

    // Generalizar atributos qids categóricos
    for (const auto &qid : catQids) {
      // Generalizar valores utilizando su ancestro común
      string gen = trees[qid].getLowestCommonAncestor(matrix[qid]).value;
      for (size_t idx = 0; idx < cluster.size(); idx++)
        data[i][idx][qid] = gen;
    }

    // Generalizar atributos qids numéricos por rangos
    for (const auto &qid : numQids) {
      double max = stod(
          *max_element(matrix[qid].begin(), matrix[qid].end(),
                       [](string a, string b) { return stod(a) < stod(b); }));
      double min = stod(
          *min_element(matrix[qid].begin(), matrix[qid].end(),
                       [](string a, string b) { return stod(a) < stod(b); }));

      for (size_t idx = 0; idx < cluster.size(); idx++)
        data[i][idx][qid] = to_string(min) + '~' + to_string(max);
    }
  }

  return data;
}

/*! Rutina principal del algoritmo k-member.
  \param dataset conjunto de datos.
  \param hierarchies mapa de jerarquias.
  \param numQids lista de índices de atributos numéricos.
  \param catQids lista de índices de atributos categóricos.
  \param confAtts lista de índices de atributos sensibles.
  \param K parámetro del modelo de privacidad k-anonymity.
  \param L parámetro del modelo de privacidad l-diversity.
  \param sensitiveValues valores realmente sensibles.
  \param diversityPenalty penalización de diversidad.
  \param diversity tipo de métrica a utilizar para calcular l-diversity.
  \param count número de iteraciones del bucle principal.
  \return conjunto de datos anonimizado (en clases de equivalencia) en forma de mapa.
*/
map<int, vector<vector<string>>>
evaluate(vector<vector<string>> dataset,
         map<int, vector<vector<string>>> hierarchies, vector<int> numQids,
         vector<int> catQids, const int confAtt, const int K, const int L,
         const vector<string> sensitiveValues, const int diversityPenalty,
         const int diversity, int &count) {
  // Crea un árbol jerárquico para cada atributo qid
  map<int, Tree> trees;
  for (const int &i : catQids) {
    trees[i] = Tree(hierarchies[i]);
  }

  // Algoritmo principal
  map<int, vector<vector<string>>> res;
  if (dataset.size() <= (size_t)K) {
    res[0] = dataset;
    count = 1;
    return generalize(res, trees, numQids, catQids, count);
  }

  vector<vector<string>> S = dataset;
  int r = randomRecord(S);
  vector<vector<string>> c;
  vector<string> aux = S[r];

  // 1º bucle
  while ((int)S.size() >= K) {
    r = furthestRecord(aux, r, S, hierarchies, trees, numQids, catQids);
    vector<vector<string>> c(1, S[r]);
    S.erase(S.begin() + r);

    while ((int)c.size() < K) {
      r = findBestRecord(S, c, hierarchies, trees, numQids, catQids, confAtt, L,
                         sensitiveValues, diversityPenalty, diversity);

      c.emplace_back(S[r]);
      S.erase(S.begin() + r);
    }

    aux = c.back();
    res[count] = c;
    count += 1;
  }

  // 2º bucle
  int idx;
  while (S.size() > 0) {
    r = randomRecord(S);
    idx = findBestCluster(res, S[r], hierarchies, trees, numQids, catQids);
    res[idx].emplace_back(S[r]);
    S.erase(S.begin() + r);
  }

  return generalize(res, trees, numQids, catQids, count);
}

int main(int argc, char **argv) {

  if (argc != 2) {
    cout << "\nInvalid arguments.\n"
            "Use ./kmember [data directory]\n\n"
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
  // Leer los nombres de los qids
  vector<string> qidNames = readQidNames(nqids);
  // Leer atributos sensibles
  string attName = readConfidentialAttName();
  vector<string> confAttNames;
  if (attName.size() != 0)
    confAttNames.emplace_back(attName);

  // Leer el directorio que contiene el conjunto de datos y las jerarquias
  vector<string> headers;
  vector<int> catQids, numQids, allQids;
  vector<int> confAtts;
  vector<vector<string>> hierarchies, dataset;
  map<int, vector<vector<string>>> hierarchiesMap;

  try {
    hierarchiesMap =
        readDirectory(fs::path(argv[1]), dataset, headers, qidNames,
                      confAttNames, catQids, confAtts, false);
    if (catQids.size() < catQids.size()) {
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

    sort(catQids.begin(), catQids.end());
    allQids = getQidsHeaders(headers, qidNames);

    sort(allQids.begin(), allQids.end());
    set_difference(allQids.begin(), allQids.end(), catQids.begin(),
                   catQids.end(), inserter(numQids, numQids.begin()));

  } catch (const char *e) {
    cout << e << endl;
    return -1;
  }

  // Leer el atributo sensible
  int confAtt = -1;
  if (confAttNames.size() != 0)
    confAtt = confAtts[0];

  // Leer parámetros vinculados a los modelos de privacidad
  const int K = readParameter("k-anonymity", "K", dataset.size());
  if (K == -1) {
    cout << "Kmember needs parameter K" << endl;
    return 1;
  }
  const int L = readParameter("l-diversity", "L", dataset.size());
  if (L != -1 && confAtt == -1) {
    cout << endl;
    cout << "An error occured.\nIf l-diversity or t-closeness "
            "are used, there should exists, at least, one "
            "confidential attribute."
         << endl
         << endl;
    return -1;
  }

  // Leer la penalización de diversidad y la métrica utilizada
  int diversityPenalty, diversity;
  vector<string> sensitiveValues;
  if (L != -1) {
    diversityPenalty = readDiversityPenalty();
    diversity = readDiversity();

    // Leer valores sensibles para SDM
    if (diversity == 1) {
      vector<string> values = transpose(hierarchiesMap[confAtt])[0];
      sensitiveValues = readSensitiveValues(values);
    }
  }

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
  int count = 0;
  map<int, vector<vector<string>>> res =
      evaluate(dataset, hierarchiesMap, numQids, catQids, confAtt, K, L,
               sensitiveValues, diversityPenalty, diversity, count);
  // *********************************
  auto stop = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::seconds>(stop - start);
  cout << endl << "===> Kmember Execution Time: ";
  cout << duration.count() << " seconds" << endl;

  cout << "===> Number of clusters: ";
  cout << count << endl;

  // Escribir conjunto de datos anonimizado
  cout << "===> Writing data" << endl;
  string directory = argv[1];
  if (directory.back() != '/')
    directory += "/";

  // Crear matriz de las clases de equivalencia
  vector<vector<string>> result;
  for (int i = 0; i < count; i++)
    result.insert(result.begin(), res[i].begin(), res[i].end());

  writeAnonymizedTable(fs::path(directory), headers, result, K, L, -1);

  // Métricas
  vector<vector<vector<string>>> clusters;
  for (const auto &[k, cluster] : res)
    clusters.emplace_back(cluster);
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
