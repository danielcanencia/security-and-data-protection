/*! \file graphData.cpp
    \brief Fichero que proporciona las funciones que comprueban si una
           partición cumple con los modelos de privacidad.
*/

#include "graphData.h"

// Constructor vacio de la clase GraphData.
GraphData::GraphData() {}

/*! Añade un nodo a la clase.
  \param data nodo a añadir.
*/
void GraphData::addData(vector<int> data) {
  int nodeId = this->contains(data);
  if (nodeId == -1) {
    this->nodes[this->idCount] = GraphNode(this->idCount, data);
    this->idCount++;
  }
}

/*! Comprueba si el nodo dado se encuentra en el grafo.
  \param node datos del nodo.
*/
int GraphData::contains(const vector<int> &node) {
  for (const auto &[id, entry] : this->nodes) {
    if (entry.isEqual(node))
      return entry.getId();
  }
  return -1;
}

/*! Elimina los nodos dados del grafoS.
  \param toPrune nodos a eliminars.
*/
void GraphData::pruneNodes(vector<vector<int>> toPrune) {
  while (!toPrune.empty()) {
    vector<int> node1 = toPrune[0];
    toPrune.erase(toPrune.begin());

    // Buscar nodo a podar
    bool flag;
    for (const auto &[id, node2] : this->nodes) {
      vector<int> data = node2.getData();
      flag = true;
      for (size_t w = 0; w < node1.size(); w++) {
        if (node1[w] != data[w]) {
          flag = false;
          break;
        }
      }
      // Podar nodo (eliminarlo)
      if (flag) {
        this->nodes.erase(node2.getId());
        break;
      }
    }
  }
}

/*! Genera todos las aristas del grafo. */
void GraphData::generateAllEdges() {
  for (const auto &[from, node1] : this->nodes) {
    GraphNode node = this->nodes[from];
    vector<int> to;

    for (const auto &[id, node2] : this->nodes) {
      if (from == id)
        continue;
      if (node.isChild(node2))
        to.emplace_back(id);
    }
    for (const auto &entry : to)
      this->edges.emplace_back(Edge(from, entry));
  }
}

/*! Establece el nodo raíz del grafo. */
void GraphData::setRoot() {
  bool flag;

  for (const auto &entry1 : this->edges) {
    int childId = entry1.getChild();
    flag = true;
    for (const auto &entry2 : this->edges) {
      if (entry2.getParent() == childId) {
        flag = false;
        break;
      }
    }

    if (flag) {
      this->root = this->nodes[childId];
      return;
    }
  }

  this->root = GraphNode();
}

/*! Devuelve el nodo raíz del grafo.
  \return nodo raíz.
*/
GraphNode GraphData::getRoot() { return this->root; }

/*! Devuelve los nodos hojas del grafo.
  \return lista de nodos hojas.
*/
set<GraphNode> GraphData::getLeaves() {
  set<GraphNode> roots;
  map<int, int> rootsIds;
  bool flag;

  // Obtener el índice del nodo raíz
  for (const auto &entry1 : this->edges) {
    int parentId = entry1.getParent();
    flag = true;
    for (const auto &entry2 : this->edges) {
      if (entry2.getChild() == parentId) {
        flag = false;
        break;
      }
    }
    if (flag) {
      roots.insert(this->nodes[entry1.getParent()]);
    }
  }

  return roots;
}

/*! Comprueba si el nodo dado se encuentra marcado.
  \param nodo datos del nodo a comprobar.
  \return 1 si se encuentra marcado, 0 si no es así.
*/
bool GraphData::isNodeMarked(GraphNode node) {
  return this->nodes[node.getId()].marked();
}

/*! Añade las generalizaciones (nodos) que no se encuentran marcados, 
    de un nodo.
  \param node nodo inicial.
  \param queue nodos generalizaciones del anterior.
  \return -1 si el nodo dado es la raíz, 0 si la función se ejecuta
          correctamente.
*/
int GraphData::addGeneralizations(const GraphNode &node,
                                  set<GraphNode> &queue) {
  vector<GraphNode> children = getChildren(node);
  if (children.empty())
    return -1;

  for (const GraphNode &child : children) {
    if (!child.marked())
      queue.insert(child);
  }

  return 0;
}

/*! Marca las generalizaciones del nodo dado.
  \param node nodo.
*/
void GraphData::markGeneralizations(const GraphNode &node) {
  this->nodes[node.getId()].setKAnon();
  this->nodes[node.getId()].mark();

  for (const GraphNode &child : getChildren(node)) {
    this->nodes[child.getId()].mark();
    this->nodes[child.getId()].setKAnon();
  }
}

/*! Devuelve los nodos hijos (más generales) de un nodo dado.
  \param node nodo.
  \return lista de nodos hijos.
*/
vector<GraphNode> GraphData::getChildren(GraphNode node) {
  vector<GraphNode> children;

  for (const Edge &edge : this->edges) {
    if (edge.getParent() == node.getId()) {
      children.emplace_back(this->nodes[edge.getChild()]);
    }
  }
  return children;
}

/*! Calcula todos los nodos que cumplen con los modelos de privacidad
    seleccionados.
  \param node nodo inicial.
  \param kList lista de nodos auxiliar que contiene todos los nodos
               añadidos en la cadena de llamadas recursivas.
  \param nodes lista de nodos kanon resultante.
*/
void GraphData::getAllKAnon(const GraphNode &node, vector<int> kList,
                            set<GraphNode> &nodes) {
  for (const GraphNode &n : getChildren(node)) {
    if (find(kList.begin(), kList.end(), n.getId()) != kList.end()) {
      nodes.insert(n);
      kList.emplace_back(n.getId());
      getAllKAnon(n, kList, nodes);
    }
  }
}

/*! Calcula el nodo final sobre el que se generalizará el conjunto
    de datos, de entre todos los nodos que cumplen los modelos de privacidad,
    seleccionando uno en función de un criterio específico.
  \param gens mapa de generalizaciones.
  \param dataset conjunto de datos.
  \param qids lista de atributos cuasi-identificadores.
  \return nodo resultante.
*/
GraphNode GraphData::getFinalKAnon(map<int, map<string, vector<string>>> gens,
                                   vector<vector<string>> dataset,
                                   vector<int> qids) {
  set<GraphNode> res;
  for (const auto &[id, node] : this->nodes) {
    if (node.isKAnon()) {
      vector<int> kList;
      res.insert(node);
      getAllKAnon(node, kList, res);
    }
  }

  // Si ningún nodo cumple el criterio devolvemos el nodo raíz
  if (res.size() == 0)
    return getRoot();

  // Obtener un nodo en función de un criterio
  // En este caso el que maximice el número de clases
  int max = -1;
  GraphNode finalNode;
  for (const GraphNode &node : res) {
    vector<int> freqs = node.evaluateFrequency(gens, dataset, qids);

    if ((int)freqs.size() > max || max == -1) {
      max = freqs.size();
      finalNode = node;
    }
  }

  return finalNode;
}
