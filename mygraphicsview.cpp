#include "mygraphicsview.h"
#include <QScrollBar>

MyGraphicsView::MyGraphicsView(QWidget *parent) : QGraphicsView(parent)
{

}

void MyGraphicsView::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier) {
        if (event->angleDelta().x() != 0)
            horizontalScrollBar()->setValue(QAbstractScrollArea::horizontalScrollBar()->value() - event->angleDelta().x());
        else
            verticalScrollBar()->setValue(QAbstractScrollArea::verticalScrollBar()->value() -  event->angleDelta().y());
    } else {
        double angle = event->angleDelta().y();
        double factor = qPow(1.0015, angle);

        auto targetViewportPos = event->position().toPoint();
        auto targetScenePos = mapToScene(event->position().toPoint());

        scale(factor, factor);
        centerOn(targetScenePos);
        QPointF deltaViewportPos = targetViewportPos - QPointF(viewport()->width() / 2.0, viewport()->height() / 2.0);
        QPointF viewportCenter = mapFromScene(targetScenePos) - deltaViewportPos;
        centerOn(mapToScene(viewportCenter.toPoint()));
    }
}

void MyGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_originX = event->position().x();
        m_originY = event->position().y();
    }
}

void MyGraphicsView::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        QPointF oldp = mapToScene(m_originX, m_originY);
        QPointF newP = mapToScene(event->pos());
        QPointF translation = newP - oldp;

        translate(translation.x(), translation.y());

        m_originX = event->position().x();
        m_originY = event->position().y();
    }
}
