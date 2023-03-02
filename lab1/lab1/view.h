#ifndef VIEW_H
#define VIEW_H

#include <QObject>

#include <QLabel>
#include <QVBoxLayout>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QKeyEvent>

#define ENABLE_ERROR_FUNCS
#include "model.h"
#include <string>

class CustomScene : public QGraphicsScene
{
    Q_OBJECT
public:
    CustomScene(QObject *parent = nullptr) : QGraphicsScene(parent) {}

    qreal dx = 0.0;
    qreal dy = 0.0;
    qreal sf = 1.0;
};

class View : public QObject
{
    Q_OBJECT
public:
    explicit View(QGraphicsView *graphics_view, QScrollArea *scroll_area1, QScrollArea *scroll_area2, QLabel *info_label, QObject *parent = nullptr);
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

    void redraw_points();

private:
    Model *model = nullptr;

    QGraphicsView *view;
    CustomScene *scene;

    // Scroll Areas
    QVBoxLayout *layout1;
    QVBoxLayout *layout2;

    // QGraphicsRectItem *solution_rect;

    QLabel *info_label;

public slots:
    void key_press_event(QKeyEvent *event);

private slots:
    // meh
    void on_invalid_input1(int point_index);
    void on_invalid_input2(int point_index);

    void on_remove_point_clicked(int point_index, Set set);

    void model_edited_point(int point_index, Set set, Point p_old, Point p_new);
    void model_updated_points();

signals:

};

#endif // VIEW_H
