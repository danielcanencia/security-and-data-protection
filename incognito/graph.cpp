/*! \file graph.cpp
    \brief Fichero que proporciona la clase graph, que simula un grafo.
*/

#include "graph.h"

// Constructor de la clase Graph.
Graph::Graph(){};

/*! Constructor de la clase Graph.
  \param nodesMax valor máximo de cada qid.
  \param qids lista de cuasi-identificadores.
  \param toPrune vector de poda.
  \param graphData clase que contiene los datos del grafo.
*/
Graph::Graph(vector<int> nodesMax, vector<int> qids,
             vector<vector<int>> toPrune, GraphData &graphData) {
  this->graphData = graphData;
  this->qids = qids;
  generateNodesAndEdges(nodesMax, toPrune);
}

/*! Función principal sobre la que se generan los contenidos del
    grafo.
  \param nodesMax valores de generalización máximos de cada qid.
  \param toPrune vector de poda.
*/
void Graph::generateNodesAndEdges(const vector<int> &nodesMax,
                                  vector<vector<int>> toPrune) {
  int nsize = nodesMax.size();
  vector<int> inode(nsize, 0);
  this->graphData.addData(inode);

  int index = 0;
  generateAllNodes(inode, nodesMax, nsize, index);
  // Fase de poda
  this->graphData.pruneNodes(toPrune);
  this->graphData.generateAllEdges();
  this->graphData.setRoot();
  return;
}

/*! Genera todos los nodos del grafo.
  \param nodes lista de nodos.
  \param nodesMax valores de generalización máximos de cada qid.
  \param nsize número de nodos.
  \param index índice auxiliar.
*/
void Graph::generateAllNodes(const vector<int> &nodes,
                             const vector<int> &nodesMax, int nsize,
                             int index) {
  vector<vector<int>> aux;

  for (int i = 0; i < nsize; i++) {
    vector<int> nodeAux = nodes;
    if (nodes[i] < nodesMax[i]) {
      nodeAux[i] += 1;

      // Crear nodo y arista
      this->graphData.addData(nodeAux);

      index += 1;
      aux.emplace_back(nodeAux);
    }
  }

  for (const vector<int> &entry : aux) {
    generateAllNodes(entry, nodesMax, nsize, index);
  }
}

/*! Devuelve el nodo raíz del grafo.
  \return nodo raíz.
*/
GraphNode Graph::getRoot() { return this->graphData.getRoot(); }

/*! Devuelve todos los nodos hojas del grafo.
  \return lista de nodos.
*/
set<GraphNode> Graph::getLeaves() { return this->graphData.getLeaves(); }

/*! Devuelve la lista de índices de qids.
  \return lista de qids.
*/
vector<int> Graph::getQids() const { return this->qids; }

/*! Comprueba si un nodo se encuentra marcado.
  \return 1 si se encuentra marcado, 0 si no es así.
*/
bool Graph::isNodeMarked(GraphNode node) {
  return this->graphData.isNodeMarked(node);
}

/*! Añade las generalizaciones de un nodo.
  \param node nodo inicial.
  \param queue nodos generalizaciones del anterior.
  \return -1 si el nodo dado es la raíz, 0 si la función se ejecuta
          correctamente.
*/
int Graph::addGeneralizations(const GraphNode &node, set<GraphNode> &queue) {
  return this->graphData.addGeneralizations(node, queue);
}

void Graph::markGeneralizations(const GraphNode &node) {
  this->graphData.markGeneralizations(node);
}

GraphNode Graph::getFinalKAnon(map<int, map<string, vector<string>>> gens,
                               vector<vector<string>> dataset,
                               vector<int> qids) {
  return graphData.getFinalKAnon(gens, dataset, qids);
}
