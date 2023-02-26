#include "view.h"

#include <QGraphicsPolygonItem>
#include <QGraphicsTextItem>
#include <QPolygonF>

#include <vector>
#include <cstdio>
#include "point.h"

enum Side { LEFT, RIGHT, UP, DOWN };

#define FIRST_SET_COLOR QColor("#ff0000")
#define SECOND_SET_COLOR QColor("#0000ff")
#define POINT_SIZE 5.0
#define MAX_PEN_WIDTH 100.0
#define MIN_ZOOM 1e-07
#define MAX_ZOOM 200.0

// View::View(QGraphicsView *graphics_view, QObject *parent) : QObject(parent)
View::View(QGraphicsView *graphics_view, QLabel *info_label, QObject *parent) : QObject(parent)
// View::View(QObject *parent) : QObject(parent)
{
    this->view = graphics_view;
    // this->view = new MyGraphicsView;

    CustomScene *scene = new CustomScene;
    this->scene = scene;
    this->view->setScene(scene);

    this->info_label = info_label;
}

View::~View()
{
    delete this->scene;
}

void View::set_model(Model *model)
{
    this->model = model;

    connect(model, SIGNAL(updated()), this, SLOT(model_updated()));
}

// MyGraphicsView *View::get_graphics_view()
// {
    // return this->view;
// }

class PointItem : public QGraphicsEllipseItem
{
public:
    enum { Type = UserType + 1 }; // Override the item type
    PointItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = nullptr) : QGraphicsEllipseItem(x, y, w, h, parent) {}

    int type() const override { return Type; } // Override the type() function

    void set_pos(qreal x, qreal y) {
        this->x = x;
        this->y = y;
    }

    QPointF get_pos() {
        return QPointF(this->x, this->y);
    }

private:
    qreal x, y;
};

void View::draw_point(const Point &point, const QColor &color) {
    qreal x = 0;
    qreal y = 0;
    qreal w = POINT_SIZE;
    qreal h = POINT_SIZE;
    PointItem *point_figure = new PointItem(x, y, w, h);

    point_figure->setPos(point.x - w/2, -point.y - h/2); // AOAOA
    // point_figure->setPos(point.x, -point.y); // AOAOA

    point_figure->set_pos(point.x, -point.y); // AOAOA

    point_figure->setPen(QPen(Qt::NoPen));
    point_figure->setBrush(color);

    this->scene->addItem(point_figure);
}

void View::draw_polygon(const std::vector<Point> &polygon, const QColor &color)
{
    QPolygonF polygon_;
    for (Point p: polygon) {
        polygon_ << QPointF(p.x, -p.y); // AOAOA
    }

    QGraphicsPolygonItem *polygon_figure = new QGraphicsPolygonItem(polygon_);
    polygon_figure->setPen(QPen(color));

    this->scene->addItem(polygon_figure);
}

void View::draw_circle(const Point &center, double radius, const QColor &color)
{
    qreal x = 0;
    qreal y = 0;
    qreal w = 2 * radius;
    qreal h = 2 * radius;
    QGraphicsEllipseItem *circle_figure = new QGraphicsEllipseItem(x, y, w, h);

    circle_figure->setPos(center.x - w/2, -center.y - h/2); // AOAOA
    circle_figure->setPen(QPen(color, 1));

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

    this->view->show();
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
            this->draw_point(circle_center1, Qt::black);
            this->draw_point(circle_center2, Qt::black);

            sd.get_first_hexagon(hexagon1);
            sd.get_second_hexagon(hexagon2);

            this->draw_polygon(hexagon1, FIRST_SET_COLOR);
            this->draw_polygon(hexagon2, SECOND_SET_COLOR);

            this->draw_circle(circle_center1, circle_radius1, FIRST_SET_COLOR);
            this->draw_circle(circle_center2, circle_radius2, SECOND_SET_COLOR);

            if (status == OK) {
                sd.get_polygon(polygon);
                sd.get_polygon_mass_center(polygon_mass_center);
                sd.get_area(area);

                this->draw_point(polygon_mass_center, Qt::black);
                this->draw_polygon(polygon, Qt::black);


                // this->draw_text(string, polygon_mass_center, Qt::black);
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

void handle_zoom(QGraphicsItem* item, qreal scale_factor, qreal sf) {
    // Make points constant size independently of zoom
    if (item->type() == PointItem::Type) {
        PointItem* pointItem = qgraphicsitem_cast<PointItem*>(item);

        QRectF rect = pointItem->rect();
        qreal new_width = rect.width() * scale_factor;
        qreal new_height = rect.height() * scale_factor;

        // pointItem->setRect(rect.x(), rect.y(), new_width, new_height);
        QPointF pos = pointItem->get_pos();

        pointItem->setRect(0, 0, new_width, new_height);
        pointItem->setPos(pos.x() - new_width/2, pos.y() - new_height/2);

        // pointItem->setPen(QPen(Qt::green));

    } else if (item->type() == QGraphicsPolygonItem::Type) {
        QGraphicsPolygonItem* polygonItem = qgraphicsitem_cast<QGraphicsPolygonItem*>(item);

        QPen pen = polygonItem->pen();
        if (sf < MAX_PEN_WIDTH) {
            pen.setWidthF(sf);
        }
        polygonItem->setPen(pen);

    } else if (item->type() == QGraphicsEllipseItem::Type) {
        QGraphicsEllipseItem* ellipseItem = qgraphicsitem_cast<QGraphicsEllipseItem*>(item);

        QPen pen = ellipseItem->pen();
        if (sf < MAX_PEN_WIDTH) {
            pen.setWidthF(sf);
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
            handle_zoom(item, out_scale_factor, sf);
        }
    }
    else if (event->key() == Qt::Key_O && sf < MAX_ZOOM) {
        this->view->setTransform(out_scale, true);
        foreach(QGraphicsItem* item, this->scene->items()) {
            handle_zoom(item, in_scale_factor, sf);
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
    else {
        // QGraphicsView::keyPressEvent(event);
    }
}

void View::model_updated()
{

}
