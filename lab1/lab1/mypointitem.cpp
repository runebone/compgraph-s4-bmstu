#include "mypointitem.h"

#include <QPen>

MyPointItem::MyPointItem(qreal x, qreal y, qreal w, qreal h, QColor color, QGraphicsItem *parent)
    : QGraphicsEllipseItem(0, 0, w, h, parent)
{
    setPos(x - w/2, -y - h/2); // XXX: -y
    setXY(x, -y); // XXX: -y

    setPen(QPen(Qt::NoPen));
    setBrush(color);

    setFlag(ItemIsSelectable, true);
}

void MyPointItem::setXY(qreal x, qreal y)
{
    m_x = x;
    m_y = y;
}

QPointF MyPointItem::getPos()
{
    return QPointF(m_x, m_y);
}
