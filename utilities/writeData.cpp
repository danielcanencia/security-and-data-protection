/*! \file writeData.cpp
    \brief Fichero que contiene todas las funciones relacionadas
           con la escritura de datos en ficheros.
*/

#include "writeData.h"

#define GET_NAME(Var) (#Var)

/*! Escribe en el fichero apuntado por el stream fp,
    el conjunto de datos dataset como cadenas de caracteres.
  \param fp stream que referencia el fichero en el realizar la escritura.
  \param dataset conjunto de datos.
  \param delimiter delimitador a utilizar para separar los datos.
*/
void writeStrings(ofstream &fp, vector<vector<string>> dataset,
                  string delimiter) {
  // Write all records
  for (size_t i = 0; i < dataset.size(); i++) {
    fp << accumulate(
              dataset[i].begin(), dataset[i].end(), string(),
              [](string &a, string &b) { return a.empty() ? b : a + ";" + b; })
       << endl;
  }
}

/*! Escribe el conjunto de datos anonimizado en un fichero localizado
    en un directorio concreto, delimitado por el valor de los parámetros
    relacionados con los modelos de privacidad utilizados para anonimizar
    los datos.
  \param inputFname nombre inicial del fichero en el realizar la escritura.
  \param headers cabecera del fichero. Define los nombres de los atributos presentes.
  \param dataset conjunto de datos anonimizados.
  \param K parámetro del modelo de privacidad k-anonymity.
  \param L parámetro del modelo de privacidad l-diversity.
  \param T parámetro del modelo de privacidad t-closeness.
  \param exFileName nombre de fichero absoluto, sobre el que no se realizaran
                modificaciones. (opcional).
  \param verbose indica si se debe mostrar información por pantalla.

*/
void writeAnonymizedTable(const string inputFname, const vector<string> headers,
                          const vector<vector<string>> dataset, const int K,
                          const int L, const long double T, const string exFileName,
                          const bool verbose) {
  string kName = K == -1 ? "" : to_string(K) + GET_NAME(K);
  if (L != -1 && K != -1)
    kName += "_";
  string lName = L == -1 ? "" : to_string(L) + GET_NAME(L);
  if (T != -1 && L != -1)
    lName += "_";
  stringstream stream;
  stream << fixed << setprecision(7) << T;
  const string pName = T == -1 ? "" : stream.str() + GET_NAME(T);

  string input = inputFname;
  if (input.back() == '/')
    input.resize(input.size() - 1);
  const string parentPath = filesystem::path{input}.string();

  string dname = "generalized_tables";
  if (parentPath.find_first_of('/') == parentPath.find_last_of('/'))
    dname += "/" + parentPath + "/";
  else
    dname +=
        parentPath.substr(parentPath.find_first_of('/'), parentPath.size()) +
        "/";

  if (!fs::is_directory(dname) || !fs::exists(dname)) {
    if (!fs::create_directories(dname)) {
      throw "Error creating output directory";
      return;
    }
  }

  string fname = dname;
  if (exFileName == "\0")
    fname += kName + lName + pName + ".csv";
  else
    fname += exFileName + ".csv";

  if (verbose) {
    cout << "===> Writing data to file: " << endl;
    cout << "\t* " + fname << endl;
  }
  ofstream fp(fname);

  vector<vector<string>> aux;
  aux.emplace_back(headers);

  try {
    writeStrings(fp, aux, ";");
    writeStrings(fp, dataset, ";");
  } catch (...) {
    cout << "Error writing anonymized data to file" << endl;
  }

  fp.close();
}
