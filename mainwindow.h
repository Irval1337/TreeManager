#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "avl.h"
#include "red_black.h"
#include "splay.h"
#include "treap.h"
#include <map>
#include <vector>
#include <random>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void drawTree(BinarySearchTree* t);
    NodeItem* drawNode(Node* t, int x, int y);
    NodeItem* drawNode(RBNode* t, int x, int y);
    NodeItem* drawNode(TNode* t, int x, int y);
    void eraseFunc(long long x);
    void updateInfo();

private slots:
    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_insertButton_clicked();

    void on_insertNButton_clicked();

    void on_eraseNButton_clicked();

    void on_searchButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    QGraphicsScene* scene;
    const int sz = 80;
    BinarySearchTree* t = nullptr;
    std::vector<long long> els;
    Node* selected = nullptr;
    TreeType tree_type = TreeType::None;
    int n = 0;
    QString last_el = "?";
};
#endif // MAINWINDOW_H
