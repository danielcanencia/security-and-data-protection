/*! \file distances.cpp
    \brief Fichero que contiene todas las funciones relacionados con el
           cálculo de distancias entre atributos.
*/

#include "distances.h"
#include <iostream>

/*! Selecciona un registro aleatorio.
  \param records lista de registros.
  \return índice del registro aleatorio seleccionado.
*/
int randomRecord(vector<vector<string>> records) {
  srand(time(NULL));
  int random = rand() % records.size();
  return random;
}

/*! Selecciona el registro más alejado al dado, utilizando las
    funciones de distancia pertinentes.
  \param record registro.
  \param index índice del registro record.
  \param records lista de registros.
  \param hierarchies mapa de jerarquias.
  \param numQids lista de índices de atributos numéricos.
  \param catQids lista de índices de atributos categóricos.
  \return índice del registro seleccionado.
*/
int furthestRecord(vector<string> record, int index,
                   vector<vector<string>> records,
                   map<int, vector<vector<string>>> hierarchies,
                   map<int, Tree> trees, vector<int> numQids,
                   vector<int> catQids) {
  long double furthestDiff = -1;
  int furthestIdx = -1;

  // Precalcular valores necesarios a través de la clase Info
  Info info(records, hierarchies, trees, numQids, catQids);

  // Comparar el registro principal con todos los demás
  for (size_t idx = 0; idx < records.size(); idx++) {
    if ((int)idx == index)
      continue;

    // Computar la distancia entre dos registros
    const long double aux = distance(record, records[idx], info);
    if (aux > furthestDiff || !furthestDiff) {
      furthestDiff = aux;
      furthestIdx = idx;
    }
  }

  // Si no se obtiene ningún resultado, se devuelve el índice
  // del registro inicial
  if (furthestIdx == -1)
    return index;
  return furthestIdx;
}

/*! Cálcula la distancia entre dos atributos numéricos.
  \param v1 primer valor numérico.
  \param v2 segundo valor numérico.
  \param maxDomSize tamaño del rango de valores.
  \return valor de la distancia calculada.
*/
long double numDistance(long double v1, long double v2,
                        long double maxDomSize) {

  return abs(v1 - v2) / maxDomSize;
}

/*! Cálcula la distancia entre dos atributos categóricos.
  \param v1 primer valor categórico.
  \param v2 segundo valor categórico.
  \param info clase Info con las funciones necesarias para
              manipular el árbol jerárquico.
  \param attIndex índice del atributo categórico.
  \return valor de la distancia calculada.
*/
long double catDistance(string &v1, string &v2, Info info, int attIndex) {

  return info.getSubTreeHeight(v1, v2, attIndex) / info.getTreeHeight(attIndex);

  return 1;
}

/*! Cálcula la distancia entre dos registros.
  \param r1 primer registro.
  \param r2 segundo registro.
  \param info clase Info con las funciones necesarias para
              manipular el árbol jerárquico.
  \return suma de las distancias calculadas.
*/
long double distance(vector<string> &r1, vector<string> &r2, Info info) {
  long double numSum = 0.0, catSum = 0.0;
  string v1, v2;
  // Atributos numéricos
  for (const auto &idx : info.getNumQids()) {
    v1 = r1[idx];
    v2 = r2[idx];

    numSum += numDistance(stold(v1), stold(v2), info.getMaxDomSize(idx));
  }

  // Atributos categóricos
  for (const auto &idx : info.getCatQids()) {
    v1 = r1[idx];
    v2 = r2[idx];

    catSum += catDistance(v1, v2, info, idx);
  }

  return numSum + catSum;
}

/*! Cálcula la pérdida de información en función de la métrica IL.
  \param records lista de registros.
  \param hierarchies mapa de jerarquias.
  \param trees árboles jerárquicos.
  \param numQids lista de índices de atributos numéricos.
  \param catQids lista de índices de atributos categóricos.
  \return valor calculado de la métrica IL.
*/
long double informationLoss(vector<vector<string>> records,
                            map<int, vector<vector<string>>> hierarchies,
                            map<int, Tree> trees, vector<int> numQids,
                            vector<int> catQids) {
  int e = records.size();
  vector<long double> numValues, catValues;

  // Atributos numéricos
  for (const int &idx : numQids) {
    long double max, min, aux;
    max = min = -1;
    vector<long double> numericDomain;

    for (const vector<string> &record : records) {
      aux = stold(record[idx]);
      numericDomain.emplace_back(aux);

      if (aux > max)
        max = aux;
      if (aux < min || min == -1)
        min = aux;
    }

    unique(numericDomain.begin(), numericDomain.end());

    numValues.emplace_back((max - min) / numericDomain.size());
  }

  // Atributos categóricos
  Info info(records, hierarchies, trees, vector<int>{}, catQids);

  for (const int &idx : catQids) {
    catValues.emplace_back(info.lowestCommonAncestor(idx) /
                           info.getTreeHeight(idx));
  }

  // Sumar todos los valores de cada vector.
  auto r1 = reduce(numValues.begin(), numValues.end());
  auto r2 = reduce(catValues.begin(), catValues.end());

  return e * (r1 + r2);
}

/*! Cálcula el registro que menor pérdida de información produza al
    añadirse a una clase de equivalencia.
  \param records lista de registros.
  \param cluster clase de equivalencia.
  \param hierarchies mapa de jerarquias.
  \param trees árboles jerárquicos.
  \param numQids lista de índices de atributos numéricos.
  \param catQids lista de índices de atributos categóricos.
  \param confAtt índice del atributo sensible.
  \param L parámetro de l-diversity.
  \param sensitiveValues valores realmente sensibles.
  \param diversityPenalty penalización de diversidad.
  \param diversity tipo de métrica a utilizar para calcular l-diversity.
  \return índice del registro obtenido.
*/
int findBestRecord(vector<vector<string>> records,
                   vector<vector<string>> cluster,
                   map<int, vector<vector<string>>> hierarchies,
                   map<int, Tree> trees, vector<int> numQids,
                   vector<int> catQids, int confAtt, const int L,
                   vector<string> sensitiveValues, const int diversityPenalty,
                   const int diversity) {
  long double min = -1, diff;
  int best;

  for (size_t i = 0; i < records.size(); i++) {
    vector<vector<string>> aux;
    aux = cluster;
    aux.emplace_back(records[i]);

    long double auxDiff =
        informationLoss(aux, hierarchies, trees, numQids, catQids) -
        informationLoss(cluster, hierarchies, trees, numQids, catQids);
    // K-Anonimidad
    diff = auxDiff;
    if (L != -1) {
      string majorClass = majorityClass(cluster, confAtt);
      string recordClass = records[i][confAtt];
      // Equal Diversity Metric
      if (diversity == 0) {
        if (isDiverse(cluster, confAtt, L))
          diff = auxDiff;
        else if (majorClass != recordClass)
          diff = auxDiff + diversityPenalty;
      }
      // Sensitive Diversity Metric
      else if (diversity) {
        if (isDiverse(cluster, confAtt, L))
          diff = auxDiff;
        else if (isSensitive(majorClass, sensitiveValues) &&
                 majorClass != recordClass) {
          diff = auxDiff + diversityPenalty;
        }
      }
    }
    if (diff < min || min == -1) {
      min = diff;
      best = i;
    }
  }

  return best;
}

/*! Cálcula la clase que menor pérdida de información produzca
    al añadirle un registro.
  \param clusters clases de equivalencia.
  \param records lista de registros.
  \param hierarchies mapa de jerarquias.
  \param trees árboles jerárquicos.
  \param numQids lista de índices de atributos numéricos.
  \param catQids lista de índices de atributos categóricos.
  \return índice de la clase elegida.
*/
int findBestCluster(map<int, vector<vector<string>>> clusters,
                    vector<string> record,
                    map<int, vector<vector<string>>> hierarchies,
                    map<int, Tree> trees, vector<int> numQids,
                    vector<int> catQids) {
  long double min = -1, diff;
  int best;

  for (size_t i = 0; i < clusters.size(); i++) {
    vector<vector<string>> aux;
    aux = clusters[i];
    aux.emplace_back(record);

    diff = informationLoss(aux, hierarchies, trees, numQids, catQids) -
           informationLoss(clusters[i], hierarchies, trees, numQids, catQids);
    if (diff < min || min == -1) {
      min = diff;
      best = i;
    }
  }

  return best;
}
