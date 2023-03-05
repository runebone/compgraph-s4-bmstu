#ifndef VIEW_H
#define VIEW_H

#include "model.h"

#include <QObject>
#include <QGraphicsView>

#include "mygraphicsview.h"
#include "mygraphicsscene.h"

class View : public QObject
{
    Q_OBJECT
public:
    explicit View(MyGraphicsView *graphicsView, QObject *parent = nullptr);

    // XXX: Just for connecting the signals of MyPointWidgets
    void setModel(Model *model);

    // Just for Controller to connect some signals to the view
    MyGraphicsView *getView() const;

private:
    MyGraphicsView *m_view;
    MyGraphicsScene *m_scene;

    // XXX: Just for connecting the signals of MyPointWidgets
    Model *m_model;

public slots:
    void on_model_updated(ModelData);

    void on_key_pressed(QKeyEvent*);
    void on_mouse_clicked(QMouseEvent*);

signals:

};

#endif // VIEW_H
