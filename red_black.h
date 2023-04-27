#pragma once

#include "binarysearchtree.h"

struct RBNode : Node {
    RBNode *left;
    RBNode *right;
    NodeItem* node = nullptr;
    int size, height;
    long long key;
    int color;

    RBNode* parent;
    RBNode(long long k, int _col = 0, RBNode* _l = nullptr, RBNode* _r = nullptr, RBNode* _p = nullptr) : left(_l), right(_r), key(k), color(_col), parent(_p) {

    }
    void updateSize() {
        if (left != nullptr) left->updateSize();
        if (right != nullptr) right->updateSize();
        size = (left != nullptr ? left->size : 0) + (right != nullptr ? right->size : 0) + 1;
    }
    RBNode *getL() {
        return this->left;
    }
    RBNode *getR() {
        return this->right;
    }
    long long getKey() {
        return this->key;
    }
    QColor getColor() {
        return color == 0 ? QColor(255, 0, 0) : QColor(0, 0, 0);
    }
};


class RBTree : public BinarySearchTree {
private:
    RBNode *root;
protected:
    void rotateLeft(RBNode *&ptr) {
        RBNode *right_child = ptr->right;
        ptr->right = right_child->left;

        if (ptr->right != nullptr)
            ptr->right->parent = ptr;

        right_child->parent = ptr->parent;

        if (ptr->parent == nullptr)
            root = right_child;
        else if (ptr == ptr->parent->left)
            ptr->parent->left = right_child;
        else
            ptr->parent->right = right_child;

        right_child->left = ptr;
        ptr->parent = right_child;
    }
    void rotateRight(RBNode *&ptr) {
        RBNode *left_child = ptr->left;
        ptr->left = left_child->right;

        if (ptr->left != nullptr)
            ptr->left->parent = ptr;

        left_child->parent = ptr->parent;

        if (ptr->parent == nullptr)
            root = left_child;
        else if (ptr == ptr->parent->left)
            ptr->parent->left = left_child;
        else
            ptr->parent->right = left_child;

        left_child->right = ptr;
        ptr->parent = left_child;
    }
    void fixInsert(RBNode *&ptr) {
        RBNode *parent = nullptr;
        RBNode *grandparent = nullptr;
        while (ptr != root && getColor(ptr) == 0 && getColor(ptr->parent) == 0) {
            parent = ptr->parent;
            grandparent = parent->parent;
            if (parent == grandparent->left) {
                RBNode *uncle = grandparent->right;
                if (getColor(uncle) == 0) {
                    setColor(uncle, 1);
                    setColor(parent, 1);
                    setColor(grandparent, 0);
                    ptr = grandparent;
                } else {
                    if (ptr == parent->right) {
                        rotateLeft(parent);
                        ptr = parent;
                        parent = ptr->parent;
                    }
                    rotateRight(grandparent);
                    std::swap(parent->color, grandparent->color);
                    ptr = parent;
                }
            } else {
                RBNode *uncle = grandparent->left;
                if (getColor(uncle) == 0) {
                    setColor(uncle, 1);
                    setColor(parent, 1);
                    setColor(grandparent, 0);
                    ptr = grandparent;
                } else {
                    if (ptr == parent->left) {
                        rotateRight(parent);
                        ptr = parent;
                        parent = ptr->parent;
                    }
                    rotateLeft(grandparent);
                    std::swap(parent->color, grandparent->color);
                    ptr = parent;
                }
            }
        }
        setColor(root, 1);
    }
    void fixDelete(RBNode *&node) {
        if (node == nullptr)
            return;

        if (node == root) {
            root = nullptr;
            return;
        }

        if (getColor(node) == 0 || getColor(node->left) == 0 || getColor(node->right) == 0) {
            RBNode *child = node->left != nullptr ? node->left : node->right;

            if (node == node->parent->left) {
                node->parent->left = child;
                if (child != nullptr)
                    child->parent = node->parent;
                setColor(child, 1);
                delete (node);
            } else {
                node->parent->right = child;
                if (child != nullptr)
                    child->parent = node->parent;
                setColor(child, 1);
                delete (node);
            }
        } else {
            RBNode *sibling = nullptr;
            RBNode *parent = nullptr;
            RBNode *ptr = node;
            setColor(ptr, 2);
            while (ptr != root && getColor(ptr) == 2) {
                parent = ptr->parent;
                if (ptr == parent->left) {
                    sibling = parent->right;
                    if (getColor(sibling) == 0) {
                        setColor(sibling, 1);
                        setColor(parent, 0);
                        rotateLeft(parent);
                    } else {
                        if (getColor(sibling->left) == 1 && getColor(sibling->right) == 1) {
                            setColor(sibling, 0);
                            if(getColor(parent) == 0)
                                setColor(parent, 1);
                            else
                                setColor(parent, 2);
                            ptr = parent;
                        } else {
                            if (getColor(sibling->right) == 1) {
                                setColor(sibling->left, 1);
                                setColor(sibling, 0);
                                rotateRight(sibling);
                                sibling = parent->right;
                            }
                            setColor(sibling, parent->color);
                            setColor(parent, 1);
                            setColor(sibling->right, 1);
                            rotateLeft(parent);
                            break;
                        }
                    }
                } else {
                    sibling = parent->left;
                    if (getColor(sibling) == 0) {
                        setColor(sibling, 1);
                        setColor(parent, 0);
                        rotateRight(parent);
                    } else {
                        if (getColor(sibling->left) == 1 && getColor(sibling->right) == 1) {
                            setColor(sibling, 0);
                            if (getColor(parent) == 0)
                                setColor(parent, 1);
                            else
                                setColor(parent, 2);
                            ptr = parent;
                        } else {
                            if (getColor(sibling->left) == 1) {
                                setColor(sibling->right, 1);
                                setColor(sibling, 0);
                                rotateLeft(sibling);
                                sibling = parent->left;
                            }
                            setColor(sibling, parent->color);
                            setColor(parent, 1);
                            setColor(sibling->left, 1);
                            rotateRight(parent);
                            break;
                        }
                    }
                }
            }
            if (node == node->parent->left)
                node->parent->left = nullptr;
            else
                node->parent->right = nullptr;
            delete(node);
            setColor(root, 1);
        }
    }
    int getColor(RBNode *&t) {
        if (t == nullptr) return 1;
        return t->color;
    }
    void setColor(RBNode *&t, int col) {
        if (t == nullptr) return;
        t->color = col;
    }
    RBNode* _insert(RBNode *&root, RBNode *&ptr) {
        if (root == nullptr)
           return ptr;

        if (ptr->key < root->key) {
           root->left = _insert(root->left, ptr);
           root->left->parent = root;
        } else if (ptr->key > root->key) {
           root->right = _insert(root->right, ptr);
           root->right->parent = root;
        }
        return root;
    }
    RBNode* _erase(RBNode *&root, long long data) {
        if (root == nullptr)
            return root;

        if (data < root->key)
            return _erase(root->left, data);

        if (data > root->key)
            return _erase(root->right, data);

        if (root->left == nullptr || root->right == nullptr)
            return root;

        RBNode *temp = minValueNode(root->right);
        root->key = temp->key;
        auto _tmp = root->node;
        root->node = temp->node;
        temp->node = _tmp;
        return _erase(root->right, temp->key);
    }
    RBNode *minValueNode(RBNode *&node) {
        RBNode *ptr = node;
        while (ptr->left != nullptr)
            ptr = ptr->left;
        return ptr;
    }
    RBNode* _find(RBNode*& head, long long x) {
        if (head == nullptr) return nullptr;
        long long k = head->key;
        if (k > x) return _find(head->left, x);
        if (k < x) return _find(head->right, x);
        return head;
    }
    int height(RBNode *head){
        if(head==nullptr) return 0;
        return head->height;
    }
public:
    RBTree() {
        root = nullptr;
    }
    void insert(long long n) {
        RBNode *node = new RBNode(n);
        root = _insert(root, node);
        fixInsert(node);
    }
    void erase(long long data) {
        RBNode *node = _erase(root, data);
        fixDelete(node);
    }
    RBNode* find(long long x) {
        return _find(root, x);
    }
    RBNode* getRoot() {
        return root;
    }
    void updateHeight(RBNode *head) {
        if (head == nullptr) return;
        updateHeight(head->left);
        updateHeight(head->right);
        head->height = 1 + std::max(height(head->left), height(head->right));
    }
};
