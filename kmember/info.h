/*! \file info.h
    \brief Fichero de cabecera del archivo info.cpp.
*/

#ifndef _INFO_H
#define _INFO_H

#include "../utilities/hierarchy.h"
#include "../utilities/tree.h"
#include <iostream>
#include <vector>

/*! Clase Info. Su función es contener todas las estructuras y funciones
    necesarias para manipular el árbol jerárquico de la forma que necesita
    k-member.
*/
class Info {
private:
  vector<vector<string>> records;
  vector<vector<string>> attsValues;
  map<int, vector<vector<string>>> hierarchies;
  map<int, Tree> trees;
  vector<int> numQids, catQids;
  map<int, int> treeHeights;
  map<int, long double> maxDomSizes;

  void calculateHeights();
  void calculateMaxDomSizes();

public:
  Info(vector<vector<string>> records,
       map<int, vector<vector<string>>> hierarchies, map<int, Tree> trees,
       vector<int> numQids, vector<int> catQids);
  vector<int> getNumQids();
  vector<int> getCatQids();
  int valueType(int qid);
  vector<vector<string>> getRecords();
  map<int, vector<vector<string>>> getHierarchies();
  int getTreeHeight(int index);
  long double getMaxDomSize(int index);
  int getSubTreeHeight(string v1, string v2, int index);
  int lowestCommonAncestor(int index);
};

#endif
