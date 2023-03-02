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

private:
    MyGraphicsView *m_view;
    MyGraphicsScene *m_scene;

    QVBoxLayout *m_leftLayout;
    QVBoxLayout *m_rightLayout;

    QLabel *m_infoLabel;

public slots:
    void on_model_updated();

    void on_clear_screen_clicked();

    void on_key_pressed(QKeyEvent*);
    void on_left_mouse_clicked(QMouseEvent*);
    void on_right_mouse_clicked(QMouseEvent*);

signals:

};

#endif // VIEW_H
