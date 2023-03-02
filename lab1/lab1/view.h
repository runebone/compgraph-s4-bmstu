#ifndef VIEW_H
#define VIEW_H

#include <QObject>
#include <QGraphicsView>
#include <QScrollArea>
#include <QLabel>
#include <QVBoxLayout>

#include "mygraphicsview.h"
#include "mygraphicsscene.h"

class View : public QObject
{
    Q_OBJECT
public:
    explicit View(MyGraphicsView *graphicsView, QScrollArea *leftScrollArea, QScrollArea *rightScrollArea, QLabel *infoLabel, QObject *parent = nullptr);
    ~View();

private:
    MyGraphicsView *m_view;
    MyGraphicsScene *m_scene;

    QVBoxLayout *m_leftLayout;
    QVBoxLayout *m_rightLayout;

    QLabel *m_infoLabel;

signals:

};

#endif // VIEW_H
