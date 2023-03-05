#include "view.h"

#include <QWidget>
#include <QGraphicsObject>
#include "solution_data.h"

#include "mypointitem.h"

#define RED QColor("#ef2f27")
#define BLUE QColor("#2c78bf")

#define FIRST_SET_COLOR RED
#define SECOND_SET_COLOR BLUE

#define FILL_COLOR QColor("#fbb829")

#define POINT_SIZE 16.0
#define LINES_WIDTH 4.0
#define MAX_PEN_WIDTH 100.0

View::View(MyGraphicsView *graphicsView, QScrollArea *leftScrollArea, QScrollArea *rightScrollArea, QLabel *infoLabel, QObject *parent)
{
    m_view = graphicsView;
    m_infoLabel = infoLabel;

    m_scene = new MyGraphicsScene(this);
    m_view->setScene(m_scene);

    m_leftLayout = new QVBoxLayout(leftScrollArea);
    m_rightLayout = new QVBoxLayout(rightScrollArea);

    // Binding layouts to ScrollAreas
    QWidget *lWidget = new QWidget(leftScrollArea);
    QWidget *rWidget = new QWidget(rightScrollArea);
    lWidget->setLayout(m_leftLayout);
    rWidget->setLayout(m_rightLayout);
    leftScrollArea->setWidget(lWidget);
    rightScrollArea->setWidget(rWidget);
}

void View::setModel(Model *model)
{
    m_model = model;
}

MyGraphicsView *View::getView() const
{
    return m_view;
}

void clearLayout(QLayout *layout) {
    QLayoutItem* item;
    QWidget* widget;

    while ((item = layout->takeAt(0)) != nullptr) {
        widget = item->widget();
        if (widget != nullptr) {
            delete widget;
        }
        delete item;
    }
}

void View::addMyPointWidgetToLayout(QLayout *layout, MyPointWidget *mpw) {
    layout->addWidget(mpw);

    connect(mpw, SIGNAL(x_input_changed(size_t,double,Set)), this, SLOT(on_point_x_changed(size_t,double,Set)));
    connect(mpw, SIGNAL(y_input_changed(size_t,double,Set)), this, SLOT(on_point_y_changed(size_t,double,Set)));
    connect(mpw, SIGNAL(remove_button_clicked(size_t,Set)), this, SLOT(on_point_remove_clicked(size_t,Set)));

    if (mpw->set() == FIRST) {
        connect(mpw, SIGNAL(invalid_input(size_t)), this, SLOT(on_invalid_input_left(size_t)));
    } else if (mpw->set() == SECOND) {
        connect(mpw, SIGNAL(invalid_input(size_t)), this, SLOT(on_invalid_input_right(size_t)));
    }
}

void View::addPointsToLayout(QLayout *layout, const std::vector<Point> &points, Set set) {
    int i = 0;
    for (Point p: points) {
        MyPointWidget *mpw = new MyPointWidget(i, p);
        mpw->set_set(set);
        addMyPointWidgetToLayout(layout, mpw);
        i++;
    }
}

void clearScene(QGraphicsScene *scene) {
    QList<QGraphicsItem*> items = scene->items();

    for (auto item : items) {
        QGraphicsObject* object = dynamic_cast<QGraphicsObject*>(item);
        if (object) {
            delete object;
        }
    }

    scene->clear();
}

void handleZoom(QGraphicsItem* item, qreal scale_factor) {
    // Make points constant size independently of zoom
    if (item->type() == MyPointItem::Type) {
        MyPointItem* pointItem = qgraphicsitem_cast<MyPointItem*>(item);

        qreal new_width = POINT_SIZE * scale_factor;
        qreal new_height = POINT_SIZE * scale_factor;

        QPointF pos = pointItem->getPos();

        pointItem->setRect(0, 0, new_width, new_height);
        pointItem->setPos(pos.x() - new_width/2, pos.y() - new_height/2);

        QPen pen = pointItem->pen();
        pen.setWidthF(LINES_WIDTH * scale_factor);
        pointItem->setPen(pen);
    } else if (item->type() == QGraphicsPolygonItem::Type) {
        QGraphicsPolygonItem* polygonItem = qgraphicsitem_cast<QGraphicsPolygonItem*>(item);

        QPen pen = polygonItem->pen();
        if (scale_factor < MAX_PEN_WIDTH) {
            pen.setWidthF(LINES_WIDTH * scale_factor);
        }
        polygonItem->setPen(pen);
    } else if (item->type() == QGraphicsEllipseItem::Type) {
        QGraphicsEllipseItem* ellipseItem = qgraphicsitem_cast<QGraphicsEllipseItem*>(item);

        QPen pen = ellipseItem->pen();
        if (scale_factor < MAX_PEN_WIDTH) {
            pen.setWidthF(LINES_WIDTH * scale_factor);
        }
        ellipseItem->setPen(pen);
    }
}


namespace draw {
void point(QGraphicsScene *scene, const Point &p, QColor color) {
    MyPointItem *figure = new MyPointItem(p.x, p.y, POINT_SIZE, POINT_SIZE, color);
    scene->addItem(figure);
}

void circle(QGraphicsScene *scene, const Point &center, double radius, QColor color) {
    qreal w = 2 * radius;
    qreal h = 2 * radius;
    QGraphicsEllipseItem *figure = new QGraphicsEllipseItem(0, 0, w, h);

    figure->setPos(center.x - w/2, -center.y - h/2); // XXX: -y
    figure->setPen(QPen(color, LINES_WIDTH));

    scene->addItem(figure);
}

void polygon(QGraphicsScene *scene, const std::vector<Point> &vertices, QColor color, bool fill = false) {
    QPolygonF polygon;

    for (Point p: vertices) {
        polygon << QPointF(p.x, -p.y); // XXX: -y
    }

    QGraphicsPolygonItem *figure = new QGraphicsPolygonItem(polygon);
    figure->setPen(QPen(color, LINES_WIDTH));

    if (fill) {
        figure->setBrush(FILL_COLOR);
    }

    scene->addItem(figure);
}

void model(QGraphicsScene *scene, const ModelData &md, QColor first = FIRST_SET_COLOR, QColor second = SECOND_SET_COLOR) {
    SolutionData sd = md.solutionData;
    err_t status = sd.get_status();

    for (Point p: md.firstSetPoints) {
        draw::point(scene, p, first);
    }

    for (Point p: md.secondSetPoints) {
        draw::point(scene, p, second);
    }

    if (isNotError(status)) {
        std::vector<Point> polygon;
        Point center;
        double radius;

        // Drawing primitives from the first set
        sd.get_first_hexagon(polygon);
        draw::polygon(scene, polygon, first);
        sd.get_first_circle_center(center);
        sd.get_first_circle_radius(radius);
        draw::circle(scene, center, radius, first);

        // Drawing primitives from the second
        sd.get_second_hexagon(polygon);
        draw::polygon(scene, polygon, second);
        sd.get_second_circle_center(center);
        sd.get_second_circle_radius(radius);
        draw::circle(scene, center, radius, second);

        if (status == OK) {
            // Draw intersection polygon
            sd.get_polygon(polygon);
            draw::polygon(scene, polygon, Qt::black, true);
        }
    }
}
}

namespace resize {
void scene(QGraphicsScene *scene, QGraphicsView *view) {
    qreal scale_factor = 1.0 / view->transform().m11();
    for (QGraphicsItem *item : scene->items()) {
        handleZoom(item, scale_factor);
    }
}

void fit(QGraphicsScene *scene, QGraphicsView *view) {
    view->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
    resize::scene(scene, view);
}
}

void View::on_model_updated(ModelData md)
{
    clearLayout(m_leftLayout);
    clearLayout(m_rightLayout);

    addPointsToLayout(m_leftLayout, md.firstSetPoints, FIRST);
    addPointsToLayout(m_rightLayout, md.secondSetPoints, SECOND);

    clearScene(m_scene);

    draw::model(m_scene, md);

    QRectF r = m_scene->itemsBoundingRect();

    m_view->fitInView(r, Qt::KeepAspectRatio);

    resize::scene(m_scene, m_view);

    resize::fit(m_scene, m_view); // XXX

    {
        QString info;
        err_t status = md.solutionData.get_status();

        if (status == OK) {
            double area;
            md.solutionData.get_area(area);
            info = QString("Площадь пересечения: %1").arg(area);
        } else if (status == ERR::LESS_THAN_THREE_POINTS_OF_POLYGONS_INTERSECTION) {
            info = QString("Шестиугольники касаются друг друга. Площадь пересечения: 0.0");
        } else if (status == ERR::POLYGONS_DO_NOT_INTERSECT) {
            info = QString("Шестиугольники не пересекаются.");
        } else {
            std::string error_description = getErrorDescription(status);
            info = QString::fromStdString(error_description);
        }

        m_infoLabel->setText(info);
    }
}

void View::on_clear_screen_clicked()
{
    clearScene(m_scene);
}


void View::on_left_mouse_clicked(QMouseEvent *event)
{
    QPoint viewPos = m_view->mapFromGlobal(QCursor::pos());
    QPointF scenePos = m_view->mapToScene(viewPos);

    qDebug() << scenePos;

    Point p = { scenePos.x(), -scenePos.y() }; // XXX: -y

    m_model->add_point(p, FIRST);
}

void View::on_right_mouse_clicked(QMouseEvent *event)
{
    QPoint viewPos = m_view->mapFromGlobal(QCursor::pos());
    QPointF scenePos = m_view->mapToScene(viewPos);

    qDebug() << scenePos;

    Point p = { scenePos.x(), -scenePos.y() }; // XXX: -y

    m_model->add_point(p, SECOND);
}

namespace selection {
void toggle(QGraphicsItem *item) {
    if (!item->isSelected()) {
        item->setSelected(true);
        item->setOpacity(0.5);

        if (item->type() == MyPointItem::Type) {
            MyPointItem *mp = qgraphicsitem_cast<MyPointItem*>(item);
            mp->setPen(QPen(FILL_COLOR, 0.1)); // XXX 0.1
        }
    } else {
        item->setSelected(false);
        item->setOpacity(1.0);

        if (item->type() == MyPointItem::Type) {
            MyPointItem *mp = qgraphicsitem_cast<MyPointItem*>(item);
            mp->setPen(Qt::NoPen);
        }
    }
}

void deselect_all_points_except_given(QGraphicsScene *scene, QGraphicsItem *my_point_item) {
    for (QGraphicsItem* item: scene->items()) {
        if (item != nullptr && item->type() == MyPointItem::Type) {
            if (item != my_point_item && item->isSelected()) {
                selection::toggle(item);
            }
        }
    }
}

MyPointItem* get_selected_point(QGraphicsScene *scene) {
    MyPointItem *mp = nullptr;

    for (QGraphicsItem *item : scene->items()) {
        if (item->isSelected() && item->type() == MyPointItem::Type) {
            mp = qgraphicsitem_cast<MyPointItem*>(item);
            break; // XXX meh
        }
    }

    return mp;
}
}

// XXX: only needed for std::find to work
bool operator==(const Point lhs, const Point rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

namespace util { // XXX: give better name
int get_selected_point_data(QGraphicsScene *scene, Point &out_point, Set &out_set) {
    MyPointItem *mp = selection::get_selected_point(scene);

    if (mp != nullptr) {
        QPointF pos = mp->getPos();
        out_point = { .x = pos.x(), .y = -pos.y() }; // XXX: -y
        out_set = mp->brush().color() == FIRST_SET_COLOR ? FIRST : SECOND; // XXX very very very bad

        qDebug() << pos;
    }

    return mp != nullptr ? 0 : -1; // XXX meh
}

// XXX bad and ugly
size_t get_point_index(Model *model, const Point &point, Set set) {
    size_t index;

    if (set == FIRST) {
        const std::vector<Point> &fsp = model->getData().firstSetPoints;
        auto item = std::find(fsp.begin(), fsp.end(), point);
        // XXX it must be there, so do not check if item is not equal to vec.end()
        index = std::distance(fsp.begin(), item);
    } else if (set == SECOND) {
        const std::vector<Point> &ssp = model->getData().secondSetPoints;
        auto item = std::find(ssp.begin(), ssp.end(), point);
        // XXX it must be there
        index = std::distance(ssp.begin(), item);
    }

    return index;
}

#include <algorithm>
void delete_selected_points(QGraphicsScene *scene, Model *model) {
    Point point;
    Set set;

    // XXX
    int rc = util::get_selected_point_data(scene, point, set);
    if (rc == 0) {
        size_t index = util::get_point_index(model, point, set);
        model->remove_point(index, set);
    }
}
}

void View::on_middle_mouse_clicked(QMouseEvent *event)
{
    QPointF cursorPos = m_view->mapToScene(event->pos());
    QGraphicsItem* item = m_scene->itemAt(cursorPos, m_view->transform());

    if (item != nullptr && item->type() == MyPointItem::Type) {
        if (!item->isSelected()) {
            selection::toggle(item);
            selection::deselect_all_points_except_given(m_scene, item);
            qDebug() << "Selected point." << qgraphicsitem_cast<MyPointItem*>(item)->getPos();
        } else {
            selection::toggle(item);
            qDebug() << "Deselected point." << qgraphicsitem_cast<MyPointItem*>(item)->getPos();
        }
    } else if (item == nullptr) {
        Point point;
        Set set;
        int rc = util::get_selected_point_data(m_scene, point, set);
        if (rc == 0) {
            size_t index = util::get_point_index(m_model, point, set);

            point = { cursorPos.x(), -cursorPos.y() }; // XXX: -y

            // qDebug() << index;
            m_model->update_point(index, point, set);
        }
    }
}

void View::on_key_pressed(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete) {
        util::delete_selected_points(m_scene, m_model);
    } else if (event->key() == Qt::Key_F5 || event->key() == Qt::Key_R) {
        m_model->dbg_emit_updated();
    } else if (event->key() == Qt::Key_Equal) {
        resize::fit(m_scene, m_view);
    } else if (event->key() == Qt::Key_I) {
        const double sf = 1.2; // XXX meh hardcode

        QTransform matrix = m_view->transform();
        matrix.scale(sf, sf);
        m_view->setTransform(matrix);

        resize::scene(m_scene, m_view);
    } else if (event->key() == Qt::Key_O) {
        const double sf = 0.8; // XXX

        QTransform matrix = m_view->transform();
        matrix.scale(sf, sf);
        m_view->setTransform(matrix);

        resize::scene(m_scene, m_view);
    }
}

void View::on_point_x_changed(size_t index, double value, Set set)
{
    Point p = m_model->get_point(index, set);
    p.x = value;
    m_model->dbg_update_point(index, p, set);
}

void View::on_point_y_changed(size_t index, double value, Set set)
{
    Point p = m_model->get_point(index, set);
    p.y = value;
    m_model->dbg_update_point(index, p, set);
}

void View::on_point_remove_clicked(size_t index, Set set)
{
    m_model->remove_point(index, set);
}

void View::on_invalid_input_left(size_t index)
{
    // TODO
}

void View::on_invalid_input_right(size_t index)
{
    // TODO
}
