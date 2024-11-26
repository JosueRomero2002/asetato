#include "BTree.hpp"

// -------------------------------------------------------------------------------------------------------------

// Implementación de BTree
BTree::BTree(int t)
{
    root = nullptr;
    this->t = t;
}

void BTree::traverse()
{
    if (root != nullptr)
        root->traverse();
}

BTreeNode *BTree::search(int k)
{
    return (root == nullptr) ? nullptr : root->search(k);
}

void BTree::insert(int k)
{
    if (root == nullptr)
    {
        root = new BTreeNode(t, true);
        root->keys.push_back(k);
    }
    else
    {
        if (root->keys.size() == 2 * t - 1)
        {
            BTreeNode *newRoot = new BTreeNode(t, false);
            newRoot->children.push_back(root);
            newRoot->splitChild(0, root);
            int i = (newRoot->keys[0] < k) ? 1 : 0;
            newRoot->children[i]->insertNonFull(k);
            root = newRoot;
        }
        else
        {
            root->insertNonFull(k);
        }
    }
}

void BTree::remove(int k)
{
    if (!root)
    {
        cout << "El árbol está vacío.\n";
        return;
    }
    root->remove(k);
    if (root->keys.empty())
    {
        BTreeNode *temp = root;
        if (root->isLeaf)
            root = nullptr;
        else
            root = root->children[0];
        delete temp;
    }
}
