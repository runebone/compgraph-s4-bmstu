#include "mygraphicsscene.h"

#include <QPainter>

void MyGraphicsScene::drawBackground(QPainter* painter, const QRectF& rect) {
    QGraphicsScene::drawBackground(painter, rect);

    int gridSize = 1;
    int left = int(rect.left()) - (int(rect.left()) % gridSize);
    int top = int(rect.top()) - (int(rect.top()) % gridSize);

    QPen pen(Qt::DotLine);
    pen.setColor(Qt::gray);
    pen.setWidthF(0.01); // XXX

    for (qreal x = left; x < rect.right(); x += gridSize) {
        painter->setPen(pen);
        painter->drawLine(QPointF(x, rect.top()), QPointF(x, rect.bottom()));
    }

    for (qreal y = top; y < rect.bottom(); y += gridSize) {
        painter->setPen(pen);
        painter->drawLine(QPointF(rect.left(), y), QPointF(rect.right(), y));
    }
}
