#pragma once

#include "binarysearchtree.h"

struct TNode : Node {
    TNode *left;
    TNode *right;
    NodeItem* node = nullptr;
    int size, height;
    long long key, prior;
    QColor col;
    TNode(long long k, long long _prior = rand(), TNode* _l = nullptr, TNode* _r = nullptr, QColor _col = QColor(0, 0, 0)) : left(_l), right(_r), key(k), prior(_prior), col(_col) {
        size = 1;
        height = 1;
    }
    void updateSize() {
        if (left != nullptr) left->updateSize();
        if (right != nullptr) right->updateSize();
        size = (left != nullptr ? left->size : 0) + (right != nullptr ? right->size : 0) + 1;
    }
    TNode *getL() {
        return this->left;
    }
    TNode *getR() {
        return this->right;
    }
    long long getKey() {
        return this->key;
    }
    QColor getColor() {
        return col;
    }
};


class treap : public BinarySearchTree {
private:
    TNode *root;
protected:
    int height(TNode *head){
        if(head==nullptr) return 0;
        return head->height;
    }
    TNode* merge (TNode *l, TNode *r) {
        if (!l) return r;
        if (!r) return l;
        if (l->prior > r->prior) {
            l->right = merge(l->right, r);
            return l;
        }
        else {
            r->left = merge(l, r->left);
            return r;
        }
    }
    std::pair<TNode*, TNode*> split (TNode *p, int x) {
        if (!p) return {0, 0};
        if (p->key <= x) {
            std::pair<TNode*, TNode*> q = split(p->right, x);
            p->right = q.first;
            return {p, q.second};
        }
        else {
            std::pair<TNode*, TNode*> q = split(p->left, x);
            p->left = q.second;
            return {q.first, p};
        }
    }

public:
    treap() {
        root = nullptr;
    }
    void insert(long long n) {
        TNode* node = new TNode(n);
        auto [l, r] = split(root, n);
        root = merge(l, merge(node, r));
    }
    void erase(long long data) {
        auto [l, r] = split(root, data - 1);
        auto [m1, m2] = split(r, data);
        delete m1;
        root = merge(l, m2);
    }
    TNode* find(long long x) {
        auto [l, r] = split(root, x - 1);
        auto [m1, m2] = split(r, x);
        TNode* node = m1;
        root = merge(l, merge(m1, m2));
        return node;
    }
    TNode* getRoot() {
        return root;
    }
    void updateHeight(TNode *head) {
        if (head == nullptr) return;
        updateHeight(head->left);
        updateHeight(head->right);
        head->height = 1 + std::max(height(head->left), height(head->right));
    }
};
