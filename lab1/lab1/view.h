#ifndef VIEW_H
#define VIEW_H

#include <QObject>

#include <QLabel>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QKeyEvent>
#include "model.h"

class CustomScene : public QGraphicsScene
{
public:
    CustomScene(QObject *parent = nullptr) : QGraphicsScene(parent) {}
};

class View : public QObject
{
    Q_OBJECT
public:
    // explicit View(QGraphicsView *graphics_view, QObject *parent = nullptr);
    explicit View(QGraphicsView *graphics_view, QLabel *info_label, QObject *parent = nullptr);
    // explicit View(QObject *parent = nullptr);
    ~View();

    void set_model(Model *model);

    // MyGraphicsView *get_graphics_view();

    void draw_point(const Point &point, const QColor &color);
    void draw_polygon(const std::vector<Point> &polygon, const QColor &color);
    void draw_circle(const Point &center, double radius, const QColor &color);
    void draw_text(const QString &str, const Point &point, const QColor &color);
    void draw_points();
    void draw_solution();
    void clear();

private:
    Model *model = nullptr;

    // MyGraphicsView *view;
    QGraphicsView *view;
    CustomScene *scene;

    QLabel *info_label;

    int canvas_topleft_x;
    int canvas_topleft_y;
    int canvas_width;
    int canvas_height;

public slots:
    void key_press_event(QKeyEvent *event);

private slots:
    void model_updated();

signals:

};

#endif // VIEW_H
