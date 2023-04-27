#pragma once

#include "binarysearchtree.h"

class SplayTree : public BinarySearchTree {
private:
    Node* root;
    Node* RR_Rotate(Node* k2)
    {
        Node* k1 = k2->left;
        k2->left = k1->right;
        k1->right = k2;
        return k1;
    }
    Node* LL_Rotate(Node* k2)
    {
        Node* k1 = k2->right;
        k2->right = k1->left;
        k1->left = k2;
        return k1;
    }
    Node* splay(Node* root, long long key)
    {
        if (!root)
            return nullptr;
        Node header;
        header.left = header.right = nullptr;
        Node* LeftTreeMax = &header;
        Node* RightTreeMin = &header;
        while (1)
        {
            if (key < root->key)
            {
                if (!root->left)
                    break;
                if (key < root->left->key)
                {
                    root = RR_Rotate(root);
                    if (!root->left)
                        break;
                }
                RightTreeMin->left = root;
                RightTreeMin = RightTreeMin->left;
                root = root->left;
                RightTreeMin->left = nullptr;
            }
            else if (key > root->key)
            {
                if (!root->right)
                    break;
                if (key > root->right->key)
                {
                    root = LL_Rotate(root);
                    if (!root->right)
                        break;
                }
                LeftTreeMax->right = root;
                LeftTreeMax = LeftTreeMax->right;
                root = root->right;
                LeftTreeMax->right = nullptr;
            }
            else
                break;
        }
        /* assemble L Tree, Middle Tree and R tree */
        LeftTreeMax->right = root->left;
        RightTreeMin->left = root->right;
        root->left = header.right;
        root->right = header.left;
        return root;
    }
    std::pair<Node*, Node*> split(Node* root, long long key) {
        if (root == nullptr) return {nullptr, nullptr};
        root = splay(root, key);
        if (root->key == key) {
            return {root->left, root->right};
        }
        if (root->key < key) {
            auto r = root->right;
            root->right = nullptr;
            return {root, r};
        }
        auto l = root->left;
        root->left = nullptr;
        return {l, root};
    }
    Node* _insert(Node* root, long long key)
    {
        if (root == nullptr)
            return new Node(key);

        root = splay(root, key);
        auto [v, u] = split(root, key);
        root = new Node(key, v, u);
        return root;
    }
    Node* maxValueNode(Node *&node) {
        Node *ptr = node;
        while (ptr->right != nullptr)
            ptr = ptr->right;
        return ptr;
    }
    Node* merge(Node* l, Node* r) {
        if (l == nullptr) return r;
        if (r == nullptr) return l;

        Node* head = maxValueNode(l);
        l = splay(l, head->getKey());
        l->right = r;
        return l;
    }
    Node* _find(Node* root, long long x) {
        root = splay(root, x);
        return root;
    }
    Node* _erase(Node* root, long long x) {
        root = splay(root, x);
        return merge(root->left, root->right);
    }
public:
    SplayTree() {
        root = nullptr;
    }
    void insert(long long x) {
        root = _insert(root, x);
    }
    void erase(long long x) {
        root = _erase(root, x);
    }
    Node* find(long long x) {
        return root = _find(root, x);
    }
    Node* getRoot() {
        return root;
    }
    void updateHeight(Node* head) {
        if (head == nullptr) return;
        updateHeight(head->left);
        updateHeight(head->right);
        head->height = 1 + std::max(head->left == nullptr ? 0 : head->left->height, head->right == nullptr ? 0 : head->right->height);
    }
};
