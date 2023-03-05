#ifndef MYGRAPHICSSCENE_H
#define MYGRAPHICSSCENE_H

#include <QObject>
#include <QGraphicsScene>

class MyGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    MyGraphicsScene(QObject *parent = nullptr) : QGraphicsScene(parent) {}

protected:
    void drawBackground(QPainter* painter, const QRectF& rect) override;
signals:

};

#endif // MYGRAPHICSSCENE_H
