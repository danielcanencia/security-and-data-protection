/*! \file kmeans.cpp
    \brief Fichero principal del algoritmo K-Means

    Anonimiza un conjunto de datos compuesto únicamente por atributos
    numéricos dado un archivo con extensión csv y un parámetro K representando
    el número de clusters a generar.

    Ejemplo: ./kmeans.out datasets/3-attributes/k-means-100.csv 2
*/

#include "kmeans.h"

//! Constructor de la clase KMeans
/*!
  \param K parámetro K del algoritmo (número de clusters a generar).
*/
Kmeans::Kmeans(int K) { this->K = K; }

//! Genera un vector compuesto por clusters (groups), inicializando sus
//! respectivos centroides.
/*!
  \param records conjunto de registros.
  \return vector compuesto por clusters (clase Group)
*/
vector<Group> Kmeans::inicializeCentroids(vector<Record> &records) {
  vector<Group> groups;
  vector<int> aux;

  srand(time(NULL));

  for (int i = 0; i < K; i++) {
    while (1) {
      int random = rand() % records.size();
      // Comprobar si se repite algún valor
      if (find(aux.begin(), aux.end(), random) == aux.end()) {
        // Inicialización de centroides y asignación de sus clusters
        aux.emplace_back(random);
        records.at(random).setGroup(i);
        Group group(i, records.at(random));
        groups.emplace_back(group);
        break;
      }
    }
  }
  return groups;
}

//! Calcula los centroides / clusters a los que asignar cada uno de los
//! registros, en función de la distancia euclideana entre registro y centroide.
/*!
  \param groups conjunto de clusters (contiene los centroides).
  \param records conjunto de registros.
  \return vector compuesto por índices de clusters.
*/
vector<double> Kmeans::centroidsDistances(const vector<Group> &groups,
                                          const vector<Record> &records) {
  vector<double> res;
  int gindex;
  double min, ed;
  vector<vector<double>> centroids;
  for (const Group &group : groups) {
    centroids.emplace_back(group.getCentroid());
  }

  // Calculo de la distancia euclidiana entre cada uno  de los centroides
  // y todos los registros 
  for (const Record &record : records) {
    min = record.euclideanDistance(centroids[0]);
    gindex = 0;

    for (int i = 1; i < (int)centroids.size(); i++) {
      ed = record.euclideanDistance(centroids[i]);
      if (ed < min) {
        min = ed;
        gindex = i;
      }
    }
    // Lista de centroides pertenecientes a cada registro
    res.emplace_back(gindex);
  }

  return res;
}

//! Actualiza o recalcula los centroides de cada cluster o grupo.
/*!
  \param groups conjunto de clusters.
  \param newGroups índices de los clusters a los que se asigna cada registro.
  \param curRecords registros iniciales.
  \return vector compuesto por índices de registros.
*/
bool Kmeans::updateGroups(vector<Group> &groups,
                          const vector<double> &newGroups,
                          vector<Record> &curRecords) {
  bool end = 1;
  int newGroup, curGroup;

  for (int i = 0; i < (int)newGroups.size(); i++) {
    newGroup = newGroups[i];
    curGroup = curRecords[i].getGindex();
    if (newGroup != curGroup) {
      if (curGroup != -1) {
        groups[curGroup].removeRecord(curRecords[i]);
      }
      curRecords[i].setGroup(newGroup);
      groups[newGroup].addRecord(curRecords[i]);
      end = 0;
    }
  }
  return end;
}

bool Kmeans::convergency(vector<vector<double>> centroids,
                 vector<Group> updatedGroups, int epsilon) {
  vector<double> c;

  if (centroids.size() != updatedGroups.size()) {
    return false;
  }

  for (int i = 0; i < (int)updatedGroups.size(); i++) {
    c = updatedGroups[i].getCentroid();

    // Distancia Euclidiana
    int sum = 0;
    for (int j = 0; j < (int)min(centroids[i].size(), c.size()); j++)
      sum += pow((double)centroids[i][j] - c[j], 2);
    
    if (sqrt(sum) > epsilon) 
      return false;
  }

  return true;
}

//! Bucle principal del algoritmo KMeans.
/*!
  \param records conjunto de registros.
  \return conjunto de clusters.
*/
vector<Group> Kmeans::computeAll(vector<Record> &records, int epsilon) {

  // 1. Inicializar los centroides
  vector<Group> groups = inicializeCentroids(records);

  // Bucle inicial
  vector<vector<double>> oldCentroids(1);
  while (!convergency(oldCentroids, groups, epsilon)) {
    // 2. Euclidean Distance And Group Classification
    vector<double> newGroups = centroidsDistances(groups, records);
    // 3. Actualizar el grupo al que pertenece cada registro
    bool end = updateGroups(groups, newGroups, records);
    if (end)
      break;

    // 4. Recalcular el centroide de cada grupo
    oldCentroids = {};
    for (Group &group : groups) {
      // Guardar la situación de los centroides en el grupo
      oldCentroids.emplace_back(group.getCentroid());
      group.recalculateCentroid();
    }
  }

  return groups;
}

//! Generaliza cada uno de los registros pertenecientes a cada cluster,
//! utilizando global recoding.
/*!
  \param groups conjunto de cluster sin anonimizar.
  \param qids conjunto de atributos cuasi-identificadores.
  \return conjunto de clusters anonimizado.
*/
vector<vector<vector<string>>> generalize(vector<Group> groups,
                                          vector<int> qids) {
  vector<vector<vector<string>>> clusters;

  // Global recoding
  for (auto &group : groups) {
    clusters.emplace_back(group.generalize(qids));
  }

  return clusters;
}

//! Realiza la lectura del conjunto de datos
/*!
  \param file fichero en el que esta contenido el conjunto de datos.
  \param headers nombres de todos los atributos
  \param qidNames nombre de los atributos cuasi-identificadores.
  \param qids índices de los atributos cuasi-identificadores.
  \return conjunto de registros.
*/
vector<Record> preprocessing(string file, vector<string> &headers,
                             vector<string> qidNames, vector<int> &qids) {
  // Leer fichero CSV
  ifstream input{file};
  if (!input.is_open()) {
    throw "Error reading file";
  }

  // Parse fichero CSV
  vector<Record> values;
  int index = 0;
  string line, headersAux;
  try {
    // Headers && Qids
    getline(input, headersAux);
    string tmp;
    stringstream ss(headersAux);
    while (getline(ss, tmp, ';')) {
      headers.push_back(tmp);
    }
    headers.back().pop_back();

    // ïndices de los qids
    for (size_t i = 0; i < qidNames.size(); i++) {
      auto it = find(headers.begin(), headers.end(), qidNames[i]);
      if (it != headers.end())
        qids.emplace_back(it - headers.begin());
      else if (headers.back() == qidNames[i])
        qids.emplace_back(headers.size() - 1);
    }

    // Registros
    for (; getline(input, line);) {
      Record record = Record(index);
      istringstream strm(move(line));

      for (string val; getline(strm, val, ';');) {
        record.pushBackValue(stod(val));
      }
      values.push_back(move(record));
      index++;
    }
  } catch (const exception &e) {
    input.close();
    return values;
  }

  input.close();
  return values;
}

int main(int argc, char **argv) {

  // Arguments
  if (argc != 3) {
    cout << "Invalid arguments. Use ./kmeans [filename] [k]" << endl;
    return -1;
  }
  // Fichero de entrada
  string filename{argv[1]};
  if (filename.substr(filename.size() - 4, filename.size()) != ".csv") {
    cout << "Please, use a csv input file" << endl;
    return -1;
  }

  // K
  int K = atoi(argv[2]);
  // Lectura de parámetros
  // Nombres de qids
  const int nqids = readNumberOfQids();
  vector<string> qidNames = readQidNames(nqids);
  const int epsilon = readEpsilon();

  // Preproceso de datos
  vector<Record> records;
  vector<int> qids;
  vector<string> headers;

  try {
    records = preprocessing(filename, headers, qidNames, qids);
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
    sort(qids.begin(), qids.end());
  } catch (exception &e) {
    cout << "Error reading file" << endl;
    return -1;
  }

  // Leer Pesos
  vector<double> weights = readWeights(nqids, qidNames);

  cout << "* K-Means algorithm is being run..." << endl;
  // Estimar tiempo de ejecución
  auto start = chrono::high_resolution_clock::now();
  // Especificar el número de clases a generar
  Kmeans kmeans(K);
  // Algoritmo principal
  vector<Group> groups = kmeans.computeAll(records, epsilon);
  // Generar clusters
  vector<vector<vector<string>>> clusters = generalize(groups, qids);

  // Obtener tiempo de ejecución
  auto stop = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
  cout << endl << "===> K-Means Execution Time: ";
  cout << duration.count() << " microseconds" << endl;
  cout << "===> Number of clusters: ";
  cout << groups.size() << endl;
  cout << "* K-Means algorithm finished. A csv file will be generated...."
       << endl;

  // Escribir los datos anonimizados en un fichero
  string directory = filename;
  directory = directory.substr(directory.find('/') + 1, directory.size());
  directory = directory.substr(0, directory.find_last_of("."));
  directory.insert(0, "outputs/");
  directory += "_K" + to_string(K) + "/clusters/";
  fs::create_directories(directory);
  cout << "* Writing output to directory: " + directory << endl;

  // Crear matriz de resultados
  vector<vector<string>> result;
  for (size_t i = 0; i < clusters.size(); i++) {
    result.insert(result.begin(), clusters[i].begin(), clusters[i].end());
    writeAnonymizedTable(fs::path(directory), headers, clusters[i], K, -1, -1,
                         "cluster" + to_string(i + 1), false);
  }

  // MÉTRICAS
  // GCP
  try {
    // 	1. Precalculamos NCP para cada clase de equivalencia
    vector<long double> cncps =
        calculateNCPS(clusters, weights, qids, qids, {});
    // 	2. Calculamos GCP
    calculateGCP(clusters, records.size(), qids, cncps);
  } catch (const char *e) {
    cout << e << endl;
    return -1;
  }

  // DM y CAvg se calculan en función del tamaño mínimo
  // de entre todas las clases de equivalencia
  cout << "\t===> Evaluating K value to be used on DM and CAvg metrics"
          " based on clusters sizes"
       << endl;
  int KValue =
      (*min_element(clusters.begin(), clusters.end(),
                    [](vector<vector<string>> a, vector<vector<string>> b) {
                      return a.size() < b.size();
                    }))
          .size();
  cout << "\t===> K Value: ";
  cout << KValue << endl;

  if (K != -1) {
    // DM
    calculateDM(clusters, records.size(), KValue);
    // CAvg
    calculateCAVG(clusters, records.size(), KValue);
  }

  // GenILoss
  try {
    calculateGenILoss(transpose(result), {}, qids, {}, qids, records.size());
  } catch (const char *e) {
    cout << e << endl;
    return -1;
  }

  return 0;
}
