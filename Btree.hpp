#ifndef BTREE_H
#define BTREE_H

#include "BTreeNode.hpp"

#include <iostream>
#include <vector>
using namespace std;

// ---------------------------------------------------------------------------------------------

// Clase Árbol B
class BTree
{
public:
    BTreeNode *root; // Nodo raíz
    int t;           // Grado mínimo

    // Constructor
    BTree(int t);

    // Métodos
    void traverse();          // Recorrer el árbol
    BTreeNode *search(int k); // Buscar una clave
    void insert(int k);       // Insertar una clave

    void remove(int k); // Eliminar una clave del árbol
};

#endif // BTREE_H
