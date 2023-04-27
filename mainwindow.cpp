#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWheelEvent>
#include <QFontDatabase>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QFontDatabase::addApplicationFont("Consolas.ttf");

    scene = new QGraphicsScene();
    scene->setStickyFocus(true);

    t = new avl();
    drawTree(t);
    tree_type = TreeType::AVL;
    n = 0;
    updateInfo();

    ui->treeView->setScene(scene);
    ui->treeView->setTransformationAnchor(QGraphicsView::NoAnchor);
    ui->treeView->setRenderHint(QPainter::Antialiasing);
    ui->treeView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->treeView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->treeView->setAlignment(Qt::AlignCenter | Qt::AlignTop);
    ui->treeView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->treeView->setFocusPolicy(Qt::FocusPolicy::WheelFocus);
}

void MainWindow::updateInfo() {
    QString typeString = "";
    switch (tree_type) {
    case TreeType::None:
        typeString = "?";
        break;
    case TreeType::AVL:
        typeString = "АВЛ";
        break;
    case TreeType::RB:
        typeString = "Красно-черное";
        break;
    case TreeType::Treap:
        typeString = "Декартово";
        break;
    case TreeType::Splay:
        typeString = "Splay";
        break;
    }
    ui->typeLabel->setText("Тип: " + typeString);
    ui->countLabel->setText("Количество вершин: " + QString::number(n));
    if (t->getRoot() == nullptr)
        ui->heightLabel->setText("Высота: 0");
    else {
        if (tree_type == TreeType::AVL) {
            ((avl*)t)->updateHeight(t->getRoot());
        } else if (tree_type == TreeType::RB) {
            ((RBTree*)t)->updateHeight(((RBTree*)t)->getRoot());
        } else if (tree_type == TreeType::Splay) {
            ((SplayTree*)t)->updateHeight(((SplayTree*)t)->getRoot());
        } else if (tree_type == TreeType::Treap) {
            ((treap*)t)->updateHeight(((treap*)t)->getRoot());
        }
        ui->heightLabel->setText("Высота: " + QString::number(t->getRoot()->height));
    }
    ui->minLabel->setText("Минимальный элемент: " + (n == 0 ? "?" : QString::number(els[0])));
    ui->maxLabel->setText("Максимальный элемент: " + (n == 0 ? "?" : QString::number(els.back())));
    ui->lastLabel->setText("Последний используемый: " + last_el);
}

void MainWindow::eraseFunc(long long x) {
    last_el = QString::number(x);
    if (tree_type == TreeType::AVL) {
        ((avl*)t)->erase(x);
    } else if (tree_type == TreeType::RB) {
        ((RBTree*)t)->erase(x);
    } else if (tree_type == TreeType::Splay) {
        ((SplayTree*)t)->erase(x);
    } else if (tree_type == TreeType::Treap) {
        ((treap*)t)->erase(x);
    }
    n--;
    els.erase(std::find(els.begin(), els.end(), x));
    drawTree(t);
}

void MainWindow::drawTree(BinarySearchTree* t) {
    qDeleteAll(scene->items());
    delete scene;
    scene = new QGraphicsScene();
    ui->treeView->setScene(scene);
    if (t == nullptr || t->getRoot() == nullptr) {
        updateInfo();
        return;
    }
    if (selected != nullptr) {
        selected = nullptr;
    }

    if (tree_type == TreeType::AVL) {
        ((avl*)t)->getRoot()->updateSize();
    } else if (tree_type == TreeType::RB) {
        ((RBTree*)t)->getRoot()->updateSize();
    } else if (tree_type == TreeType::Splay) {
        ((SplayTree*)t)->getRoot()->updateSize();
    } else if (tree_type == TreeType::Treap) {
        ((treap*)t)->getRoot()->updateSize();
    }
    updateInfo();
    if (tree_type == TreeType::AVL) {
        drawNode(((avl*)t)->getRoot(), 0, 0);
    } else if (tree_type == TreeType::RB) {
        drawNode(((RBTree*)t)->getRoot(), 0, 0);
    } else if (tree_type == TreeType::Splay) {
        drawNode(((SplayTree*)t)->getRoot(), 0, 0);
    } else if (tree_type == TreeType::Treap) {
        drawNode(((treap*)t)->getRoot(), 0, 0);
    }
}

NodeItem* MainWindow::drawNode(Node* t, int x, int y) {
    if (t == nullptr) return nullptr;
    NodeItem* node = new NodeItem(x, y, t->getKey(), sz, t->getColor(), this);
    scene->addItem(node);
    t->node = node;
    if (t->getL() != nullptr) {
        int lx = x - (t->getL()->getR() != nullptr ? t->getL()->getR()->size : 0) * sz - sz;
        scene->addItem(node->getEdge(drawNode(t->getL(), lx, y + sz * 2)));
    }
    if (t->getR() != nullptr) {
        int rx = x + (t->getR()->getL() != nullptr ? t->getR()->getL()->size : 0) * sz + sz;
        scene->addItem(node->getEdge(drawNode(t->getR(), rx, y + sz * 2)));
    }
    return node;
}
NodeItem* MainWindow::drawNode(RBNode* t, int x, int y) {
    if (t == nullptr) return nullptr;
    NodeItem* node = new NodeItem(x, y, t->getKey(), sz, t->getColor(), this);
    scene->addItem(node);
    t->node = node;
    if (t->getL() != nullptr) {
        int lx = x - (t->getL()->getR() != nullptr ? t->getL()->getR()->size : 0) * sz - sz;
        scene->addItem(node->getEdge(drawNode(t->getL(), lx, y + sz * 2)));
    }
    if (t->getR() != nullptr) {
        int rx = x + (t->getR()->getL() != nullptr ? t->getR()->getL()->size : 0) * 80 + 80;
        scene->addItem(node->getEdge(drawNode(t->getR(), rx, y + sz * 2)));
    }
    return node;
}
NodeItem* MainWindow::drawNode(TNode* t, int x, int y) {
    if (t == nullptr) return nullptr;
    NodeItem* node = new NodeItem(x, y, t->getKey(), sz, t->getColor(), this, t->prior);
    scene->addItem(node);
    t->node = node;
    if (t->getL() != nullptr) {
        int lx = x - (t->getL()->getR() != nullptr ? t->getL()->getR()->size : 0) * sz - sz;
        scene->addItem(node->getEdge(drawNode(t->getL(), lx, y + sz * 2)));
    }
    if (t->getR() != nullptr) {
        int rx = x + (t->getR()->getL() != nullptr ? t->getR()->getL()->size : 0) * 80 + 80;
        scene->addItem(node->getEdge(drawNode(t->getR(), rx, y + sz * 2)));
    }
    return node;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_4_clicked()
{
    ui->treeView->scale(1.1, 1.1);
}


void MainWindow::on_pushButton_5_clicked()
{
    ui->treeView->scale(1/1.1, 1/1.1);
}


void MainWindow::on_insertButton_clicked()
{
    long long val = ui->insertBox->text().toLongLong();
    t->insert(val);
    n++;
    els.push_back(val);
    sort(els.begin(), els.end());
    last_el = QString::number(val);
    drawTree(t);
}


void MainWindow::on_insertNButton_clicked()
{
    std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    long long cnt = ui->insertBox->text().toLongLong();
    for(int i = 0; i < cnt; ++i) {
        long long val = rand();
        t->insert(val);
        n++;
        els.push_back(val);
        last_el = QString::number(val);
    }
    sort(els.begin(), els.end());
    drawTree(t);
}


void MainWindow::on_eraseNButton_clicked()
{
    if (els.size() == 0) return;

    std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    int cnt = ui->eraseNBox->text().toLongLong();
    for(int i = 0; i < cnt; ++i) {
        int val = rng() % els.size();
        if (tree_type == TreeType::AVL) {
            ((avl*)t)->erase(els[val]);
        } else if (tree_type == TreeType::RB) {
            ((RBTree*)t)->erase(els[val]);
        } else if (tree_type == TreeType::Splay) {
            ((SplayTree*)t)->erase(els[val]);
        } else if (tree_type == TreeType::Treap) {
            ((treap*)t)->erase(els[val]);
        }
        n--;
        els.erase(els.begin() + val, els.begin() + val + 1);
        last_el = QString::number(val);
    }
    drawTree(t);
}


void MainWindow::on_searchButton_clicked()
{
    long long val = ui->findBox->text().toLongLong();
    if (selected != nullptr) {
        selected->node->_ellipse->setPen(QPen(selected->node->_color));
        selected->node->update();
    }
    if (tree_type == TreeType::AVL) {
        auto p = ((avl*)t)->find(val);
        if (p == nullptr) return;
        p->node->_ellipse->setPen(QPen(Qt::blue, 2));
        p->node->update();
        selected = p;
    } else if (tree_type == TreeType::RB) {
        auto p = ((RBTree*)t)->find(val);
        if (p == nullptr) return;
        p->node->_ellipse->setPen(QPen(Qt::blue, 2));
        p->node->update();
        selected = p;
    } else if (tree_type == TreeType::Splay) {
        auto p = ((SplayTree*)t)->find(val);
        if (p == nullptr) return;
        drawTree(t);
        p = ((SplayTree*)t)->find(val);
        if (p->key != val) return;
        p->node->_ellipse->setPen(QPen(Qt::blue, 2));
        p->node->update();
        selected = p;
    } else if (tree_type == TreeType::Treap) {
        auto p = ((treap*)t)->find(val);
        if (p == nullptr) return;
        drawTree(t);
        p = ((treap*)t)->find(val);
        if (p->key != val) return;
        p->node->_ellipse->setPen(QPen(Qt::blue, 2));
        p->node->update();
        selected = p;
    }
    if (selected != nullptr) {
        last_el = QString::number(val);
    }
}


void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    switch(index) {
    case 0:
        t = new avl();
        drawTree(t);
        tree_type = TreeType::AVL;
        n = 0;
        updateInfo();
        break;
    case 1:
        t = new RBTree();
        drawTree(t);
        tree_type = TreeType::RB;
        n = 0;
        updateInfo();
        break;
    case 2:
        t = new treap();
        drawTree(t);
        tree_type = TreeType::Treap;
        n = 0;
        updateInfo();
        break;
    case 3:
        t = new SplayTree();
        drawTree(t);
        tree_type = TreeType::Splay;
        n = 0;
        updateInfo();
        break;
    }
}

