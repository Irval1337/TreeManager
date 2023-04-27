#include "binarysearchtree.h"
#include <stack>

Node::Node() {

}
Node::Node(long long k, Node* _l, Node* _r, QColor col) {
    key = k;
    left = _l;
    right = _r;
    color = col;
    size = 1;
    height = 1;
}
Node::~Node() {

}
Node *Node::getL() {
    return this->left;
}
Node *Node::getR() {
    return this->right;
}
long long Node::getKey() {
    return this->key;
}
QColor Node::getColor() {
    return this->color;
}
void Node::updateSize() {
    if (left != nullptr) left->updateSize();
    if (right != nullptr) right->updateSize();
    size = (left != nullptr ? left->size : 0) + (right != nullptr ? right->size : 0) + 1;
}

BinarySearchTree::BinarySearchTree() {
    root = nullptr;
    height = 0;
}
BinarySearchTree::BinarySearchTree(int h, Node* r) {
    root = r;
    height = h;
}
BinarySearchTree::~BinarySearchTree() {
    if (root == nullptr) return;
    std::stack<Node*> st;
    st.push(root);
    while (!st.empty()) {
        auto t = st.top();
        st.pop();
        if (t->getL() != nullptr) st.push(t->getL());
        if (t->getR() != nullptr) st.push(t->getR());
        delete t;
    }
}
Node* BinarySearchTree::getRoot() {
    return root;
}
void BinarySearchTree::insert(long long val) {
    Q_UNUSED(val);
}
void BinarySearchTree::erase(long long val) {
    Q_UNUSED(val);
}
Node* BinarySearchTree::find(long long val) {
    Q_UNUSED(val);
    return nullptr;
}
void BinarySearchTree::updateHeight(Node *head) {
    if (head == nullptr) return;
    updateHeight(head->left);
    updateHeight(head->right);
    head->height = 1 + std::max(head->left == nullptr ? 0 : head->left->height, head->right == nullptr ? 0 : head->right->height);
}

