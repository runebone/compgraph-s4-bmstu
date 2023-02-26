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
    explicit View(QGraphicsView *graphics_view, QScrollArea *scroll_area, QLabel *info_label, QObject *parent = nullptr);
    ~View();

    void set_model(Model *model);

    void draw_point(const Point &point, const QColor &color);
    void draw_polygon(const std::vector<Point> &polygon, const QColor &color, bool fill = false);
    void draw_circle(const Point &center, double radius, const QColor &color);
    void draw_text(const QString &str, const Point &point, const QColor &color);
    void draw_points();
    void draw_solution();
    void clear();

    void resize_fit_all();
    void resize_fit_solution();

private:
    Model *model = nullptr;

    QGraphicsView *view;
    CustomScene *scene;

    QScrollArea *scroll_area;

    // QGraphicsRectItem *solution_rect;

    QLabel *info_label;

public slots:
    void key_press_event(QKeyEvent *event);

private slots:
    void model_updated();

signals:

};

#endif // VIEW_H
