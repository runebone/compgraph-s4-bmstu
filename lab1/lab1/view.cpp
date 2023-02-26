#include "view.h"

#include <QGraphicsPolygonItem>
#include <QGraphicsTextItem>
#include <QPolygonF>
#include "mypointwidget.h"

#include <vector>
#include <cstdio>
#include "point.h"

enum Side { LEFT, RIGHT, UP, DOWN };

#define FIRST_SET_COLOR QColor("#ef2f27")
#define SECOND_SET_COLOR QColor("#2c78bf")
#define SELECT_COLOR QColor("#fbb829")
#define POINT_SIZE 5.0
#define LINES_WIDTH 3.0
#define MAX_PEN_WIDTH 100.0
#define MIN_ZOOM 1e-07
#define MAX_ZOOM 200.0

View::View(QGraphicsView *graphics_view, QScrollArea *scroll_area1, QScrollArea *scroll_area2, QLabel *info_label, QObject *parent) : QObject(parent)
{
    this->view = graphics_view;

    CustomScene *scene = new CustomScene(this);
    this->scene = scene;
    this->view->setScene(scene);

    this->info_label = info_label;

    // Setup scroll area
    QVBoxLayout* layout1 = new QVBoxLayout(scroll_area1);
    QVBoxLayout* layout2 = new QVBoxLayout(scroll_area2);
    this->layout1 = layout1;
    this->layout2 = layout2;

    QWidget* widget1 = new QWidget(scroll_area1);
    QWidget* widget2 = new QWidget(scroll_area2);
    widget1->setLayout(layout1);
    widget2->setLayout(layout2);

    // Point p = { .x = 1, .y = 2 };
    // MyPointWidget* mpw = new MyPointWidget(0, p);
    // layout1->addWidget(mpw);

    scroll_area1->setWidget(widget1);
    scroll_area2->setWidget(widget2);
}

View::~View()
{
    delete this->scene;
}

void View::set_model(Model *model)
{
    this->model = model;

    connect(model, SIGNAL(updated()), this, SLOT(model_updated_points()));
}

class PointItem : public QGraphicsEllipseItem
{
public:
    enum { Type = UserType + 1 }; // Override the item type

    PointItem(qreal x, qreal y, qreal w, qreal h, QColor color, QGraphicsItem *parent = nullptr) : QGraphicsEllipseItem(0, 0, w, h, parent)
    {
        this->setPos(x - w/2, -y - h/2); // AOAOA
        this->set_pos(x, -y); // AOAOA

        this->setPen(QPen(Qt::NoPen));
        this->setBrush(color);

        this->color = color;
    }

    int type() const override { return Type; } // Override the type() function

    void set_pos(qreal x, qreal y) {
        this->x = x;
        this->y = y;
    }

    QPointF get_pos() {
        return QPointF(this->x, this->y);
    }

    void toggle_selected(qreal width) {
        this->is_selected = !this->is_selected;

        if (this->is_selected) {
            width *= 25;
            QPen pen = this->pen();
            if (width < MAX_PEN_WIDTH) {
                pen.setStyle(Qt::SolidLine);
                pen.setColor(SELECT_COLOR);
                pen.setWidthF(width);
            }
            this->setPen(pen);
        } else {
            this->setPen(QPen(Qt::NoPen));
            this->setBrush(this->color);
        }
    }

private:
    qreal x, y;
    QColor color;
    bool is_selected = 0;
};

PointItem *get_point_item(const Point &point, const QColor &color) {
    qreal w = POINT_SIZE;
    qreal h = POINT_SIZE;

    return new PointItem(point.x, point.y, w, h, color);
}

void View::draw_point(const Point &point, const QColor &color) {
    PointItem *point_figure = get_point_item(point, color);
    this->scene->addItem(point_figure);
}

QGraphicsPolygonItem *get_polygon_item(const std::vector<Point> &polygon, const QColor &color, bool fill = false) {
    QPolygonF polygon_;
    for (Point p: polygon) {
        polygon_ << QPointF(p.x, -p.y); // AOAOA
    }

    QGraphicsPolygonItem *polygon_item = new QGraphicsPolygonItem(polygon_);
    polygon_item->setPen(QPen(color, LINES_WIDTH));

    if (fill) {
        polygon_item->setBrush(SELECT_COLOR);
    }

    return polygon_item;
}

void View::draw_polygon(const std::vector<Point> &polygon, const QColor &color, bool fill) {
    QGraphicsPolygonItem *polygon_figure = get_polygon_item(polygon, color, fill);
    this->scene->addItem(polygon_figure);
}

QGraphicsEllipseItem *get_circle_item(const Point &center, double radius, const QColor &color) {
    qreal x = 0;
    qreal y = 0;
    qreal w = 2 * radius;
    qreal h = 2 * radius;
    QGraphicsEllipseItem *circle_item = new QGraphicsEllipseItem(x, y, w, h);

    circle_item->setPos(center.x - w/2, -center.y - h/2); // AOAOA
    circle_item->setPen(QPen(color, LINES_WIDTH));

    return circle_item;
}

void View::draw_circle(const Point &center, double radius, const QColor &color)
{
    QGraphicsEllipseItem *circle_figure = get_circle_item(center, radius, color);
    this->scene->addItem(circle_figure);
}

void View::draw_text(const QString &str, const Point &point, const QColor &color)
{
    QGraphicsTextItem *text = new QGraphicsTextItem(str);
    text->setDefaultTextColor(color);
    text->setPos(point.x, -point.y); // AOAOA

    this->scene->addItem(text);
}

void View::draw_points()
{
    std::vector<Point> fsp = this->model->get_first_set_points();
    std::vector<Point> ssp = this->model->get_second_set_points();

    for (Point p: fsp) {
        this->draw_point(p, FIRST_SET_COLOR);
    }

    for (Point p: ssp) {
        this->draw_point(p, SECOND_SET_COLOR);
    }
}

void View::draw_solution()
{
    if (this->model) {
        SolutionData sd = model->get_solution_data();

        err_t status = sd.get_status();
        Point circle_center1;
        Point circle_center2;
        double circle_radius1;
        double circle_radius2;
        std::vector<Point> hexagon1;
        std::vector<Point> hexagon2;
        std::vector<Point> polygon;
        Point polygon_mass_center;
        double area;

        if (status == OK || status == ERR::POLYGONS_DO_NOT_INTERSECT || status == ERR::LESS_THAN_THREE_POINTS_OF_POLYGONS_INTERSECTION) {
            sd.get_first_circle_center(circle_center1);
            sd.get_second_circle_center(circle_center2);

            sd.get_first_circle_radius(circle_radius1);
            sd.get_second_circle_radius(circle_radius2);

            this->draw_points();
            // this->draw_point(circle_center1, Qt::black);
            // this->draw_point(circle_center2, Qt::black);

            sd.get_first_hexagon(hexagon1);
            sd.get_second_hexagon(hexagon2);

            this->draw_polygon(hexagon1, FIRST_SET_COLOR);
            this->draw_polygon(hexagon2, SECOND_SET_COLOR);

            this->draw_circle(circle_center1, circle_radius1, FIRST_SET_COLOR);
            this->draw_circle(circle_center2, circle_radius2, SECOND_SET_COLOR);

            // ===
            // QGraphicsItemGroup *solution_group = new QGraphicsItemGroup;
            //
            // QGraphicsItem *circle1 = get_circle_item(circle_center1, circle_radius1, FIRST_SET_COLOR);
            // QGraphicsItem *circle2 = get_circle_item(circle_center2, circle_radius2, SECOND_SET_COLOR);
            // solution_group->addToGroup(circle1);
            // solution_group->addToGroup(circle2);

            // QGraphicsRectItem *rect = new QGraphicsRectItem(solution_group->boundingRect());
            // rect->setPen(Qt::NoPen);

            // this->solution_rect = rect;

            // delete circle1;
            // delete circle2;
            // delete solution_group;
            // ===

            if (status == OK) {
                sd.get_polygon(polygon);
                sd.get_polygon_mass_center(polygon_mass_center);
                sd.get_area(area);

                // this->draw_point(polygon_mass_center, Qt::black);
                this->draw_polygon(polygon, Qt::black, true);

                QString info = QString("Площадь пересечения: %1").arg(area);
                this->info_label->setText(info);
                qDebug() << info;
            } else if (status == ERR::LESS_THAN_THREE_POINTS_OF_POLYGONS_INTERSECTION) {
                QString info = QString("Шестиугольники касаются друг друга. Площадь пересечения: 0");
                this->info_label->setText(info);
                qDebug() << info;
            } else if (status == ERR::POLYGONS_DO_NOT_INTERSECT) {
                QString info = QString("Шестиугольники не пересекаются.");
                this->info_label->setText(info);
                qDebug() << info;
            }
        }
    }
}

void View::clear()
{
    this->scene->clear();
}

void View::resize_fit_all()
{
    QRectF boundingRect = this->scene->itemsBoundingRect();
    this->scene->setSceneRect(boundingRect);
    this->view->fitInView(boundingRect, Qt::KeepAspectRatio);
}

void View::resize_fit_solution()
{
    // QRectF boundingRect = this->solution_rect->boundingRect();
    // this->scene->setSceneRect(boundingRect);
    // this->view->fitInView(boundingRect, Qt::KeepAspectRatio);
}

void handle_zoom(QGraphicsItem* item, qreal scale_factor) {
    // Make points constant size independently of zoom
    if (item->type() == PointItem::Type) {
        PointItem* pointItem = qgraphicsitem_cast<PointItem*>(item);

        qreal new_width = POINT_SIZE * scale_factor;
        qreal new_height = POINT_SIZE * scale_factor;

        QPointF pos = pointItem->get_pos();

        pointItem->setRect(0, 0, new_width, new_height);
        pointItem->setPos(pos.x() - new_width/2, pos.y() - new_height/2);

    } else if (item->type() == QGraphicsPolygonItem::Type) {
        QGraphicsPolygonItem* polygonItem = qgraphicsitem_cast<QGraphicsPolygonItem*>(item);

        QPen pen = polygonItem->pen();
        if (scale_factor < MAX_PEN_WIDTH) {
            pen.setWidthF(scale_factor);
        }
        polygonItem->setPen(pen);

    } else if (item->type() == QGraphicsEllipseItem::Type) {
        QGraphicsEllipseItem* ellipseItem = qgraphicsitem_cast<QGraphicsEllipseItem*>(item);

        QPen pen = ellipseItem->pen();
        if (scale_factor < MAX_PEN_WIDTH) {
            pen.setWidthF(scale_factor);
        }
        ellipseItem->setPen(pen);
    }
}

void move_point(QGraphicsItem *item, qreal dx, qreal dy) {
    // == Assert that item is point
    if (item->type() == PointItem::Type) {
        PointItem* pointItem = qgraphicsitem_cast<PointItem*>(item);
        QPointF pos = pointItem->get_pos();
        pointItem->set_pos(pos.x() + dx, pos.y() + dy);
    }
}

void move_item(QGraphicsItem *item, qreal translate_speed, qreal sf, Side where) {
    if (where == RIGHT) {
        qreal dx = translate_speed * sf;
        qreal dy = 0;
        if (item->type() == PointItem::Type) { move_point(item, dx, dy); }
        item->setPos(item->pos().x() + dx, item->pos().y() + dy);
    }
    else if (where == LEFT) {
        qreal dx = -translate_speed * sf;
        qreal dy = 0;
        if (item->type() == PointItem::Type) { move_point(item, dx, dy); }
        item->setPos(item->pos().x() + dx, item->pos().y() + dy);
    }
    else if (where == UP) {
        qreal dx = 0;
        qreal dy = -translate_speed * sf;
        if (item->type() == PointItem::Type) { move_point(item, dx, dy); }
        item->setPos(item->pos().x() + dx, item->pos().y() + dy);
    }
    else if (where == DOWN) {
        qreal dx = 0;
        qreal dy = translate_speed * sf;
        if (item->type() == PointItem::Type) { move_point(item, dx, dy); }
        item->setPos(item->pos().x() + dx, item->pos().y() + dy);
    }
}

void View::key_press_event(QKeyEvent *event)
{
    qreal in_scale_factor = 1.25;
    qreal out_scale_factor = 0.8;
    QTransform in_scale, out_scale;
    in_scale.scale(in_scale_factor, in_scale_factor);
    out_scale.scale(out_scale_factor, out_scale_factor);

    double translate_speed = 20.0;

    // Scale factor
    qreal sf = 1.0 / this->view->transform().m11();

    if (event->key() == Qt::Key_I && sf > MIN_ZOOM) {
        this->view->setTransform(in_scale, true);
        foreach(QGraphicsItem* item, this->scene->items()) {
            handle_zoom(item, sf * LINES_WIDTH);
            // Crutch for lines to have constant width when zooming in
            sf = 1.0 / this->view->transform().m11();
            handle_zoom(item, sf * LINES_WIDTH);
        }
    }
    else if (event->key() == Qt::Key_O && sf < MAX_ZOOM) {
        this->view->setTransform(out_scale, true);
        sf = 1.0 / this->view->transform().m11();
        foreach(QGraphicsItem* item, this->scene->items()) {
            handle_zoom(item, sf * LINES_WIDTH);
        }
    }
    else if (event->key() == Qt::Key_H) {
        foreach(QGraphicsItem* item, this->scene->items()) {
            // "* sf" gives us constant speed of movement independently of zoom
            move_item(item, translate_speed, sf, Side::RIGHT);
        }
    }
    else if (event->key() == Qt::Key_J) {
        foreach(QGraphicsItem* item, this->scene->items()) {
            move_item(item, translate_speed, sf, Side::UP);
        }
    }
    else if (event->key() == Qt::Key_K) {
        foreach(QGraphicsItem* item, this->scene->items()) {
            move_item(item, translate_speed, sf, Side::DOWN);
        }
    }
    else if (event->key() == Qt::Key_L) {
        foreach(QGraphicsItem* item, this->scene->items()) {
            move_item(item, translate_speed, sf, Side::LEFT);
        }
    }
    else if (event->key() == Qt::Key_Equal) {
        this->resize_fit_all();
        sf = 1.0 / this->view->transform().m11();
        foreach(QGraphicsItem* item, this->scene->items()) {
            handle_zoom(item, sf * LINES_WIDTH);
        }
    }
    else if (event->key() == Qt::Key_R) {
        this->resize_fit_solution();
        sf = 1.0 / this->view->transform().m11();
        foreach(QGraphicsItem* item, this->scene->items()) {
            handle_zoom(item, sf * LINES_WIDTH);
        }
    }
    else {
        // QGraphicsView::keyPressEvent(event);
    }
}

void View::model_edited_point(int point_index)
{

}

void View::model_updated_points()
{

}
