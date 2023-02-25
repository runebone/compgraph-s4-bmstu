#include "view.h"

#include <QGraphicsPolygonItem>
#include <QGraphicsTextItem>
#include <QPolygonF>

#include <vector>
#include <cstdio>
#include "point.h"

#define FIRST_SET_COLOR QColor("#ff0000")
#define SECOND_SET_COLOR QColor("#0000ff")
#define POINT_SIZE 4.0

View::View(QGraphicsView *graphics_view, QObject *parent) : QObject(parent)
// View::View(QObject *parent) : QObject(parent)
{
    this->view = graphics_view;
    // this->view = new MyGraphicsView;

    CustomScene *scene = new CustomScene;
    this->scene = scene;
    this->view->setScene(scene);
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

void View::draw_point(const Point &point, const QColor &color) {
    QGraphicsEllipseItem *point_figure = new QGraphicsEllipseItem(0, 0, POINT_SIZE, POINT_SIZE);
    point_figure->setPos(point.x, -point.y); // AOAOA
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
        std::vector<Point> hexagon1;
        std::vector<Point> hexagon2;
        std::vector<Point> polygon;
        Point polygon_mass_center;
        double area;

        if (status == OK || status == ERR::POLYGONS_DO_NOT_INTERSECT || status == ERR::LESS_THAN_THREE_POINTS_OF_POLYGONS_INTERSECTION) {
            sd.get_first_circle_center(circle_center1);
            sd.get_second_circle_center(circle_center2);

            this->draw_points();
            this->draw_point(circle_center1, Qt::black);
            this->draw_point(circle_center2, Qt::black);

            sd.get_first_hexagon(hexagon1);
            sd.get_second_hexagon(hexagon2);

            this->draw_polygon(hexagon1, FIRST_SET_COLOR);
            this->draw_polygon(hexagon2, SECOND_SET_COLOR);

            if (status == OK) {
                sd.get_polygon(polygon);
                sd.get_polygon_mass_center(polygon_mass_center);
                sd.get_area(area);

                this->draw_point(polygon_mass_center, Qt::black);
                this->draw_polygon(polygon, Qt::black);

                QString string = QString("%1 %2").arg(area).arg(polygon.size());
                qDebug() << string;

                // this->draw_text(string, polygon_mass_center, Qt::black);
            }
        }
    }
}

void View::clear()
{
    this->scene->clear();
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

    if (event->key() == Qt::Key_I) {
        this->view->setTransform(in_scale, true);
        foreach(QGraphicsItem* item, this->scene->items()) {
            // Make points constant size independently of zoom
            QGraphicsEllipseItem* ellipseItem = qgraphicsitem_cast<QGraphicsEllipseItem*>(item);
            if (ellipseItem) {
                QRectF rect = ellipseItem->rect();
                qreal new_width = rect.width() * out_scale_factor;
                qreal new_height = rect.height() * out_scale_factor;
                ellipseItem->setRect(rect.x(), rect.y(), new_width, new_height);
                continue;
            }

            QGraphicsPolygonItem* polygonItem = qgraphicsitem_cast<QGraphicsPolygonItem*>(item);
            if (polygonItem) {
                QPen pen = polygonItem->pen();
                pen.setWidthF(pen.widthF() * out_scale_factor);
                polygonItem->setPen(pen);
                continue;
            }
        }
    }
    else if (event->key() == Qt::Key_O) {
        this->view->setTransform(out_scale, true);
        foreach(QGraphicsItem* item, this->scene->items()) {
            // Make points constant size independently of zoom
            QGraphicsEllipseItem* ellipseItem = qgraphicsitem_cast<QGraphicsEllipseItem*>(item);
            if (ellipseItem) {
                QRectF rect = ellipseItem->rect();
                qreal new_width = rect.width() * in_scale_factor;
                qreal new_height = rect.height() * in_scale_factor;
                ellipseItem->setRect(rect.x(), rect.y(), new_width, new_height);
                continue;
            }

            QGraphicsPolygonItem* polygonItem = qgraphicsitem_cast<QGraphicsPolygonItem*>(item);
            if (polygonItem) {
                QPen pen = polygonItem->pen();
                pen.setWidthF(pen.widthF() * in_scale_factor);
                polygonItem->setPen(pen);
                continue;
            }
        }
    }
    else if (event->key() == Qt::Key_H) {
        foreach(QGraphicsItem* item, this->scene->items()) {
            // "* sf" gives us constant speed of movement independently of zoom
            item->setPos(item->pos().x() + translate_speed * sf, item->pos().y());
        }
    }
    else if (event->key() == Qt::Key_J) {
        foreach(QGraphicsItem* item, this->scene->items()) {
            item->setPos(item->pos().x(), item->pos().y() - translate_speed * sf);
        }
    }
    else if (event->key() == Qt::Key_K) {
        foreach(QGraphicsItem* item, this->scene->items()) {
            item->setPos(item->pos().x(), item->pos().y() + translate_speed * sf);
        }
    }
    else if (event->key() == Qt::Key_L) {
        foreach(QGraphicsItem* item, this->scene->items()) {
            item->setPos(item->pos().x() - translate_speed * sf, item->pos().y());
        }
    }
    else {
        // QGraphicsView::keyPressEvent(event);
    }
    // qDebug() << event->key();
}

void View::model_updated()
{

}
