#ifndef NODEITEM_H
#define NODEITEM_H

#include <QGraphicsObject>
#include <QPixmap>
#include <QEvent>

class MainWindow;

class NodeItem : public QGraphicsObject
{
public:
    QGraphicsEllipseItem* _ellipse;
    QColor _color;

    NodeItem();
    NodeItem(int x, int y, long long value, int size, QColor color, MainWindow* p, long long prior = -1);
    ~NodeItem();

    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    QRectF boundingRect() const;
    QGraphicsLineItem* getEdge(NodeItem* a);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
private:
    float getMaximumFontSize(QString);
    int _size, _x, _y;
    long long _value, _prior;
    MainWindow* _p;
    const int _min_font = 14;
};

#endif // NODEITEM_H
