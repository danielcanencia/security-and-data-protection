
#include "edge.h"

/*! Constructor de la clase Partition.
  \param from nodo del que parte la arita, nodo origen.
  \param to nodo destino de la arista.
*/
Edge::Edge(int from, int to) {
  this->from = from;
  this->to = to;
}

/*! Devuelve el nodo origen.
  \return índice del nodo origen.
*/
int Edge::getParent() const { return this->from; }

/*! Devuelve el nodo destino.
  \return índice del nodo destino.
*/
int Edge::getChild() const { return this->to; }

/*! Establece el nodo origen.
  \param to índice del nodo origen.
*/
void Edge::setChild(const int to) { this->to = to; }

/*! Comprueba si una arista es igual a otra.
  \return 1 si las aritas son iguales, 0 si no es así.
*/
bool Edge::isEqual(int from, int to) const {
  if (this->from == from && this->to == to)
    return true;
  return false;
}

/*! Imprime los contenidos de la arista. */
void Edge::print() const {
  std::cout << "from: " + std::to_string(from) +
                   ", "
                   "to: " +
                   std::to_string(to)
            << std::endl;
}
