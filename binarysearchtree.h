#pragma once

#include <QColor>
#include "nodeitem.h"

class Node {
public:
    Node *left;
    Node *right;
    NodeItem* node = nullptr;
    int size, height;
    long long key;

    Node();
    Node(long long, Node* _l = nullptr, Node* _r = nullptr, QColor col = QColor(0, 0, 0));
    virtual ~Node();
    virtual Node *getL();
    virtual Node *getR();
    virtual long long getKey();
    virtual QColor getColor();
    virtual void updateSize();
private:
    QColor color;
};


class BinarySearchTree
{
public:
    BinarySearchTree();
    BinarySearchTree(int h, Node* root);
    virtual ~BinarySearchTree();
    virtual Node* getRoot();
    virtual void insert(long long val);
    virtual void erase(long long val);
    virtual Node* find(long long val);
    virtual void updateHeight(Node* node);
private:
    int height;
    Node* root;
};

enum TreeType {
    None, AVL, RB, Treap, Splay
};
