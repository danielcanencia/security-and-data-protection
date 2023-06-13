/*! \file input.cpp
    \brief Fichero que contiene las funciones relacionadas con la
           lectura de parámetros.
*/

#include "input.h"

/*! Lee el número de qids que se desea utilizar en la anonimización de los
    datos.
  \return número de qids.
*/
const int readNumberOfQids() {
  string question = "Number of qids: ";
  cout << question;
  int nqids;

  while (1) {
    cin >> nqids;
    if (cin.fail()) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Error, enter a valid number." << endl;
      cout << question;
      continue;
    }
    break;
  }

  return nqids;
}

/*! Lee los nombres de los qids que se desean utilizar en la anonimización de los
    datos.
  \param nqids número de qids.
  \return nombre de los qids.
*/
vector<string> readQidNames(const int nqids) {
  set<string> qid_set;
  for (int i = 0; i < nqids; i++) {
    cout << "Enter qid name (" << i << "): ";
    string qidName;
    cin >> qidName;
    qid_set.insert(qidName);
  }
  if ((int)qid_set.size() != nqids) {
    throw "Input Error: Qids should be unique. "
          "Check if you repeated some of them.";
  }

  return vector<string>(qid_set.begin(), qid_set.end());
}

/*! Lee los nombres de los atributos sensibles o SAs que se desean utilizar
    en la anonimización de los datos.
  \return nombre de los SAs.
*/
vector<string> readConfidentialAttNames() {
  cout << "Confidential attributes: " << endl;
  // Read number of confidential attributes
  int nAtts;
  while (1) {
    cout << "\tNumber of confidential attributes: ";
    cin >> nAtts;
    if (cin.fail()) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "\tError, enter a valid number." << endl;
      continue;
    }
    break;
  }

  // Read confidential attributes names
  set<string> att_set;
  for (int i = 0; i < nAtts; i++) {
    cout << "\tEnter att name (" << i << "): ";
    string attName;
    cin >> attName;
    att_set.insert(attName);
  }
  if ((int)att_set.size() != nAtts) {
    const string error = "Input Error: Confidential Attributes should be "
                         "unique. Check if you repeated some of them.";
    cout << error << endl;
    return vector<string>(1, error);
  }

  return vector<string>(att_set.begin(), att_set.end());
}

/*! Lee el parámetro loss o suppThreshold utilizado por Datafly.
  \return valor del parámetro.
*/
long double readSuppThreshold() {
  string question = "Suppresion Threshold (percentage, recommended 0.3-2%): ";
  cout << question;
  long double threshold;

  while (1) {
    cin >> threshold;
    if (cin.fail()) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Error, enter a valid number." << endl;
      cout << question;
      continue;
    }

    if (threshold < 0.0 || threshold > 100.0) {
      cout << "Error, value should be between 0 and 100." << endl;
      cout << question;
      continue;
    }
    break;
  }
  
  return threshold;
}

/*! Lee el parámetro epsilon utilizado por K-Means.
  \return valor del parámetro.
*/
int readEpsilon() {
  string question = "Epsilon (convergency threshold): ";
  cout << question;
  int epsilon;

  while (1) {
    cin >> epsilon;
    if (cin.fail()) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Error, enter a valid number." << endl;
      cout << question;
      continue;
    }

    break;
  }
  
  return epsilon;
}

/*! Lee los pesos que corresponden a cada uno de los qids.
  \param nqids número de qids.
  \param qidNames nombres de los qids.
  \return pesos de los qids.
*/
vector<double> readWeights(const int nqids, vector<string> qidNames) {
  vector<double> weights;
  cout << "Do you want to use custom weights? "
          "(will only be used on gcp metric) [Y(y)/N(n)]: ";
  char answer;
  cin >> answer;
  bool keep = false;
  while (!keep) {
    switch (answer) {
    case 'Y':
    case 'y':
      for (int i = 0; i < nqids;) {
        cout << "Weight for qid " << i << ": ";
        float weight;
        cin >> weight;
        if (cin.fail()) {
          cin.clear();
          cin.ignore();
          cout << "Error, enter a valid number." << endl;
        } else {
          weights.emplace_back(weight);
          i++;
        }
      }
      if (accumulate(weights.begin(), weights.end(), (float)0) != (float)1) {
        cout << "Input Error: Weights must sum 1." << endl;
        weights.clear();
        continue;
      }
      keep = true;
      break;
    case 'N':
    case 'n':
      weights = vector<double>(qidNames.size(), 1.0 / qidNames.size());
      keep = true;
      break;
    default:
      cout << "Do you want to use custom weights? "
              "(will only be used on gcp metric) [Y(y)/N(n)]: ";
      cin >> answer;
    }
  }

  return weights;
}

/*! Determina si se quiere considerar algún atributo categórico como numérico.
  \param numQids índice de los qids numéricos.
  \param catQids índice de los qids categóricos.
  \param headers cabecera del fichero. Define los nombres de los atributos presentes.
  \return consideración del tipo de cada atributo que utilizar en el cálculo de las métricas.
*/
tuple<vector<int>, vector<int>> readMetricsQids(vector<int> numQids,
                                                vector<int> catQids,
                                                vector<string> headers) {
  vector<int> numMetricsQids = numQids, catMetricsQids;
  string question = "Do you want to treat some hierarchical attributes as "
                    "numerical? (will only be used on metrics) [Y(y)/N(n)]: ";
  cout << question;
  char answer;
  cin >> answer;
  bool keep;
  keep = true;
  string aux;
  int number;
  while (keep) {
    switch (answer) {
    case 'Y':
    case 'y':
      if (catQids.size() > 0)
        cout << "Enter number printed between brackets: " << endl;
      else
        cout << "\tThere are no categorical attributes. Continuing" << endl;

      while (catQids.size() > 0) {
        cout << "\t";
        for (size_t i = 0; i < catQids.size(); i++)
          cout << headers[catQids[i]] + "(" + to_string(i) + ") ";
        cout << endl;
        cout << "\t\t[enter q to quit] >> ";
        cin >> aux;

        if (aux == "q")
          break;

        try {
          number = stoi(aux);
          if (number < 0 || number >= (int)catQids.size()) {
            cin.clear();
            cout << "\tError: Number not present. Try again." << endl;
          } else {
            numMetricsQids.emplace_back(catQids[number]);
            catQids.erase(catQids.begin() + number);
          }
        } catch (...) {
          cin.clear();
          cout << "\tError, enter a number or q." << endl;
        }
      }
      keep = false;
      break;
    case 'N':
    case 'n':
      keep = false;
      break;
    default:
      cout << question;
      cin >> answer;
    }
  }
  for (const auto &qid : catQids) {
    if (find(numMetricsQids.begin(), numMetricsQids.end(), qid) ==
        numMetricsQids.end())
      catMetricsQids.emplace_back(qid);
  }

  tuple<vector<int>, vector<int>> tuple(numMetricsQids, catMetricsQids);
  return tuple;
}

/*! Lee un parámetro de un modelo de privacidad específico (K, L o T).
  \param privacyDef nombre del modelo de privacidad.
  \param parameter valor del parámetro relacionado con el modelo de privacidad.
  \param datasetSize cardinalidad del conjunto de datos (número de registros).
  \return valor del parámetro.
*/
long double readParameter(const string privacyDef, const string parameter,
                          const int datasetSize) {
  // Default value. Means privacy definition won't be checked
  // (k-anonimity, l-diversity, t-closeness, etc)
  long double param = -1;

  string question =
      "Do yo want to anonymize preserving " + privacyDef + "? [Y(y)/N(n)]: ";
  cout << question;
  char answer;
  cin >> answer;
  bool keep = true;
  while (keep) {
    switch (answer) {
    case 'Y':
    case 'y':
      while (1) {
        cout << "Insert " + parameter + ": ";
        cin >> param;
        if (cin.fail()) {
          cin.clear();
          cin.ignore();
          cout << "Error reading number, try again." << endl;
          continue;
        }
        if (param > datasetSize) {
          cin.clear();
          cin.ignore();
          cout << parameter +
                      " value should be equal or less tham dataset size.";
          cout << endl;
          continue;
        }
        break;
      }
      keep = false;
      break;
    case 'N':
    case 'n':
      keep = false;
      break;
    default:
      cout << "Error: enter a valid value" << endl;
      cout << question;
      cin >> answer;
    }
  }

  return param;
}

/*! Lee los valores de los parámetros de todos los modelos de privacidad a utilizar.
  \param datasetSize cardinalidad del conjunto de datos (número de registros).
  \param confAtts índice de los atributos sensibles o SAs.
  \param K parámetro del modelo de privacidad k-anonymity.
  \param L parámetro del modelo de privacidad l-diversity.
  \param T parámetro del modelo de privacidad t-closeness.
  \return 0 si se ha producido algún error, 1 en otro caso.
*/
bool readParameters(const int datasetSize, const int confAtts, int &K, int &L,
                    long double &T) {
  // K (K-anonimity)
  K = readParameter("k-anonymity", "K", datasetSize);

  // L (l-diversity)
  L = readParameter("l-diversity", "L", datasetSize);

  // T (t-closeness)
  T = readParameter("t-closeness", "T", datasetSize);

  // Check l-diversity and t-closeness errors
  if ((L != -1 || T != -1.0) && confAtts == 0) {
    cout << endl;
    cout << "An error occured.\nIf l-diversity or t-closeness "
            "are used, there should exists, at least, one "
            "confidential attribute."
         << endl
         << endl;
    return 0;
  }

  if (K == -1 && L == -1 && T == -1.0) {
    cout << "Error, some privacy technique should be used." << endl;
    readParameters(datasetSize, confAtts, K, L, T);
  }

  return 1;
}
