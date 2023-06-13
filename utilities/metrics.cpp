/*! \file metrics.cpp
    \brief Fichero que contiene todas la implementación de las métricas
           DM, CAvg y GenILoss.
*/

#include "metrics.h"

#define GET_NAME(Var) (#Var)

/*! Calcula y muestra por pantalla el valor de la métrica Discernibility Metric
    o DM.
  \param clusters clases de equivalencia de las que se componen los datos
                  anonimizados.
  \param tableSize cardinalidad de la tabla original.
*/
void calculateDM(vector<vector<vector<string>>> clusters, const int tableSize,
                 const int K) {
  long double dm = 0;
  for (const vector<vector<string>> &cluster : clusters) {
    if ((long double)cluster.size() >= K)
      dm += pow(cluster.size(), 2.0);
    else
      dm += tableSize * cluster.size();
  }

  cout << "\t* DM: ";
  cout << fixed << setprecision(3) << dm << endl;
}

/*! Calcula y muestra por pantalla el valor de la métrica Equivalent Class
    Size Metric o CAvg.
  \param clusters clases de equivalencia de las que se componen los datos
                  anonimizados.
  \param tableSize cardinalidad de la tabla original.
  \param K parámetro de la k-anonimidad.
*/
void calculateCAVG(vector<vector<vector<string>>> clusters, const int tableSize,
                   const int K) {
  const long double denom = (long double)(clusters.size() * K);
  const long double cavg = (denom == 0) ? 0 : tableSize / denom;

  cout << "\t* CAvg: ";
  cout << fixed << setprecision(3);
  cout << cavg << endl;
}

/*! Calcula el valor máximo de entre una lista de valores numéricos dados.
  \param entries lista de valores numéricos.
  \return valor máximo.
*/
long double calculateMaxNumValue(vector<string> entries) {
  try {
    stold(entries[0]);
  } catch (const exception &e) {
    throw "\nError, some categorical attribute "
          " can't be treated as numerical\n";
  }

  // Max Value
  string globalMax =
      (*max_element(entries.begin(), entries.end(),
                    [&](const string &a, const string &b) {
                      string str1, str2;
                      size_t posA = a.find("~");
                      size_t posB = b.find("~");
                      if (posA == string::npos)
                        str1 = a;
                      else
                        str1 = a.substr(a.find("~") + 1, a.size());

                      if (posB == string::npos)
                        str2 = b;
                      else
                        str2 = b.substr(b.find("~") + 1, b.size());

                      return strtold(str1.c_str(), NULL) <
                             strtold(str2.c_str(), NULL);
                    }))
          .c_str();

  if (globalMax.find("~") == string::npos)
    return strtold(globalMax.c_str(), NULL);

  return strtold(
      globalMax.substr(globalMax.find("~") + 1, globalMax.size()).c_str(),
      NULL);
}

/*! Calcula el valor mínimo de entre una lista de valores numéricos dados.
  \param entries lista de valores numéricos.
  \return valor mínimo.
*/
long double calculateMinNumValue(vector<string> entries) {

  // Max Value
  string globalMin = (*min_element(entries.begin(), entries.end(),
                                   [&](const string &a, const string &b) {
                                     string str1, str2;
                                     size_t posA = a.find("~");
                                     size_t posB = b.find("~");
                                     if (posA == string::npos)
                                       str1 = a;
                                     else
                                       str1 = a.substr(0, a.find("~"));

                                     if (posB == string::npos)
                                       str2 = b;
                                     else
                                       str2 = b.substr(0, b.find("~"));

                                     return strtold(str1.c_str(), NULL) <
                                            strtold(str2.c_str(), NULL);
                                   }))
                         .c_str();

  if (globalMin.find("~") == string::npos)
    return strtold(globalMin.c_str(), NULL);

  return strtold(globalMin.substr(0, globalMin.find("~")).c_str(), NULL);
}

/*! Calcula el valor de la métrica GenILoss para los valores numéricos.
  \param entry valor sobre el que aplicar la métrica.
  \param entries lista de valores numéricos.
  \param globalMax valor numérico máximo.
  \param globalMin valor numérico mínimo.
  \return valor de la métrica.
*/
long double calculateNumGenILoss(const string entry, vector<string> entries,
                                 const long double globalMax,
                                 const long double globalMin) {
  const int delimeterPos = entry.find("~");

  const long double min = strtold(entry.substr(0, delimeterPos).c_str(), NULL);
  const long double max =
      strtold(entry.substr(delimeterPos + 1, entry.size()).c_str(), NULL);

  if ((max - min) == 0.0 || (globalMax - globalMin) == 0.0) {
    return 0;
  }

  return (max - min) / (globalMax - globalMin);
}

/*! Calcula el valor de la métrica GenILoss para los valores categóricos.
  \param entry valor del atributo sobre el que calcular el valor de la
               métrica.
  \param tree árbol jerárquico del atributo.
  \return valor de la métrica.
*/
long double calculateCatGenILoss(const string entry, Tree tree) {

  const int directChildren = tree.getDirectChildren(entry).size();
  const int childrenInLevel = tree.getChildrenInLevel(entry).size();
  const int numerator =
      directChildren >= 1 ? directChildren - 1 : directChildren;
  const int denominator =
      childrenInLevel >= 1 ? childrenInLevel - 1 : childrenInLevel;

  return denominator == 0 ? 0 : numerator / (long double)denominator;
}

/*! Calcula y muestra por pantalla el valor de la métrica Generalized Information Loss
    o GenILoss.
  \param transposedDataset conjunto de datos transpuesto.
  \param trees árboles jerárquicos.
  \param allQids índice de todos los qids.
  \param catQids índice de los qids categóricos.
  \param numQids índice de los qids numéricos.
  \param tableSize cardinalidad de la tabla original.
*/
void calculateGenILoss(vector<vector<string>> transposedDataset,
                       map<int, Tree> trees, const vector<int> allQids,
                       const vector<int> catQids, const vector<int> numQids,
                       const int tableSize) {
  const long double initialLoss =
      1.0 / (transposedDataset[0].size() * (catQids.size() + numQids.size()));
  long double loss = 0;

  vector<map<string, int>> catFreqs, numFreqs;
  vector<tuple<long double, long double>> numGlobal;
  vector<string> numEntries;

  // Construct a frequency map of values for each qid
  for (const auto &idx : allQids) {
    map<string, int> freqs;
    for (const auto &entry : transposedDataset[idx]) {
      try {
        freqs[entry] += 1;
      } catch (...) {
        freqs[entry] = 1;
        if (find(numQids.begin(), numQids.end(), idx) != numQids.end())
          numEntries.emplace_back(entry);
      }
    }

    auto it = find(catQids.begin(), catQids.end(), idx);
    if (find(numQids.begin(), numQids.end(), idx) != numQids.end()) {
      numFreqs.emplace_back(freqs);
      tuple<long double, long double> tuple(
          make_tuple(calculateMaxNumValue(transposedDataset[idx]),
                     calculateMinNumValue(transposedDataset[idx])));
      numGlobal.emplace_back(tuple);
    } else if (it != catQids.end())
      catFreqs.emplace_back(freqs);
  }

  // Numerical Attributes
  for (size_t i = 0; i < numFreqs.size(); i++) {
    long double globalMax = get<0>(numGlobal[i]);
    long double globalMin = get<1>(numGlobal[i]);
    // Calculate Attributes GenILoss
    for (const auto &[k, v] : numFreqs[i])
      loss += calculateNumGenILoss(k, numEntries, globalMax, globalMin) * v;
  }

  // Categorical Attributes
  for (size_t i = 0; i < catFreqs.size(); i++) {
    // Calculate Attributes GenILoss
    for (const auto &[k, v] : catFreqs[i])
      loss += calculateCatGenILoss(k, trees[catQids[i]]) * v;
  }

  cout << "\t* GenILoss: ";
  cout << fixed << setprecision(3);
  cout << initialLoss * loss << endl;
}
