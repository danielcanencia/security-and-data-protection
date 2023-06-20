/*! \file diversity.cpp
    \brief Fichero que contiene todas las funciones relacionadas con los
           modelos de privacidad k-anonimity y l-diversity.
*/

#include "diversity.h"

/*! Comprueba si una clase de equivalencia es l-diversa.
  \param cluster clase de equivalencia.
  \param confAtt índice del atributo sensible.
  \param L parámetro de la l-diversidad.
  \return 1 si la clase es l-diversa, 0 si no lo es.
*/
bool isDiverse(vector<vector<string>> cluster, const int confAtt, const int L) {
  map<string, int> freqs;

  // Frecuencias del atributo sensible
  for (const auto &entry : cluster) {
    try {
      freqs[entry[confAtt]] += 1;
    } catch (...) {
      freqs[entry[confAtt]] = 1;
    }
  }

  // Cada atributo confidencial debe presentar, al menor, L
  // valores bien representados
  if ((int)freqs.size() < L)
    return false;

  return true;
}

/*! Obtiene el valor del atributo que más frecuencia presenta.
  \param cluster clase de equivalencia.
  \param confAtt índice del atributo sensible.
  \return valor seleccionado.
*/
string majorityClass(vector<vector<string>> cluster, const int confAtt) {
  map<string, int> freqs;

  // Frecuencias del atributo sensible
  for (const auto &entry : cluster) {
    try {
      freqs[entry[confAtt]] += 1;
    } catch (...) {
      freqs[entry[confAtt]] = 1;
    }
  }

  // Obtener el valor con la frecuencia máxima
  return (*max_element(
              freqs.begin(), freqs.end(),
              [](const auto &a, const auto &b) { return a.second < b.second; }))
      .first;
}

/*! Comprueba si la clase mayoritaria se encuentra en una lista de valores.
  \param majorityClass clase mayoritaria (valor).
  \param valor lista de valores.
  \return 1 si la clase mayoritaria se encuentra en la lista, 0 si no es así.
*/
bool isSensitive(string majorityClass, vector<string> values) {
  return find(values.begin(), values.end(), majorityClass) != values.end();
}

/*! Leer el nombre del atributo sensible a utilizar por el algoritmo.
  \return valor del atributo sensible.
*/
string readConfidentialAttName() {
  string attName;
  string question =
      "Do yo want to use a confidential attribute? "
      "(will only be used if l-diversity is choosen) [Y(y)/N(n)]: ";
  cout << question;
  char answer;
  cin >> answer;
  bool keep = true;
  while (keep) {
    switch (answer) {
    case 'Y':
    case 'y':
      cout << "Confidential attribute: " << endl;
      cout << "\tEnter att name: ";
      cin >> attName;
      keep = false;
      break;
    case 'N':
    case 'n':
      keep = false;
      break;
    default:
      cout << question << endl;
      cin >> answer;
    }
  }

  return attName;
}

/*! Leer el valor del parámetro de la penalización de diversidad.
  \return penalización de diversidad.
*/
int readDiversityPenalty() {
  string question = "Diversity Penalty: ";
  cout << question;

  int penalty;
  while (1) {
    cin >> penalty;
    if (cin.fail()) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Error, enter a valid number." << endl;
      cout << question;
      continue;
    }
    break;
  }

  return penalty;
}

/*! Lee la métrica a utilizar por el algoritmo (EDM o SDM).
  \return índice de la métrica a utilizar.
*/
int readDiversity() {
  string question = "Diversity Metric:\n"
                    "\tEqual Diversity Metric (0), "
                    "Sensitive Diversity Metric (1): ";
  cout << question;

  int diversity;
  while (1) {
    cin >> diversity;
    if (cin.fail()) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Error, enter a valid number." << endl;
      cout << question;
      continue;
    }
    break;
  }

  return diversity;
}

/*! Leer los valores realmente sensibles a utilizar por la métrica SDM.
  \return lista de valores.
*/
vector<string> readSensitiveValues(vector<string> values) {
  vector<string> sensitiveValues;
  string aux;

  while (!values.empty()) {
    cout << "Enter number printed between brackets:\n\t";
    for (size_t i = 0; i < values.size(); i++)
      cout << values[i] + "(" + to_string(i) + ") ";
    cout << endl << "\t\t[enter q to quit] >> ";
    cin >> aux;

    if (aux == "q")
      break;

    int idx = stoi(aux);
    if (idx >= (int)values.size() || idx < 0) {
      cout << "Error, index out of range." << endl;
      continue;
    }
    sensitiveValues.emplace_back(values[idx]);
    values.erase(values.begin() + idx);
  }

  return sensitiveValues;
}
