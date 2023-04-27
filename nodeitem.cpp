#include "nodeitem.h"
#include <QPainter>
#include <math.h>
#include <QBitmap>
#include <QEvent>
#include "mainwindow.h"
#include <QLabel>

#define FONT_PRECISION (0.5)

NodeItem::NodeItem()
{

}
NodeItem::NodeItem(int x, int y, long long value, int size, QColor color, MainWindow* p, long long prior) {
    _x = x, _y = y, _value = value, _color = color, _size = size, _p = p, _prior = prior;
    _ellipse = new QGraphicsEllipseItem(_x, _y, _size, _size);
    QPen pen = QPen(_color, 1);
    _ellipse->setPen(pen);
    _value = value;
}
QGraphicsLineItem* NodeItem::getEdge(NodeItem* a) {
    auto r1 = a->boundingRect(), r2 = this->boundingRect();
    QGraphicsLineItem* line = new QGraphicsLineItem(r1.x() + r1.width() / 2, r1.y() + r1.height() / 2, r2.x() + r2.width() / 2, r2.y() + r2.height() / 2);
    line->setZValue(-1);
    line->setPen(QPen(Qt::black, 1));
    return line;
}
NodeItem::~NodeItem() {

}
QRectF NodeItem::boundingRect() const {
    return QRectF(_x, _y, _size, _size);
}
void NodeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    auto prevPen = painter->pen();
    auto prevBrush = painter->brush();
    painter->setBrush(Qt::white);
    painter->setPen(_ellipse->pen());
    painter->drawEllipse(_x, _y, _size, _size);
    painter->setPen(prevPen);
    painter->setBrush(prevBrush);
    if (_prior == -1) {
        QString s = QString::number(_value);
        QFont newFont = QFont("Calibri", _min_font);
        float fontSize = getMaximumFontSize(s);
        newFont.setPointSizeF(std::min(fontSize, (float)13));
        painter->setFont(newFont);
        painter->drawText(boundingRect(), Qt::AlignCenter, s);
    } else {
        QString s1 = QString::number(_value), s2 = QString::number(_prior);
        QFont newFont1 = QFont("Calibri", _min_font), newFont2 = QFont("Calibri", _min_font);
        float fontSize1 = getMaximumFontSize(s1 + "88"), fontSize2 = getMaximumFontSize(s2 + "88");
        newFont1.setPointSizeF(std::min(fontSize1, (float)_min_font));
        newFont2.setPointSizeF(std::min(fontSize2, (float)_min_font));
        painter->setFont(newFont1);
        painter->drawText(boundingRect(), Qt::AlignCenter, s1 + "\n");
        painter->setFont(newFont2);
        painter->drawText(boundingRect(), Qt::AlignCenter, "\n" + s2);
    }
}
float NodeItem::getMaximumFontSize(QString s) {
    QFont font = QFont("Calibri", _min_font);
    const QRectF widgetRect = _ellipse->boundingRect();
    const float widgetWidth = widgetRect.width();
    const float widgetHeight = widgetRect.height();

    QRectF newFontSizeRect;
    float currentSize = font.pointSizeF();

    float step = currentSize/2.0;

    /* If too small, increase step */
    if (step<=FONT_PRECISION){
        step = FONT_PRECISION*4.0;
    }

    float lastTestedSize = currentSize;

    float currentHeight = 0;
    float currentWidth = 0;
    if (s==""){
        return currentSize;
    }

    /* Only stop when step is small enough and new size is smaller than QWidget */
    while(step>FONT_PRECISION || (currentHeight > widgetHeight) || (currentWidth > widgetWidth)){
        /* Keep last tested value */
        lastTestedSize = currentSize;

        /* Test label with its font */
        font.setPointSizeF(currentSize);
        /* Use font metrics to test */
        QFontMetricsF fm(font);

        newFontSizeRect = fm.boundingRect(widgetRect, 0, s);

        currentHeight = newFontSizeRect.height();
        currentWidth = newFontSizeRect.width();

        /* If new font size is too big, decrease it */
        if ((currentHeight > widgetHeight) || (currentWidth > widgetWidth)){
            //qDebug() << "-- contentsRect()" << label->contentsRect() << "rect"<< label->rect() << " newFontSizeRect" << newFontSizeRect << "Tight" << text << currentSize;
            currentSize -=step;
            /* if step is small enough, keep it constant, so it converge to biggest font size */
            if (step>FONT_PRECISION){
                step/=2.0;
            }
            /* Do not allow negative size */
            if (currentSize<=0){
                break;
            }
        }
        /* If new font size is smaller than maximum possible size, increase it */
        else{
            //qDebug() << "++ contentsRect()" << label->contentsRect() << "rect"<< label->rect() << " newFontSizeRect" << newFontSizeRect << "Tight" << text << currentSize;
            currentSize +=step;
        }
    }
    return lastTestedSize;
}
void NodeItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    _p->eraseFunc(_value);
}
