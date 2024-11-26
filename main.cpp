#include "BTree.hpp"

int main()
{
    BTree tree(3);

    tree.insert(10);
    tree.insert(20);
    tree.insert(5);
    tree.insert(6);
    tree.insert(12);
    tree.insert(30);
    tree.insert(7);
    tree.insert(17);

    cout << "Recorrido del árbol:" << endl;
    tree.traverse();

    cout << "\n\nEliminar 6:" << endl;
    tree.remove(6);
    tree.traverse();

    cout << "\n\nEliminar 13 (no existe):" << endl;
    tree.remove(13);
    tree.traverse();

    cout << "\n\nEliminar 7:" << endl;
    tree.remove(7);
    tree.traverse();

    return 0;
}
