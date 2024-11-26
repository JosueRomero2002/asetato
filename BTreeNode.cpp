#include "BTreeNode.hpp"

// -------------------------------------------------------------------------------------------------------------
// Implementación de BTreeNode
BTreeNode::BTreeNode(int t, bool isLeaf)
{
    this->t = t;
    this->isLeaf = isLeaf;
}

void BTreeNode::traverse()
{
    int i = 0;
    for (; i < keys.size(); i++)
    {
        if (!isLeaf)
            children[i]->traverse();
        cout << keys[i] << " ";
    }
    if (!isLeaf)
        children[i]->traverse();
}

BTreeNode *BTreeNode::search(int k)
{
    int i = 0;
    while (i < keys.size() && k > keys[i])
        i++;
    if (i < keys.size() && keys[i] == k)
        return this;
    if (isLeaf)
        return nullptr;
    return children[i]->search(k);
}

void BTreeNode::insertNonFull(int k)
{
    int i = keys.size() - 1;
    if (isLeaf)
    {
        keys.push_back(0);
        while (i >= 0 && keys[i] > k)
        {
            keys[i + 1] = keys[i];
            i--;
        }
        keys[i + 1] = k;
    }
    else
    {
        while (i >= 0 && keys[i] > k)
            i--;
        i++;
        if (children[i]->keys.size() == 2 * t - 1)
        {
            splitChild(i, children[i]);
            if (keys[i] < k)
                i++;
        }
        children[i]->insertNonFull(k);
    }
}

void BTreeNode::splitChild(int i, BTreeNode *y)
{
    BTreeNode *z = new BTreeNode(y->t, y->isLeaf);
    z->keys.assign(y->keys.begin() + t, y->keys.end());
    y->keys.resize(t - 1);
    if (!y->isLeaf)
    {
        z->children.assign(y->children.begin() + t, y->children.end());
        y->children.resize(t);
    }
    children.insert(children.begin() + i + 1, z);
    keys.insert(keys.begin() + i, y->keys[t - 1]);
}

void BTreeNode::remove(int k)
{
    int idx = 0;
    while (idx < keys.size() && keys[idx] < k)
        idx++;

    if (idx < keys.size() && keys[idx] == k)
    {
        if (isLeaf)
        {
            keys.erase(keys.begin() + idx);
        }
        else
        {
            if (children[idx]->keys.size() >= t)
            {
                int pred = getPredecessor(idx);
                keys[idx] = pred;
                children[idx]->remove(pred);
            }
            else if (children[idx + 1]->keys.size() >= t)
            {
                int succ = getSuccessor(idx);
                keys[idx] = succ;
                children[idx + 1]->remove(succ);
            }
            else
            {
                merge(idx);
                children[idx]->remove(k);
            }
        }
    }
    else
    {
        if (isLeaf)
        {
            cout << "La clave " << k << " no está en el árbol.\n";
            return;
        }
        bool flag = (idx == keys.size());
        if (children[idx]->keys.size() < t)
            fill(idx);
        if (flag && idx > keys.size())
            children[idx - 1]->remove(k);
        else
            children[idx]->remove(k);
    }
}

// ----------------------------getPredecessor-------------------------------
// Encuentra el predecesor inmediato de una clave en un nodo interno.
// Esto se logra tomando el subárbol izquierdo de la clave en idx y avanzando hacia la derecha hasta alcanzar la hoja más grande.
// Propósito: Reemplazar la clave eliminada con su predecesor.

int BTreeNode::getPredecessor(int idx)
{
    BTreeNode *curr = children[idx];
    while (!curr->isLeaf)
        curr = curr->children[curr->keys.size()];
    return curr->keys.back();
}
// ----------------------------------------------------------------------------

// ----------------------------getSuccessor-------------------------------
// Encuentra el sucesor inmediato de una clave en un nodo interno.
// Esto se logra tomando el subárbol derecho de la clave en idx y avanzando hacia la izquierda hasta alcanzar la hoja más pequeña.
// Propósito: Reemplazar la clave eliminada con su sucesor
int BTreeNode::getSuccessor(int idx)
{
    BTreeNode *curr = children[idx + 1];
    while (!curr->isLeaf)
        curr = curr->children[0];
    return curr->keys.front();
}
// ----------------------------------------------------------------------------

// ----------------------------fill-------------------------------
// Garantiza que el hijo en el índice idx tenga al menos 𝑡 claves antes de descender en él.
// Cómo lo hace:

// Si el hijo anterior tiene al menos 𝑡 claves, pide prestada una clave de él.
// Si el siguiente hijo tiene al menos 𝑡 claves, pide prestada una clave de él.
// De lo contrario, fusiona el hijo con uno de sus hermanos.

// Propósito: Mantener las propiedades del Árbol B durante la eliminación
void BTreeNode::fill(int idx)
{
    if (idx != 0 && children[idx - 1]->keys.size() >= t)
        borrowFromPrev(idx);
    else if (idx != keys.size() && children[idx + 1]->keys.size() >= t)
        borrowFromNext(idx);
    else
    {
        if (idx != keys.size())
            merge(idx);
        else
            merge(idx - 1);
    }
}
// ----------------------------------------------------------------------------

// ----------------------------borrowFromPrev-------------------------------
// El hijo en el índice idx toma prestada una clave de su hermano izquierdo.
// La clave más grande del hermano izquierdo sube al nodo actual y una clave del nodo actual baja al hijo.
// Si hay subárboles, el último hijo del hermano izquierdo también se transfiere al hijo.
// Propósito: Redistribuir claves para evitar la fusión.
void BTreeNode::borrowFromPrev(int idx)
{
    BTreeNode *child = children[idx];
    BTreeNode *sibling = children[idx - 1];
    child->keys.insert(child->keys.begin(), keys[idx - 1]);
    if (!child->isLeaf)
        child->children.insert(child->children.begin(), sibling->children.back());
    keys[idx - 1] = sibling->keys.back();
    sibling->keys.pop_back();
    if (!sibling->isLeaf)
        sibling->children.pop_back();
}
// ----------------------------------------------------------------------------
void BTreeNode::borrowFromNext(int idx)
{
    BTreeNode *child = children[idx];
    BTreeNode *sibling = children[idx + 1];
    child->keys.push_back(keys[idx]);
    if (!child->isLeaf)
        child->children.push_back(sibling->children.front());
    keys[idx] = sibling->keys.front();
    sibling->keys.erase(sibling->keys.begin());
    if (!sibling->isLeaf)
        sibling->children.erase(sibling->children.begin());
}

// ----------------------------merge-------------------------------
// Combina el hijo en el índice idx con su hermano derecho (hijo en el índice idx + 1).
// La clave en el nodo actual que separa a ambos hijos baja para convertirse en la clave central del nodo fusionado.
// Todos los subárboles e hijos del hermano derecho se transfieren al hijo en el índice idx.
// Propósito: Reducir el tamaño del árbol cuando ambos hijos tienen menos de t claves.
void BTreeNode::merge(int idx)
{
    BTreeNode *child = children[idx];
    BTreeNode *sibling = children[idx + 1];
    child->keys.push_back(keys[idx]);
    child->keys.insert(child->keys.end(), sibling->keys.begin(), sibling->keys.end());
    if (!child->isLeaf)
        child->children.insert(child->children.end(), sibling->children.begin(), sibling->children.end());
    keys.erase(keys.begin() + idx);
    children.erase(children.begin() + idx + 1);
    delete sibling;
}

// -------------------------------------------------------------------------------------------------------------
