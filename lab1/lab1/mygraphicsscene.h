#ifndef MYGRAPHICSSCENE_H
#define MYGRAPHICSSCENE_H

#include <QObject>
#include <QGraphicsScene>

class MyGraphicsScene : public QObject
{
    Q_OBJECT
public:
    MyGraphicsScene(QObject *parent = nullptr) : QGraphicsScene(parent) {}

signals:

};

#endif // MYGRAPHICSSCENE_H
