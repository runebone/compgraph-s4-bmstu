#ifndef VIEW_H
#define VIEW_H

#include "model.h"

#include <QObject>
#include <QGraphicsView>
#include <QScrollArea>
#include <QLabel>
#include <QVBoxLayout>

#include "mypointwidget.h"
#include "mygraphicsview.h"
#include "mygraphicsscene.h"

class View : public QObject
{
    Q_OBJECT
public:
    explicit View(MyGraphicsView *graphicsView, QScrollArea *leftScrollArea, QScrollArea *rightScrollArea, QLabel *infoLabel, QObject *parent = nullptr);

    // XXX: Just for connecting the signals of MyPointWidgets
    void setModel(Model *model);

    // Just for Controller to connect some signals to the view
    MyGraphicsView *getView() const;

private:
    MyGraphicsView *m_view;
    MyGraphicsScene *m_scene;

    QVBoxLayout *m_leftLayout;
    QVBoxLayout *m_rightLayout;

    QLabel *m_infoLabel;

    // XXX: Just for connecting the signals of MyPointWidgets
    Model *m_model;

    // XXX: Ugly way
    void addMyPointWidgetToLayout(QLayout *layout, MyPointWidget *mpw);
    void addPointsToLayout(QLayout *layout, const std::vector<Point> &points, Set set);

public slots:
    void on_model_updated(ModelData);

    void on_clear_screen_clicked();

    void on_key_pressed(QKeyEvent*);
    void on_left_mouse_clicked(QMouseEvent*);
    void on_right_mouse_clicked(QMouseEvent*);
    void on_middle_mouse_clicked(QMouseEvent*);

private slots:
    // XXX: Ugly way
    void on_point_x_changed(size_t, double, Set);
    void on_point_y_changed(size_t, double, Set);
    void on_point_remove_clicked(size_t, Set);
    void on_invalid_input_left(size_t);
    void on_invalid_input_right(size_t);

signals:

};

// XXX extreme ugliness
bool operator==(const Point lhs, const Point rhs);

#endif // VIEW_H
