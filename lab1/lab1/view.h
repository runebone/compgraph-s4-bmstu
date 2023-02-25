#ifndef VIEW_H
#define VIEW_H

#include <QObject>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QKeyEvent>
#include "model.h"

class MyGraphicsView : public QGraphicsView {
public:
    MyGraphicsView(QWidget* parent = nullptr) : QGraphicsView(parent) {}

protected:
    void mousePressEvent(QMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            lastPos = event->pos();
            setCursor(Qt::ClosedHandCursor);
        }
    }

    void mouseMoveEvent(QMouseEvent* event) override {
        if (event->buttons() & Qt::LeftButton) {
            QPoint delta = event->pos() - lastPos;
            // horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
            // verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
            lastPos = event->pos();
        }
    }

    void mouseReleaseEvent(QMouseEvent* event) override {
        setCursor(Qt::ArrowCursor);
    }

private:
    QPoint lastPos;
};


class CustomScene : public QGraphicsScene
{
public:
    CustomScene(QObject *parent = nullptr) : QGraphicsScene(parent) {}

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override {
        QGraphicsScene::drawBackground(painter, rect);

        // Set the pen color and style for the grid lines
        QPen gridPen(Qt::gray, 1, Qt::DashLine);

        // Set the size of the grid squares
        qreal gridSize = 20;

        // Round the scene rectangle to the nearest grid square
        qreal left = int(rect.left() / gridSize) * gridSize;
        qreal top = int(rect.top() / gridSize) * gridSize;
        qreal right = int(rect.right() / gridSize) * gridSize;
        qreal bottom = int(rect.bottom() / gridSize) * gridSize;

        // Draw the vertical grid lines
        for (qreal x = left; x <= right; x += gridSize) {
            painter->setPen(gridPen);
            painter->drawLine(QPointF(x, top), QPointF(x, bottom));
        }

        // Draw the horizontal grid lines
        for (qreal y = top; y <= bottom; y += gridSize) {
            painter->setPen(gridPen);
            painter->drawLine(QPointF(left, y), QPointF(right, y));
        }
    }
};

class View : public QObject
{
    Q_OBJECT
public:
    explicit View(QGraphicsView *graphics_view, QObject *parent = nullptr);
    // explicit View(QObject *parent = nullptr);
    ~View();

    void set_model(Model *model);

    // MyGraphicsView *get_graphics_view();

    void draw_point(const Point &point, const QColor &color);
    void draw_polygon(const std::vector<Point> &polygon, const QColor &color);
    void draw_text(const QString &str, const Point &point, const QColor &color);
    void draw_points();
    void draw_solution();
    void clear();

private:
    Model *model = nullptr;

    // MyGraphicsView *view;
    QGraphicsView *view;
    CustomScene *scene;

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
