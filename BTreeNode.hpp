#ifndef BTREENODE_HPP
#define BTREENODE_HPP

#include <iostream>
#include <vector>
using namespace std;

// ---------------------------------------------------------------------------------------------
// Clase Nodo del Árbol B
class BTreeNode
{
public:
    vector<int> keys;             // Claves en el nodo
    vector<BTreeNode *> children; // Punteros a los hijos
    bool isLeaf;                  // Indica si es hoja
    int t;                        // Grado mínimo

    // Constructor
    BTreeNode(int t, bool isLeaf);

    // Métodos
    void traverse();                      // Recorrer el nodo
    BTreeNode *search(int k);             // Buscar una clave
    void insertNonFull(int k);            // Insertar en un nodo no lleno
    void splitChild(int i, BTreeNode *y); // Dividir un hijo lleno

    void remove(int k);           // Eliminar una clave
    int getPredecessor(int idx);  // Obtener el predecesor inmediato
    int getSuccessor(int idx);    // Obtener el sucesor inmediato
    void fill(int idx);           // Llenar un nodo con menos de t claves
    void borrowFromPrev(int idx); // Pedir una clave al nodo hermano anterior
    void borrowFromNext(int idx); // Pedir una clave al nodo hermano siguiente
    void merge(int idx);          // Fusionar dos nodos hijos
};

#endif // BTREENODE_HPP