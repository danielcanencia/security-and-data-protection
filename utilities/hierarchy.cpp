/*! \file hierarchy.cpp
    \brief Fichero que contiene todas las funciones de lectura de ficheros.
*/

#include "hierarchy.h"

/*! Devuelve el rango máximo (máxima generalización)
    perteneciente a un atributo numérico.
  \param dataset conjunto de datos.
  \param qidIndex índice del atributo qid numérico.
*/
string getNumericRoot(const vector<vector<string>> dataset,
                      const int qidIndex) {
  long double dmin, dmax;

  string value = dataset[0][qidIndex];
  int index = value.find('~');
  if (index != (int)string::npos) {
    dmin = stod(value.substr(0, index));
    dmax = stod(value.substr(index + 1, value.size()));
  } else
    dmin = dmax = stod(value);

  long double splitMin, splitMax;
  for (const auto &entry : dataset) {
    value = entry[qidIndex];
    index = value.find('~');
    if (index != (int)string::npos) {
      splitMin = stod(value.substr(0, index));
      splitMax = stod(value.substr(index + 1, value.size()));
    } else
      splitMin = splitMax = stod(value);

    if (splitMin < dmin)
      dmin = splitMin;
    if (splitMax > dmax)
      dmax = splitMax;
  }

  string max = to_string(dmax);
  string min = to_string(dmin);
  // Remove trailing decimal zeroes
  if (min.find('.') != string::npos) {
    min = min.substr(0, min.find_last_not_of('0') + 1);
    if (min.find('.') == min.size() - 1)
      min = min.substr(0, min.size() - 1);
  }
  if (max.find('.') != string::npos) {
    max = max.substr(0, max.find_last_not_of('0') + 1);
    if (max.find('.') == max.size() - 1)
      max = max.substr(0, max.size() - 1);
  }

  string root = min + "~" + max;
  return root;
}

/*! Calcula el índice de un atributo dado su nombre.
  \param qidName nombre del atributo.
  \param headers cabecera del fichero. Define los nombres de los atributos presentes.
  \return índice del atributo
*/
int getHierarchyIdx(const string qidName, vector<string> headers) {
  string tmp;

  // Convert qid name to lowercase
  transform(qidName.begin(), qidName.end(), inserter(tmp, tmp.end()),
            [](unsigned char x) { return tolower(x); });

  // Iterate through the headers and find the correct index
  std::vector<string>::iterator itr = find(headers.begin(), headers.end(), tmp);

  if (itr != headers.cend())
    return distance(headers.begin(), itr);

  return -1;
}

/*! Identifica si un atributo se encuentra entre la lista de atributos dada.
  \param qidName nombre del atributo.
  \param qidNames lista de nombres de atributos.
  \return 1 si se encuentra presente, 0 en otro caso.
*/
bool compareAttQid(string qidName, vector<string> qidNames) {

  string s1;

  // Get a lowercase version of the arguments
  transform(qidName.begin(), qidName.end(), inserter(s1, s1.end()),
            [](unsigned char x) { return tolower(x); });

  if (find(qidNames.begin(), qidNames.end(), s1) != qidNames.end())
    return true;
  return false;
}

/*! Transpone una matriz de dimensión 3, en nuestro caso, corresponde a
    las jerarquías de todos los atributos.
  \param hierarchies matriz que representa las jerárquias de todos los atributos qids.
  \return matriz de jerarquía transpuesta.
*/
vector<vector<vector<string>>>
transposeAndFormat(const vector<vector<vector<string>>> &hierarchies) {
  vector<vector<vector<string>>> res;

  for (const vector<vector<string>> &entry : hierarchies) {
    res.emplace_back(transpose(entry));
  }

  return res;
}

/*! Lee el directorio general en el que se encuentran los ficheros de jerarquia
    (en un directorio separado) y el conjunto de datos inicial.
  \param directory directorio relativo.
  \param dataset conjunto de datos.
  \param headers cabecera del fichero. Define los nombres de los atributos presentes.
  \param qids índices de atributos cuasi-identificadores o qids.
  \param atts índices de atributos sensibles o SAs.
  \param transpose indica si se desea devolver la matriz de jerarquías de forma
                   transpuesta.
  \return índice del atributo
*/
map<int, vector<vector<string>>>
readDirectory(fs::path const &directory, vector<vector<string>> &dataset,
              vector<string> &headers, vector<string> attQids,
              vector<string> confAtts, vector<int> &qids, vector<int> &atts,
              const bool transpose) {

  // Get a lowercase version of the attribute names
  vector<string> attNames;
  for (const string &tmp : attQids) {
    string auxTmp;

    transform(tmp.begin(), tmp.end(), inserter(auxTmp, auxTmp.end()),
              [](unsigned char x) { return tolower(x); });

    attNames.push_back(auxTmp);
  }

  // Get a lowercase version of the confidential
  // attribute names
  vector<string> confAttNames;
  for (const string &tmp : confAtts) {
    string auxTmp;

    transform(tmp.begin(), tmp.end(), inserter(auxTmp, auxTmp.end()),
              [](unsigned char x) { return tolower(x); });

    confAttNames.push_back(auxTmp);
  }

  // Locate csv input file and hierarchies directory
  string file;
  if (fs::exists(directory) && fs::is_directory(directory)) {
    auto const entry = fs::directory_iterator(directory).operator*();
    if (fs::is_regular_file(entry) && entry.path().extension() == ".csv")
      file = entry.path().filename();
    else
      throw "Error, file format error.";
  }
  const string filename = string(directory) + "/" + file;

  vector<string> hierarchies;
  const string hierarchies_dir = string(directory) + "/" + "hierarchies" + "/";
  try {
    for (auto const &entry :
         fs::recursive_directory_iterator(hierarchies_dir)) {
      if (fs::is_directory(entry))
        continue;
      hierarchies.emplace_back(hierarchies_dir +
                               entry.path().filename().c_str());
    }
  } catch (...) {
    throw "Error, directory doesn't exist.";
  }

  // Read csv dataset
  ifstream input1{filename};
  if (!input1.is_open()) {
    throw "Error reading file";
  }

  // Skip headers and insert them into a variable
  string headers_aux;
  getline(input1, headers_aux);

  // Get a lowercase version of the headers
  vector<string> headersVector;
  string tmp;
  stringstream ss(headers_aux);
  while (getline(ss, tmp, ';')) {
    headers.push_back(tmp);
    transform(tmp.begin(), tmp.end(), tmp.begin(),
              [](unsigned char x) { return tolower(x); });
    headersVector.push_back(tmp);
  }

  // Get confidential attribute indexes
  for (size_t i = 0; i < confAttNames.size(); i++) {
    auto it = find(headersVector.begin(), headersVector.end(), confAttNames[i]);
    if (it != headersVector.end())
      atts.emplace_back(it - headersVector.begin());
    else if (headersVector.back() == confAttNames[i])
      atts.emplace_back(confAttNames.size() - 1);
  }

  // Read input table
  string line;
  for (; getline(input1, line);) {
    // Dont read empty lines
    if (line.length() == 0)
      continue;

    istringstream strm(move(line));
    vector<string> row;
    for (string val; getline(strm, val, ';');) {
      row.emplace_back(val);
    }
    dataset.emplace_back(row);
  }
  input1.close();

  // Read hierarchy files
  string qidName;
  vector<string> qidNames;

  int idx;
  vector<vector<vector<string>>> res;
  vector<int> indexes;
  for (const string &filename : hierarchies) {
    ifstream input2{filename};
    if (!input2.is_open()) {
      throw "Error reading file";
    }

    // Read first line: qid's hierarchy name
    getline(input2, qidName);
    qidNames.emplace_back(qidName);

    // Check if it is a qid or a confidential attribute
    bool confAttFlag = false;
    if (compareAttQid(qidName, confAttNames))
      confAttFlag = true;

    if (!compareAttQid(qidName, attNames) &&
        !compareAttQid(qidName, confAttNames))
      continue;

    // Read hierarchy values
    vector<vector<string>> hierarchy;
    for (; getline(input2, line);) {
      // Dont read empty lines
      if (line.length() == 0)
        continue;
      istringstream strm(move(line));
      vector<string> row;
      for (string val; getline(strm, val, ';');) {
        row.emplace_back(val);
      }
      hierarchy.emplace_back(row);
    }
    input2.close();

    // Get hierarchy corresponding qid
    idx = getHierarchyIdx(qidName, headersVector);
    if (!confAttFlag)
      qids.emplace_back(idx);
    indexes.emplace_back(idx);
    res.emplace_back(hierarchy);
  }

  // Get transposed hierchies (e.g Incognito algorithm)
  vector<vector<vector<string>>> tRes;
  if (transpose)
    tRes = transposeAndFormat(res);

  // Map qid values and hierarchy sets
  map<int, vector<vector<string>>> hMap;
  for (size_t i = 0; i < indexes.size(); i++) {
    if (!transpose)
      hMap[indexes[i]] = res[i];
    else
      hMap[indexes[i]] = tRes[i];
  }

  return hMap;
}

/*! Transpone una matriz de dimensión 2.
  \param matrix matriz de dimensión 2.
  \return matriz transpuesta.
*/
vector<vector<string>> transpose(const vector<vector<string>> &matrix) {

  if (matrix.size() == 0)
    return matrix;

  const int rows = matrix.size();
  const int cols = matrix[0].size();
  vector<vector<string>> arr(cols, vector<string>(rows));

  // Transpose vector
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      arr[j][i] = matrix[i][j];
    }
  }

  return arr;
}

/*! Calcula las posibles permutaciones de un vector de datos.
  \param len tamaño de la permutación.
  \param data vector de datos inicial.
  \return matriz con todas las permutaciones posibles.
*/
vector<vector<int>> getPermutations(int len, const vector<int> data) {

  vector<vector<int>> permutations;
  int auxArray[data.size()];
  permute(data, permutations, data.size(), len, auxArray);
  return permutations;
}

/*! Calcula las posibles permutaciones de un vector de datos.
  \param data vector ede datos inicial.
  \param permutations matriz con todas las permutaciones posibles.
  \param n tamaño del vector de datos inicial.
  \param len tamano de la permutación.
  \param aux array auxiliar.
  \param rept maraca el número de llamadas recursivas a esta función.
  \param idx índice del vector sobre el que trabaja la función.
*/
void permute(const vector<int> data, vector<vector<int>> &permutations, int n,
             int len, int *aux, int rept, int idx) {

  if (idx == len) {
    vector<int> entry;
    for (int i = 0; i < len; i++) {
      entry.emplace_back(data[aux[i]]);
    }
    permutations.emplace_back(entry);
    return;
  }

  for (int i = rept; (i < n) && (n - i + 1); i++) {
    aux[idx] = i;
    permute(data, permutations, n, len, aux, i + 1, idx + 1);
  }
}

/*! Calcula el índice de cada qid en la cabecera del fichero de datos original.
  \param headers cabecera del fichero. Define los nombres de los atributos presentes.
  \param qids índices de atributos cuasi-identificadores o qids.
  \return lista de índices de los qids.
*/
vector<int> getQidsHeaders(const vector<string> headers,
                           const vector<string> qids) {
  vector<int> res;
  int idx;

  // Get a lowercase version of the headers
  vector<string> headersVector;
  for (string s : headers) {
    transform(s.begin(), s.end(), s.begin(),
              [](unsigned char x) { return tolower(x); });
    headersVector.push_back(s);
  }

  // Get a lowercase version of the qids
  vector<string> qidsVector;
  for (string qid : qids) {
    transform(qid.begin(), qid.end(), qid.begin(),
              [](unsigned char x) { return tolower(x); });
    qidsVector.push_back(qid);
  }

  // Check if qids are present in dataset input file
  for (const auto &qid : qidsVector) {
    if (find(headersVector.begin(), headersVector.end(), qid) ==
        headersVector.end()) {
      return res;
    }
  }

  for (const string &qid : qidsVector) {
    idx = getHierarchyIdx(qid, headersVector);
    res.emplace_back(idx);
  }

  return res;
}
