#ifndef MYPOINTITEM_H
#define MYPOINTITEM_H

#include <QGraphicsEllipseItem>

class MyPointItem : public QGraphicsEllipseItem
{
public:
    enum { Type = UserType + 1 }; // Override the item type

    explicit MyPointItem(qreal x, qreal y, qreal w, qreal h, QColor color, QGraphicsItem *parent = nullptr);

    int type() const override { return Type; } // Override the type() function

    void setXY(qreal x, qreal y);
    QPointF getPos();

private:
    qreal m_x, m_y;
};

#endif // MYPOINTITEM_H
