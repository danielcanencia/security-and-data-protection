/*! \file tree.cpp
    \brief Fichero que contiene la clase Tree, responsable de la creación
    y manejo de un árbol de jerarquía.
*/

#include "tree.h"
#include <iostream>

//! Constructor vacio
Tree::Tree(){};

/*! Constructor.
  \param hierarchy jeraquía a mostrar en formato de árbol.
*/
Tree::Tree(vector<vector<string>> hierarchy) {

  for (const auto &entries : hierarchy) {
    addLeave(entries[0], entries[1], entries.size() - 1);

    size_t i;
    string child = entries[0];
    for (i = 1; i < entries.size() - 1; i++) {
      child = addNode(entries[i], child, entries[i + 1].c_str());
    }

    addNode(entries[i], child);
  }

  // Calculate tree height
  this->height = calculateHeight();
}

/*! Añade un nodo hoja al árbol.
  \param value valor del nodo hoja.
  \param parent valor del nodo padre.
  \param depth profundidad en la que se encuentra la hoja.
*/
void Tree::addLeave(string value, string parent, int depth) {
  // Check if leave is already present
  if (this->nodes.count(value))
    return;

  Node node;
  node.value = value;
  node.parent = parent;
  node.isLeaf = true;
  node.depth = depth;
  this->nodes[value] = node;

  this->leaves.emplace_back(value);
}

/*! Añade un nodo al árbol.
  \param value valor del nodo.
  \param child valor del nodo hijo.
  \param parent valor del nodo padre.
*/
string Tree::addNode(string value, string child, const char *parent) {

  // Node already present
  if (this->nodes.count(value)) {
    this->nodes[value].children.insert(child);
    this->nodes[value].numSubTreeLeaves += 1;
    return value;
  }

  // Node not present
  Node node;
  node.value = value;
  if (parent)
    node.parent = (string)parent;
  else {
    node.parent = value;
    this->root = value;
  }

  node.depth = this->nodes[child].depth - 1;
  node.children = {child};
  node.numSubTreeLeaves += 1;
  this->nodes[value] = node;

  return value;
}

/*! Calcula la altura del árbol.
  \return altura del árbol.
*/
int Tree::calculateHeight() {
  set<int> depths;

  for (const string &value : leaves)
    depths.insert(this->nodes[value].depth);

  return *max_element(depths.begin(), depths.end()) + 1;
}

/*! Devuelve la altura del árbol.
  \return altura del árbol.
*/
int Tree::getHeight() { return this->height; }

/*! Devuelve la altura del árbol dado un nodo y la profundidad máxima
    conocida del mismo.
  \param value valor del nodo.
  \param depth profundidad máxima conocida.
  \return altura del árbol.
*/
int Tree::getHeight(string value, int &depth) {
  const Node node = this->nodes[value];
  int aux;

  for (const auto &child : node.children) {
    aux = getDepth(child);
    if (aux > depth)
      depth = aux;

    getHeight(child, depth);
  }

  return depth;
}

/*! Devuelve la altura del subárbol del que el nodo dado es raiz.
  \param value valor del nodo.
  \return altura del subárbol.
*/
int Tree::getHeight(string value) {
  const Node node = this->nodes[value];
  int maxDepth = node.depth;

  for (const auto &child : node.children)
    getHeight(child, maxDepth);

  return maxDepth - node.depth + 1;
}

/*! Devuelve el número de hojas descientes del nodo dado.
  \param value valor del nodo.
  \return número de hojas.
*/
int Tree::getNumSubTreeLeaves(string value) {
  return this->nodes[value].numSubTreeLeaves;
}

/*! Devuelve la profundidad del subárbol del que el nodo dado es raiz.
  \param value valor del nodo.
  \return profundidad del subárbol.
*/
int Tree::getDepth(string value) { return this->nodes[value].depth; }

/*! Determina si un nodo es hijo de otro.
  \param node valor del nodo padre.
  \param target valor del posible nodo hijo.
  \return 1 si target es hijo de node, y 0 si no es así.
*/
bool Tree::isChild(string node, string target) {
  set<string> children = this->nodes[node].children;

  if (find(children.begin(), children.end(), target) != children.end())
    return true;

  for (const auto &child : children) {
    if (isChild(child, target))
      return true;
  }

  return false;
}

/*! Devuelve los hijos directos del nodo dado.
  \param value valor del nodo.
  \return nodos hijos.
*/
vector<string> Tree::getDirectChildren(string value) {
  set<string> children = this->nodes[value].children;
  return vector<string>(children.begin(), children.end());
}

/*! Devuelve todos los hijos del nodo dado.
  \param value valor del nodo.
  \return nodos hijos.
*/
vector<string> Tree::getAllChildren(string value) {
  vector<string> children;

  for (const auto &child : this->nodes[value].children) {
    children.emplace_back(child);
    vector<string> next = getAllChildren(child);
    children.insert(children.begin(), next.begin(), next.end());
  }

  return children;
}

/*! Devuelve los nodos hijos del nodo dado que se encuentren
    en el siguiente nivel de profundidad.
  \param value valor del nodo.
  \return hijos del nodo.
*/
vector<string> Tree::getChildrenInLevel(string value) {
  vector<string> children;
  const int level = this->nodes[value].depth + 1;

  for (auto const &[k, v] : this->nodes) {
    if (v.depth == level)
      children.emplace_back(k);
  }

  return children;
}

/*! Devuelve la siguiente generalización del valor dado.
  \param value valor del nodo.
  \return valor generalizado.
*/
string Tree::getNextGen(string value) {

  // Node present
  if (this->nodes.count(value)) {
    string parent = this->nodes[value].parent;
    if (parent == value)
      return value;

    return parent;
  }

  cout << value << endl;
  // Not found
  throw "Error: Element not found in the tree";
}

/*! Devuelve el número de hojas de las que se compone el árbol.
  \return número de hojas.
*/
int Tree::getNumLeaves() { return this->leaves.size(); }

/*! Devuelve el ancestro común de dos nodos.
  \param node nodo de menor profundidad.
  \param target nodo de mayor profundidad.
  \return nodo ancestro.
*/
Node Tree::getCommonAncestor(Node node, string target) {
  if (node.value == target)
    return this->nodes[node.value];

  if (find(node.children.begin(), node.children.end(), target) !=
      node.children.end())
    return node;

  Node n = this->nodes[node.parent];
  vector<string> children = getAllChildren(node.value);

  if (find(children.begin(), children.end(), target) == children.end())
    return getCommonAncestor(n, target);

  return n;
}

/*! Devuelve el ancestro común, posicionado a menor profundidad, de dos nodos.
  \param r1 valor del primer nodo.
  \param r2 valor del segundo nodo.
  \return nodo ancestro.
*/
Node Tree::getLowestCommonAncestor(string r1, string r2) {

  Node n1 = this->nodes[r1];
  Node n2 = this->nodes[r2];
  Node node;
  string target;

  if (r1 == r2)
    return this->nodes[r1];

  if (this->nodes[n1.parent].numSubTreeLeaves <
      this->nodes[n2.parent].numSubTreeLeaves) {
    node = this->nodes[n1.parent];
    target = n2.value;
  } else {
    node = this->nodes[n2.parent];
    target = n1.value;
  }

  return getCommonAncestor(node, target);
}

/*! Devuelve el ancestro común, posicionado a menor profundidad,
    de varios nodos.
  \param values valores de todos los nodos.
  \return nodo ancestro.
*/
Node Tree::getLowestCommonAncestor(vector<string> values) {
  Node node;
  string aux = values[0];

  if (values.size() == 1) {
    return this->nodes[aux];
  }

  for (size_t i = 1; i < values.size(); i++) {
    node = getLowestCommonAncestor(aux, values[i]);
    // Return root node if we've reached it
    if (node.parent == node.value)
      return node;
    aux = node.value;
  }

  return node;
}

/*! Calcula el valor de la métrica NCP dado un conjunto de nodos.
  \param values valores de los nodos.
  \return valor de la métrica NCP.
*/
long double Tree::getNCP(vector<string> values) {
  // Get unique values only
  vector<string> aux = values;
  vector<string>::iterator it = unique(aux.begin(), aux.end());
  aux.resize(distance(aux.begin(), it));

  int subTreeLeaves = getLowestCommonAncestor(aux).numSubTreeLeaves;
  long double ncp = (long double)subTreeLeaves;
  return ncp;
}
